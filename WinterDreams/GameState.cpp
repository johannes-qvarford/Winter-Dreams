#include "GameState.h"
#include "PhysicalEntity.h"
#include "Script.h"
#include "PlayerRelated.h"
#include "WindowManager.h"

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

	//return true, if 
	//lhs has a smaller y coordinate, or
	//they have the same y coordinate, and lhs has a smaller x coordinate
	return (lhsBox.top < rhsBox.top) || (lhsBox.top == rhsBox.top && lhsBox.left < rhsBox.left);
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

void GameState::render() {
	//TODO: revisit to fix graphical problems that will probably occur.
	
	auto& window = *WindowManager::get().getWindow();
	auto& renderStates = *WindowManager::get().getStates();

	//clear window for drawing, and reset transformation matrix.
	window.clear();
	renderStates.transform = sf::Transform::Identity;

	for(auto it = mBackgroundTextures.begin(), end = mBackgroundTextures.end(); it != end; ++it) {
		//draw background...
		sf::Texture& texture = *it->get();
		sf::Sprite sprite(texture);
		window.draw(sprite);
	}

	for(auto it = mForegroundTextures.begin(), end = mForegroundTextures.end(); it != end; ++it) {
		//draw foreground...
		sf::Texture& texture = *it->get();
		sf::Sprite sprite(texture);
		window.draw(sprite);
	}

	//sort them in drawing order.
	mPhysicalEntities.sort(smallerPosition);
	
	//save old matrix for later
	auto oldMatrix = renderStates.transform;

	//this entire thing could be replaced with a clockwise rotation(62.5 degrees) i think
	{
 
		//for every normal XSTEP in the game x axis, there is a translation of SC_X_XOFFSET in screen x axis, and ISO_X_YOFFSET in screen y axis.
		static const float SC_X_XOFFSET = 32;
		static const float SC_X_YOFFSET = 16;
		static const float X_STEP = 35.77708763999664;

		static const float SC_Y_XOFFSET = -32;
		static const float SC_Y_YOFFSET = 16;
		static const float Y_STEP = 35.77708763999664;

		static const sf::Transform gameToScreen(
			SC_X_XOFFSET / X_STEP,	SC_Y_XOFFSET / Y_STEP,	0,
			SC_X_YOFFSET / X_STEP,	SC_Y_YOFFSET / Y_STEP,	0,
			0,						0,						0
		);

		//translate game coordinates to screen coordinates
		renderStates.transform = gameToScreen;
	}

	for(auto it = mPhysicalEntities.begin(), end = mPhysicalEntities.end(); it != end; ++it) {
		PhysicalEntity* physical_p = it->get();
		physical_p->drawSelf();
	}

	//get the old matrix(that is probably the identity matrix)
	renderStates.transform = oldMatrix;

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