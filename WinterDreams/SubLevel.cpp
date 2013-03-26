#include "SubLevel.h"
#include "LevelState.h"
#include "Player.h"
#include "Camera.h"

#include "Player.h"
#include "LevelState.h"

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"

#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"
#include "FileStructure.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>

static bool smallerPosition(std::shared_ptr<Drawable> lhs_sp, std::shared_ptr<Drawable> rhs_sp);
static void handleCollision(Collidable* lhs_p, Collidable* rhs_p);

template<typename C, typename E>
static void deleteInactive(C* con, std::shared_ptr<E> remove_sp) {
	
	//we need to be careful about invalid iterators
	//if we erase an iterator; it is invalidated.
	//the iterators around it are not.
	{
		auto it = con->begin();
		auto end = con->end();

		//iterator to next element
		auto nextIt = it;

		while(it != end) {
			++nextIt;

			auto e_sp = *it;

			if(e_sp == remove_sp) {
				con->erase(it);
			}
			//nextIt is not invalidated either way
			it = nextIt;
		}
	}
}

SubLevel::SubLevel(LevelState* levelState_p):
	mLevelState_p(levelState_p),
	mNameToEntity(),
	mNameToAiPath(),
	mEntities(),
	mWorldDrawables(),
	mScreenDrawables(),
	mSeekers(),
	mRecievers(),
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

	//update all entities
	BOOST_FOREACH(auto& e_sp, mEntities) {
		e_sp->update(this);
	}

	//check collisions
	BOOST_FOREACH(auto& s_sp, mSeekers) {
		BOOST_FOREACH(auto& r_sp, mRecievers) {
			handleCollision(s_sp.get(), r_sp.get());
		}
	}

	//delete inactive entities
	std::vector<std::shared_ptr<Entity> > inactiveEntities;
	BOOST_FOREACH(auto& e_sp, mEntities) {
		if(e_sp->getAlive() == false) {
			inactiveEntities.push_back(e_sp);
		}
	}
	BOOST_FOREACH(auto e_sp, inactiveEntities) {
		deleteInactive(&mEntities, e_sp);
		
		//check if the entity belongs to drawables and collidables too.
		if(auto d_sp = std::dynamic_pointer_cast<Drawable>(e_sp)) {
			deleteInactive(&mScreenDrawables, d_sp);
			deleteInactive(&mWorldDrawables, d_sp);
		}
		if(auto col_sp = std::dynamic_pointer_cast<Collidable>(e_sp)) {
			deleteInactive(&mSeekers, col_sp);
			deleteInactive(&mRecievers, col_sp);
		}
	}
}

void SubLevel::addCollidable(std::shared_ptr<Collidable> col_sp, SubLevel::CollidableType type) {
	if(type == SEEK_RECIEVER) {
		mRecievers.push_back(col_sp);
	} else {
		mSeekers.push_back(col_sp);
	}
}

void SubLevel::addDrawable(std::shared_ptr<Drawable> draw_sp, SubLevel::DrawableType type){
	if(type == DRAW_SCREEN) {
		mScreenDrawables.push_back(draw_sp);
	} else {
		mWorldDrawables.push_back(draw_sp);
	}
}

void SubLevel::addEntity(std::shared_ptr<Entity> ent_sp) {
	mEntities.push_back(ent_sp);
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
	mWorldDrawables.sort(smallerPosition);
	
	//Calculate a viewrect, which will be used for view culling
	auto& view = WindowManager::get().getWindow()->getView();
	auto center = view.getCenter();
	auto size = view.getSize();
	auto viewRect = sf::FloatRect(center.x - size.x*0.6f, center.y - size.y*0.6f, size.x*1.2f, size.y*1.2f) ;

	BOOST_FOREACH(auto d, mWorldDrawables) {
		d->draw();
	}

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

	BOOST_FOREACH(auto d, mScreenDrawables) {
		d->draw();
	}

	for( int i = 0; i < 20; ++i) {

		auto md = std::string("maxDis[") + std::to_string(static_cast<long long>(i)) + "]";
		auto br = std::string("brightness[") + std::to_string(static_cast<long long>(i)) + "]";

		mLightCircleShader->setParameter( br.c_str(), 0 );
		mLightCircleShader->setParameter( md.c_str(), 1 );
	}
}

void SubLevel::deleteInactives() {
	
}

static bool smallerPosition(std::shared_ptr<Drawable> lhs_sp, std::shared_ptr<Drawable> rhs_sp) {
	auto& lhsBox = lhs_sp->getHitBox();
	auto& rhsBox = rhs_sp->getHitBox();

	if( lhs_sp->getLayer() < rhs_sp->getLayer() )
		return true;
	else if( lhs_sp->getLayer() > rhs_sp->getLayer() )
		return false;

	auto lhsIsoDepth = lhsBox.left + lhsBox.top + (lhsBox.width + lhsBox.height) / 2;
	auto rhsIsoDepth = rhsBox.left + rhsBox.top + (rhsBox.width + rhsBox.height) / 2;

	return lhsIsoDepth < rhsIsoDepth;
}

static void handleCollision(Collidable* lhs_p, Collidable* rhs_p) {
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
		rhs_p->onCollision(lhs_p, intersection);
	}
}

void SubLevel::setLightPoint(const int& lightID, const sf::Vector2f& position, const float& brightness, const float& maxDis){
	auto pos = GAME_TO_SCREEN * position;

	auto md = std::string("maxDis[") + std::to_string(static_cast<long long>(lightID)) + "]";
	auto br = std::string("brightness[") + std::to_string(static_cast<long long>(lightID)) + "]";
	auto posX = std::string("lightPosx[") + std::to_string(static_cast<long long>(lightID)) + "]";
	auto posY = std::string("lightPosy[") + std::to_string(static_cast<long long>(lightID)) + "]";

	auto viewPos = getLevel()->getCamera()->getPosition();
	auto viewSize = WindowManager::get().getRenderWindow()->getView().getSize();
	viewPos.x -= viewSize.x / 2;
	viewPos.y -= viewSize.y / 2;

	auto x = (pos.x - viewPos.x) / viewSize.x;
	auto y = 1 -((pos.y - viewPos.y) / viewSize.y);

	mLightCircleShader->setParameter( posX, x );
	mLightCircleShader->setParameter( posY, y );
	mLightCircleShader->setParameter( br, brightness );
	mLightCircleShader->setParameter( md, maxDis );
}
