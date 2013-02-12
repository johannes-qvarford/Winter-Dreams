#include "GameState.h"

#include "Script.h"
#include "GraphicalEntity.h"
#include "CollisionZone.h"

#include "WindowManager.h"
#include "GameToScreen.h"
#include "FileStructure.h"
#include "ResourceManager.h"

#include <algorithm>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <iostream>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

static bool smallerPosition(std::shared_ptr<PhysicalEntity> lhs_p, std::shared_ptr<PhysicalEntity> rhs_sp);
static void handleCollision(PhysicalEntity* lhs_p, PhysicalEntity* rhs_p);

GameState::GameState():
	mNameToEntity(),
	mNameToAiPath(),
	mCollisionZones(),
	mGraphicalEntities(),
	mScripts(),
	mMapTexture(),
	mBackgroundTexture()
{
}

GameState::~GameState() {
}

void GameState::update() {

	//update graphical entities.
	for(auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it) {
		auto graphical_sp = *it;

		graphical_sp->update(this);
		checkCollisions(graphical_sp);
	}

	//update collision zones.
	for(auto it = mCollisionZones.begin(), end = mCollisionZones.end(); it != end; ++it) {
		auto colZone_sp = *it;

		colZone_sp->update(this);
	}

	//update scripts
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		auto script_sp = *it;

		script_sp->update(this);
	}

	//delete inactive entities.
	deleteInactives();
}

void GameState::addGraphicalEntity(std::shared_ptr<GraphicalEntity> graphical_sp){
	mGraphicalEntities.push_back(graphical_sp);
}

void GameState::addScript(std::shared_ptr<Script> script_sp) {
	mScripts.push_back(script_sp);
}

void GameState::addCollisionZone(std::shared_ptr<CollisionZone> colZone_sp) {
	mCollisionZones.push_back(colZone_sp);
}

void GameState::setMapTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position) {
	mMapTexture = std::make_pair(texture_sp, position);
}

void GameState::setBackgroundTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position) {
	mBackgroundTexture = std::make_pair(texture_sp, position);
}

void GameState::mapEntityToName(const std::string& name, std::weak_ptr<Entity> entity_wp) {
	mNameToEntity.insert(std::make_pair(name, entity_wp));
}

void GameState::mapAiPathToName(const std::string& name, const std::vector<sf::Vector2f>& path) {
	mNameToAiPath[name] = path;
}

std::weak_ptr<Entity> GameState::getEntity(const std::string& name) {
	return mNameToEntity[name];
}

const std::vector<sf::Vector2f>& GameState::getAiPath(const std::string& name) {
	return mNameToAiPath[name];
}

void GameState::render() {

	auto& window = *WindowManager::get().getWindow();
	auto& renderWindow = *WindowManager::get().getRenderWindow();
	auto& renderStates = *WindowManager::get().getStates();
	
	//clear window for drawing, and reset transformation matrix.
	window.clear();
	renderStates.transform = sf::Transform::Identity;

	//draw map
	{
		auto sprite = sf::Sprite(*mMapTexture.first);
		sprite.setPosition(mMapTexture.second);
		window.draw(sprite);
	}
	
#ifdef DEBUG_SOLIDZONE

	std::list<std::shared_ptr<PhysicalEntity> > L;
	auto& view = WindowManager::get().getWindow()->getView();
	auto center = view.getCenter();
	auto size = view.getSize();
	auto viewRect = sf::FloatRect(center.x - size.x*0.6, center.y - size.y*0.6, size.x*1.2, size.y*1.2) ;

	for( auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it){
		auto aRect = GAME_TO_SCREEN.transformRect( (*it)->getHitBox() );
		if( viewRect.intersects( aRect ) ){	
			L.push_back( std::static_pointer_cast<PhysicalEntity>(*it) );
		}
	}
	for( auto it = mCollisionZones.begin(), end = mCollisionZones.end(); it != end; ++it){
		auto aRect = GAME_TO_SCREEN.transformRect( (*it)->getHitBox() );
		if( viewRect.intersects( aRect ) ){	
			L.push_back( std::static_pointer_cast<PhysicalEntity>(*it) );
		}
	} 
	L.sort(smallerPosition);

	for( auto a = L.begin(), b = L.end(); a != b; ++a) {	
		(*a)->drawSelf();
	}
	std::cout << "["<<L.size() <<"]"<<" ";
#else

	//sort them in drawing order.
	mGraphicalEntities.sort(smallerPosition);

	for(auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it) {
		auto graphical_sp = *it;
		graphical_sp->drawSelf();
	}

#endif


	//display
	
	static float pxt=1;
	static float lightPosx=0.5;
	static float lightPosy=0.5;
	auto shader = ResourceManager::get().getShader(FS_DIR_SHADERS + "Darkness.frag");
	window.display();

	shader->setParameter("lightPosx[0]",0.5);
	shader->setParameter("lightPosy[0]",0.5);

	sf::Sprite renderTextureSprite(window.getTexture());
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		shader->setParameter("lightPosx[1]", float(sf::Mouse::getPosition(renderWindow).x)/float(renderWindow.getSize().x));
		shader->setParameter("lightPosy[1]", 1-float(sf::Mouse::getPosition(renderWindow).y)/float(renderWindow.getSize().y));
		shader->setParameter("pixel_threshold",pxt);
		renderWindow.draw(renderTextureSprite, shader.get());
	} else {
		renderWindow.draw(renderTextureSprite);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		pxt+=0.1;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
		pxt-=0.1;
	}

		//draw script effects directly on screen
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		auto script_sp = *it;
		script_sp->draw();
	}

	renderWindow.display();

}

void GameState::deleteInactives() {
	//we need to be careful about invalid iterators
	//if we erase an iterator; it is invalidated.
	//the iterators around it are not.
	
	//iterate over graphical entities
	{
		auto it = mGraphicalEntities.begin();
		auto end = mGraphicalEntities.end();

		//iterator to next element
		auto nextIt = it;

		while(it != end) {
			++nextIt;

			auto graphical_sp = *it;

			if(graphical_sp->getAlive() == false) {
				//'it' is invalidated
				mGraphicalEntities.erase(it);
			}
			//nextIt is not invalidated either way
			it = nextIt;
		}
	}

	//iterate over collision zones
	{
		auto it = mCollisionZones.begin();
		auto end = mCollisionZones.end();

		auto nextIt = it;

		while(it != end) {
			++nextIt;

			auto colZone_sp = *it;

			if(colZone_sp->getAlive() == false) {
				mCollisionZones.erase(it);
			}

			it = nextIt;
		}
	}

	//iterare over scripts
	{
		auto it = mScripts.begin();
		auto end = mScripts.end();

		auto nextIt = it;

		while(it != end) {
			++nextIt;

			Script* script_p = it->get();

			if(script_p->getAlive() == false) {
				mScripts.erase(it);
			}

			it = nextIt;
		}
	}
}

static bool smallerPosition(std::shared_ptr<PhysicalEntity> lhs_p, std::shared_ptr<PhysicalEntity> rhs_p) {
	auto& lhsBox = lhs_p->getHitBox();
	auto& rhsBox = rhs_p->getHitBox();

	auto lhsIsoDepth = lhsBox.left + lhsBox.top + (lhsBox.width + lhsBox.height) / 2;
	auto rhsIsoDepth = rhsBox.left + rhsBox.top + (rhsBox.width + rhsBox.height) / 2;

	return lhsIsoDepth < rhsIsoDepth;
}

void GameState::checkCollisions(std::shared_ptr<GraphicalEntity> graphical_sp) {

	for(auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it) {
		auto other_sp = *it;

		//we can't collide with ourselves!
		if(graphical_sp == other_sp)
			continue;

		//check and handle collision
		handleCollision(graphical_sp.get(), other_sp.get());
	}

	for(auto it = mCollisionZones.begin(), end = mCollisionZones.end(); it != end; ++it) {
		auto other_sp = *it;

		//check and handle collision
		handleCollision(other_sp.get(), graphical_sp.get());
	}
}

static void handleCollision(PhysicalEntity* lhs_p, PhysicalEntity* rhs_p) {
	//collision boxes have negative width
	//perform check as if they had a smaller y position, and a positive height
	
	auto lhsDummy = lhs_p->getHitBox();
	auto rhsDummy = rhs_p->getHitBox();

	lhsDummy.top = lhsDummy.top + lhsDummy.height;
	lhsDummy.height = -lhsDummy.height;
	rhsDummy.top = rhsDummy.top + rhsDummy.height;
	rhsDummy.height = -rhsDummy.height;

	auto intersection = sf::FloatRect();

	if(lhsDummy.intersects(rhsDummy, intersection)) {
		lhs_p->onCollision(rhs_p, intersection);
	}
}