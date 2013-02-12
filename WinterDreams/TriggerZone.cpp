#include "TriggerZone.h"
#include "SubLevel.h"
#include "Player.h"
#include "WindowManager.h"
#include "GameToScreen.h"

#ifdef _DEBUG
#include <SFML/Graphics/VertexArray.hpp>
#endif

//number of times that a zones update method need
//be called without it touching a player
//to mean that the player has left the zone.
static const int EXIT_FRAMES = 2;

TriggerZone::TriggerZone(const sf::FloatRect& hitBox, const std::list<std::string>& onEnterNames, const std::list<std::string>& onExitNames, int lightLevel, bool triggerOnce, bool startEnabled):
	CollisionZone(startEnabled, hitBox, triggerOnce),
	mUpdatesSinceLastTouch(EXIT_FRAMES + 1),
	mInZone(false),
	mLightLevel(lightLevel),
	mEnterNames(onEnterNames),
	mExitNames(onExitNames),
	mSubLevel_p(NULL)
{
}

void TriggerZone::update(SubLevel* subLevel_p) {
	mUpdatesSinceLastTouch++;
	mUpdatesSinceLastTouch %= (EXIT_FRAMES + 2);

	//we have found our sublevel
	mSubLevel_p = subLevel_p;

	//if there have been a few frames since last touch,
	//and the player is in the zone,
	//then the player has exited the zone.
	if(mInZone && mUpdatesSinceLastTouch == EXIT_FRAMES) {

		mInZone = false;

		//do nothing if not enabled
		if(getEnabled() == false)
			return;
		
		for(auto it = mEnterNames.begin(), end = mExitNames.end(); it != end; ++it) {
			auto& name = *it;
			auto entity_wp = mSubLevel_p->getEntity(name);
			
			//do nothing if entity is dead
			if(entity_wp.expired())
				return;

			//swap enabled state of entity
			std::shared_ptr<Entity> entity_sp(entity_wp);
			entity_sp->swapEnabled();
		}
	}
}

void TriggerZone::drawSelf() {
#ifdef _DEBUG

	sf::Vertex vertices[] =
	{
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top), sf::Color::Red, sf::Vector2f( 0,  0)),
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f( 0, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top), sf::Color::Red, sf::Vector2f(10,  0))
	};

	auto& window = *WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	//translate to screen coordinates
	states.transform *= GAME_TO_SCREEN;

	window.draw(vertices, 4, sf::LinesStrip, states);
#endif
}

void TriggerZone::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection) {
	
	//only collide with players with a high enough light level.
	auto player_p = dynamic_cast<Player*>(entityCollidedWith_p);
	if(player_p == nullptr || player_p->getCurrentLightLevel() < mLightLevel)
		return;


	//if the player isn't in the zone yet,
	//then the player has entered the zone
	if(mInZone == false) {
		mInZone = true;

		//do nothing if not enabled
		if(getEnabled() == false)
			return;

		for(auto it = mExitNames.begin(), end = mExitNames.end(); it != end; ++it) {
			auto& name = *it;

			//do nothing if entity is dead
			auto entity_wp = mSubLevel_p->getEntity(name);
			if(entity_wp.expired())
				return;

			//swap enabled state of entity
			std::shared_ptr<Entity> entity_sp(entity_wp);
			entity_sp->swapEnabled();

			//disable self if it's a one time trigger
			CollisionZone::onCollision(entityCollidedWith_p, intersection);
		}
	}


	//reset
	mUpdatesSinceLastTouch = 0;
}