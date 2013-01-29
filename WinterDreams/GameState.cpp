#include "GameState.h"
#include "PhysicalEntity.h"
#include "Script.h"
#include "PlayerRelated.h"
#include "WindowManager.h"
#include "GameToScreen.h"

#include <algorithm>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>

static void checkCollisions(GameState::PhysicalEntities* physicals_p, PlayerRelated* pr_p) {
	//iterate over entities to check for collisions.
	for(auto it = physicals_p->begin(), end = physicals_p->end(); it != end; ++it)
	{
		//entity to check collisions with
		auto col_p = it->get();

		//we can't collide with ourselves!
		if(static_cast<PhysicalEntity*>(pr_p) == col_p)
			continue;

		//PlayerRelated entities can't collide with other PlayerRelated entities.
		if(dynamic_cast<PlayerRelated*>(col_p))
			continue;

		auto& prBox = pr_p->getHitBox();
		auto& colBox = col_p->getHitBox();
		auto intersection = sf::FloatRect();

		//did we intersect with the entity?
		if(prBox.intersects(colBox,intersection))
		{
			col_p->onCollision(pr_p, intersection);
		}
	}
}

static bool smallerPosition(std::shared_ptr<PhysicalEntity> lhs_p, std::shared_ptr<PhysicalEntity> rhs_p) {
	auto& lhsBox = lhs_p->getHitBox();
	auto& rhsBox = rhs_p->getHitBox();

	auto lhsIsoDepth = lhsBox.left + lhsBox.top + (lhsBox.width + lhsBox.height) / 2;
	auto rhsIsoDepth = rhsBox.left + rhsBox.top + (rhsBox.width + rhsBox.height) / 2;

	return lhsIsoDepth < rhsIsoDepth;
	/*
	auto lhsMinScreen = GAME_TO_SCREEN * sf::Vector2f(lhsBox.left, lhsBox.top);
	auto rhsMinScreen = GAME_TO_SCREEN * sf::Vector2f(rhsBox.left, rhsBox.top);
	auto lhsMaxScreen = GAME_TO_SCREEN * sf::Vector2f(lhsBox.left + lhsBox.width, lhsBox.top + lhsBox.width);
	auto rhsMaxScreen = GAME_TO_SCREEN * sf::Vector2f(rhsBox.left + rhsBox.width, rhsBox.top + rhsBox.width);

	static const float EPSILON = 0.1f;

	if(abs(lhsMaxScreen.y - rhsMaxScreen.y) < EPSILON) {
		//same max y
		if(abs(lhsMinScreen.y - rhsMinScreen.y) < EPSILON) {
			//same min y
			if(lhsMinScreen.x < rhsMinScreen.x)
				return true;
		}
		else {
			if(lhsMinScreen.y < rhsMinScreen.y)
				return true;
		}
	}
	else {
		if(lhsMaxScreen.y < rhsMaxScreen.y) {
			if(lhsMinScreen.y < rhsMinScreen.y) {
				if(lhsMinScreen.x < rhsMinScreen.x){
					if(lhsMaxScreen.x < rhsMaxScreen.x)
						return true;
				}
			}
		}
	}
	return false;
	*/
	//return true, if 
	//lhs has a smaller y coordinate, or
	//they have the same y coordinate, and lhs has a smaller x coordinate
//	return (lhsBox.top < rhsBox.top) || (lhsBox.top == rhsBox.top && lhsBox.left < rhsBox.left);
//	return (lhsBox.top + lhsBox.height < rhsBox.top + rhsBox.height) || (lhsBox.top + lhsBox.height == rhsBox.top + rhsBox.height && lhsBox.left + lhsBox.width < rhsBox.left + rhsBox.width);
}

GameState::GameState():
	mPhysicalEntities(),
	mScripts(),
	mForegroundTextures(),
	mBackgroundTextures()
{
}

GameState::~GameState() {
}

void GameState::update(int milliseconds) {

	//update entities.
	for(auto it = mPhysicalEntities.begin(), end = mPhysicalEntities.end(); it != end; ++it) {
		PhysicalEntity* entity_p = it->get();

		entity_p->update(this, milliseconds);

		//try to cast to PlayerRelated
		auto pr_p = dynamic_cast<PlayerRelated*>(entity_p);

		if(pr_p != nullptr) {
			//the entity was a PlayerRelated
			//check if it collides with something
			checkCollisions(&mPhysicalEntities, pr_p);
		}
	}

	//update scripts
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		Script* script_p = it->get();
		
		script_p->update(this, milliseconds);
	}

	//render entities and scripts. 
	render();

	//delete inactive entities.
	deleteInactives();
}

void GameState::addPhysicalEntity(std::shared_ptr<PhysicalEntity> physicalEntity_p){
	mPhysicalEntities.push_back(physicalEntity_p);
}

void GameState::addScript(std::shared_ptr<Script> script_p) {
	mScripts.push_back(script_p);
}

void GameState::setMapTexture(std::shared_ptr<sf::Texture> texture_p, const sf::Vector2f& position) {
	mMapTexture.push_back( (texture_p, position));
}

void GameState::setBackgroundTexture(std::shared_ptr<sf::Texture> texture_p) {
	mBackgroundTexture.push_back(texture_p);
}

void GameState::render() {
	//TODO: revisit to fix graphical problems that will probably occur.
	
	auto& window = *WindowManager::get().getWindow();
	auto& renderStates = *WindowManager::get().getStates();
	
	static auto view = window.getDefaultView();
	 
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		view.move(sf::Vector2f(-5, 0));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		view.move(sf::Vector2f(5, 0));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		view.move(sf::Vector2f(0, -5));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		view.move(sf::Vector2f(0, 5));
	
	window.setView(view);

	//clear window for drawing, and reset transformation matrix.
	window.clear();
	renderStates.transform = sf::Transform::Identity;

	//draw map
	{
		auto sprite = sf::Sprite(*mBackgroundTexture.first);
		sprite.setPosition(mBackgroundTexture.second);
		window.draw(sprite);
	}

	//draw background
	{
		auto sprite = sf::Sprite(*mMapTexture.first);
		sprite.setPosition(mMapTexture.second);
		window.draw(sprite);
	}

	//sort them in drawing order.
	mPhysicalEntities.sort(smallerPosition);
	
	//save old matrix for later
	//	auto oldMatrix = renderStates.transform;

	for(auto it = mPhysicalEntities.begin(), end = mPhysicalEntities.end(); it != end; ++it) {
		PhysicalEntity* physical_p = it->get();
		physical_p->drawSelf();
	}

	//get the old matrix(that is probably the identity matrix)
	//	renderStates.transform = oldMatrix;

	//draw script effects directly on screen
	for(auto it = mScripts.begin(), end = mScripts.end(); it != end; ++it) {
		Script* script_p = it->get();
		script_p->draw();
	}

	//display
	window.display();
}

void GameState::deleteInactives() {
	//we need to be careful about invalid iterators
	//if we erase an iterator; it is invalidated.
	//the iterators around it are not.
	
	//iterate over physical_entities
	{
		auto it = mPhysicalEntities.begin();
		auto end = mPhysicalEntities.end();

		//iterator to next element
		auto nextIt = it;

		while(it != end) {
			++nextIt;

			PhysicalEntity* physical_p = it->get();

			if(physical_p->isActive() == false) {
				//'it' is invalidated
				mPhysicalEntities.erase(it);
			}
			//nextIt is not invalidated either way
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

			if(script_p->isActive() == false) {
				mScripts.erase(it);
			}

			it = nextIt;
		}
	}
}