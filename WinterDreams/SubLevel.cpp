#include "SubLevel.h"
#include "LevelState.h"
#include "Player.h"
#include "Camera.h"

#include "Player.h"
#include "LevelState.h"

#include "Script.h"
#include "GraphicalEntity.h"
#include "CollisionZone.h"

#include "WindowManager.h"
#include "ResourceManager.h"
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
	mBackgroundTexture(),
	mLightCircleShader( ResourceManager::get().getShader( FS_DIR_SHADERS + "Darkness.frag" ) )
{
}

SubLevel::~SubLevel() {
}

LevelState* SubLevel::getLevel() {
	return mLevelState_p;
}

void SubLevel::update() {

	//static float i = 0;
	//static float j = 0;

	//std::cout << i << " " << j << std::endl;

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
	//	mMapTexture.second.x += 0.1;
	//	i += 0.1;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
	//	mMapTexture.second.x -= 0.1;
	//	i -= 0.1;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
	//	mMapTexture.second.y += 0.1;
	//	j += 0.1;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
	//	mMapTexture.second.y -= 0.1;
	//	j -= 0.1;
	//}

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
	window.clear();
	renderStates.transform = sf::Transform::Identity;
	
	//draw background
	{
		auto& camPos = mLevelState_p->getCamera()->getPosition();
		auto mapPos = mMapTexture.second + sf::Vector2f(mMapTexture.first->getSize() / static_cast<unsigned int>(2) );

		auto offset = camPos - mapPos;

		sf::Sprite bkg(*mBackgroundTexture.first);
		bkg.setOrigin( sf::Vector2f( mBackgroundTexture.first->getSize() / static_cast<unsigned int>(2) ) );
		bkg.setPosition( mapPos + offset*0.85f );		

		window.draw(bkg);
	}

	//draw map
	{
		auto sprite = sf::Sprite(*mMapTexture.first);
		sprite.setPosition(mMapTexture.second);
		window.draw(sprite);
	}



	//sort them in drawing order.
	mGraphicalEntities.sort(smallerPosition);
	
	//Calculate a viewrect, which will be used for view culling
	auto& view = WindowManager::get().getWindow()->getView();
	auto center = view.getCenter();
	auto size = view.getSize();
	auto viewRect = sf::FloatRect(center.x - size.x*0.6f, center.y - size.y*0.6f, size.x*1.2f, size.y*1.2f) ;

	for(auto it = mGraphicalEntities.begin(), end = mGraphicalEntities.end(); it != end; ++it) {
		auto transformedRect = GAME_TO_SCREEN.transformRect( (*it)->getHitBox() );
		if( viewRect.intersects( transformedRect ) ){	
			auto graphical_sp = *it;
			graphical_sp->drawSelf();
		}
	}

	//display


	window.display();

	sf::Sprite renderTextureSprite(window.getTexture());

#ifndef SHIPPING_REAL
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		renderWindow.draw(renderTextureSprite, mLightCircleShader.get() );

	} else {
		renderWindow.draw(renderTextureSprite);
	}
#else
	renderWindow.draw(renderTextureSprite, mLightCircleShader.get() );
#endif
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		auto script_sp = *it;
		script_sp->draw();

	}


//This is statemanagers job now
//	renderWindow.display();


	for( int i = 0; i < 10; ++i) {
		char brightness[] = "brightness[0]";
		char maxDis[] = "maxDis[0]";

		brightness[11] += i;
		maxDis[7] += i;
		
		mLightCircleShader->setParameter( brightness, 0 );
		mLightCircleShader->setParameter( maxDis, 1 );
	}
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

	if( lhs_p->getLayer() < rhs_p->getLayer() )
		return true;
	else if( lhs_p->getLayer() > rhs_p->getLayer() )
		return false;
	
	if( lhs_p->getMinorLayer() < rhs_p->getMinorLayer() )
		return true;
	else if( lhs_p->getMinorLayer() > rhs_p->getMinorLayer() )
		return false;

	auto lhsIsoDepth = lhsBox.left + lhsBox.top + (lhsBox.width + lhsBox.height) / 2;
	auto rhsIsoDepth = rhsBox.left + rhsBox.top + (rhsBox.width + rhsBox.height) / 2;

	return lhsIsoDepth < rhsIsoDepth;
}

void SubLevel::checkCollisions(std::shared_ptr<GraphicalEntity> graphical_sp) {
#ifndef SHIPPING_REAL
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::C ) )
		return;
#endif

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

void SubLevel::setLightPoint(const int& lightID, const sf::Vector2f& position, const float& brightness, const float& maxDis){
	auto pos = GAME_TO_SCREEN * position;

	char posX[] = "lightPosx[0]";
	char posY[] = "lightPosy[0]";
	char bright[] = "brightness[0]";
	char dist[] = "maxDis[0]";

	posX[10] += lightID;
	posY[10] += lightID;
	bright[11] += lightID;
	dist[7] += lightID;

	auto viewPos = getLevel()->getCamera()->getPosition();
	auto viewSize = WindowManager::get().getRenderWindow()->getView().getSize();
	viewPos.x -= viewSize.x / 2;
	viewPos.y -= viewSize.y / 2;

	auto x = (pos.x - viewPos.x) / viewSize.x;
	auto y = 1 -((pos.y - viewPos.y) / viewSize.y);

	mLightCircleShader->setParameter( posX, x );
	mLightCircleShader->setParameter( posY, y );
	mLightCircleShader->setParameter( bright, brightness );
	mLightCircleShader->setParameter( dist, maxDis );
}
