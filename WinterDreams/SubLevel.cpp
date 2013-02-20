#include "SubLevel.h"
#include "LevelState.h"
#include "Player.h"
#include "Camera.h"

#include "Script.h"
#include "GraphicalEntity.h"
#include "CollisionZone.h"

#include "WindowManager.h"
#include "GameToScreen.h"
#include "FileStructure.h"

#include <algorithm>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>
#include <iostream>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

static bool smallerPosition(std::shared_ptr<PhysicalEntity> lhs_p, std::shared_ptr<PhysicalEntity> rhs_sp);
static void handleCollision(PhysicalEntity* lhs_p, PhysicalEntity* rhs_p);

SubLevel::SubLevel(LevelState* levelState_p):
	mLevelState_p(levelState_p),
	mNameToEntity(),
	mNameToAiPath(),
	mCollisionZones(),
	mGraphicalEntities(),
	mScripts(),
	mMapTexture(),
	mBackgroundTexture()
{
}

SubLevel::~SubLevel() {
}

LevelState* SubLevel::getLevel() {
	return mLevelState_p;
}

void SubLevel::update() {

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

void SubLevel::addGraphicalEntity(std::shared_ptr<GraphicalEntity> graphical_sp){
	mGraphicalEntities.push_back(graphical_sp);
}

void SubLevel::addScript(std::shared_ptr<Script> script_sp) {
	mScripts.push_back(script_sp);
}

void SubLevel::addCollisionZone(std::shared_ptr<CollisionZone> colZone_sp) {
	mCollisionZones.push_back(colZone_sp);
}

void SubLevel::setMapTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position) {
	mMapTexture = std::make_pair(texture_sp, position);
}

void SubLevel::setBackgroundTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position) {
	mBackgroundTexture = std::make_pair(texture_sp, position);
}

void SubLevel::mapEntityToName(const std::string& name, std::weak_ptr<Entity> entity_wp) {
	mNameToEntity.insert(std::make_pair(name, entity_wp));
}

void SubLevel::mapAiPathToName(const std::string& name, const std::vector<sf::Vector2f>& path) {
	mNameToAiPath[name] = path;
}

std::weak_ptr<Entity> SubLevel::getEntity(const std::string& name) {
	return mNameToEntity[name];
}

const std::vector<sf::Vector2f>& SubLevel::getAiPath(const std::string& name) {
	return mNameToAiPath[name];
}

void SubLevel::render() {

	auto& window = *WindowManager::get().getWindow();
	auto& renderWindow = *WindowManager::get().getRenderWindow();
	auto& renderStates = *WindowManager::get().getStates();
	
	renderStates.transform = sf::Transform::Identity;
	
	//draw background
	{
		auto camPos = mLevelState_p->getCamera()->getPosition();
		auto bkg = sf::Sprite(*mBackgroundTexture.first);
		bkg.setPosition(camPos.x*0.5f-window.getSize().x/2, camPos.y*0.5f-window.getSize().y/2);
		window.draw(bkg);
	}

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
	auto viewRect = sf::FloatRect(center.x - size.x*0.6f, center.y - size.y*0.6f, size.x*1.2f, size.y*1.2f) ;

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


	//"push" new matrix
	auto oldMatrix = renderStates.transform;
//	renderStates.transform = GAME_TO_SCREEN;

	//draw paths
	for(auto it = mNameToAiPath.begin(), end = mNameToAiPath.end(); it != end; ++it) {
		auto& path = it->second;
		
		//put path in a vertex array, draw points as a linestrip.
		sf::VertexArray arr(sf::LinesStrip, path.size());

		for(size_t i = 0, size = path.size();i < size; ++i) {
			arr[i] = GAME_TO_SCREEN * path[i];
		}
		window.draw(arr, renderStates);
	}

	//"pop" new matrix
	renderStates.transform = oldMatrix;
#else

	//sort them in drawing order.
	mGraphicalEntities.sort(smallerPosition);
	
	for(auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it) {
		auto graphical_sp = *it;
	graphical_sp->drawSelf();
	}

#endif

	//display
	
	static float br=1;
	static float maxDis=0.25;
	static float lightPosx=0.5;
	static float lightPosy=0.5;
	static sf::Shader* shader = new sf::Shader;
	static bool init = false;
	if(!init)
		shader->loadFromFile(FS_DIR_SHADERS + "Darkness.frag", sf::Shader::Fragment);
	init = true;

	window.display();

	auto& p_sp = mLevelState_p->getPlayer();
	auto& pHitBox = p_sp->getHitBox();
	auto pPosition = GAME_TO_SCREEN * 
		sf::Vector2f(pHitBox.left + (pHitBox.width / 2.f), pHitBox.top + (pHitBox.height / 2.f));

	auto& c_sp = mLevelState_p->getCamera();
	auto& cPosition = c_sp->getPosition();
	
	auto winSize = window.getSize();

	auto position = sf::Vector2f(winSize.x / 2.f, winSize.y / 2.f) + (pPosition - cPosition);

	position.x /= winSize.x;
	position.y /= winSize.y;

	position.y = 1.0f - position.y;

	shader->setParameter("lightPosx[0]",position.x);
	shader->setParameter("lightPosy[0]",position.y);

	//shader->setParameter("lightPosx[0]",0.5);
	//shader->setParameter("lightPosy[0]",0.5);

	sf::Sprite renderTextureSprite(window.getTexture());
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		shader->setParameter("lightPosx[1]", float(sf::Mouse::getPosition(renderWindow).x)/float(renderWindow.getSize().x));
		shader->setParameter("lightPosy[1]", 1-float(sf::Mouse::getPosition(renderWindow).y)/float(renderWindow.getSize().y));
		shader->setParameter("brightness",br);
		shader->setParameter("maxDis",maxDis);
		renderWindow.draw(renderTextureSprite, shader);
	} else {
		renderWindow.draw(renderTextureSprite);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		br+=0.1f;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
		br-=0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
		maxDis+=0.01f;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		maxDis-=0.01f;
	}
			//draw script effects directly on screen
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		auto script_sp = *it;
		script_sp->draw();
	}

//This is statemanagers job now
//	renderWindow.display();

}

void SubLevel::deleteInactives() {
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

void SubLevel::checkCollisions(std::shared_ptr<GraphicalEntity> graphical_sp) {

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