#include "TriggerZone.h"
#include "GameState.h"
#include "Player.h"

//number of times that a zones update method need
//be called without it touching a player
//to mean that the player has left the zone.
static const int EXIT_FRAMES = 2;

TriggerZone::TriggerZone(const sf::FloatRect& hitBox, const std::string& onEnterName, const std::string& onExitName, int lightLevel, bool triggerOnce):
	CollisionZone(triggerOnce, hitBox, triggerOnce),
	mUpdatesSinceLastTouch(EXIT_FRAMES + 1),
	mInZone(false),
	mLightLevel(lightLevel),
	mEnterName(onEnterName),
	mExitName(onExitName),
	mGameState(NULL)
{
}

void TriggerZone::update(GameState* state) {
	mUpdatesSinceLastTouch++;
	mUpdatesSinceLastTouch %= (EXIT_FRAMES + 2);

	//we have found our game state
	mGameState = state;

	//if there have been a few frames since last touch,
	//and the player is in the zone,
	//then the player has exited the zone.
	if(mInZone && mUpdatesSinceLastTouch == EXIT_FRAMES) {
		
		mInZone = false;

		//do nothing if entity is dead
		auto entity_wp = mGameState->getEntity(mEnterName);
		if(entity_wp.expired())
			return;

		//swap enabled state of entity
		std::shared_ptr<Entity> entity_sp(entity_wp);
		entity_sp->swapEnabled();
	}
}

void TriggerZone::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection) {
	
	//only collide with players with a high enough light level.
	auto player_p = dynamic_cast<Player*>(entityCollidedWith_p);
	if(player_p == nullptr || player_p->getCurrentLightLevel() < mLightLevel)
		return;


	//if the player isn't in the zone yet,
	//then the player has entered the zone
	if(mInZone == false) {
		//do nothing if entity is dead
		auto entity_wp = mGameState->getEntity(mExitName);
		if(entity_wp.expired())
			return;

		//swap enabled state of entity
		std::shared_ptr<Entity> entity_sp(entity_wp);
		entity_sp->swapEnabled();

		//destroy self if it's a one time trigger
		CollisionZone::onCollision(entityCollidedWith_p, intersection);
	}

	mInZone = true;
	
	//reset
	mUpdatesSinceLastTouch = 0;
}