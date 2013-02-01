#include "TriggerZone.h"
#include "GameState.h"


TriggerZone::TriggerZone(const sf::FloatRect& hitBox, bool triggerOnce, const std::string& onEnterName, const std::string& onExitName, int lightLevel):
	CollisionZone(hitBox, triggerOnce, false),
	mTouchedLastFrame(false),
	mLightLevel(lightLevel),
	mEnterName(onEnterName),
	mExitName(onExitName),
	mGameState(NULL)
{
}

void TriggerZone::update(GameState* state) {
	mGameState = state;
	//on enter
	if(mTouchedLastFrame) {
		//do nothing if entity is dead
		auto entity_wp = mGameState->getEntity(mEnterName);
		if(entity_wp.expired())
			return;

		std::shared_ptr<Entity> entity_sp(entity_wp);
		entity_sp->swapEnabled();
		mTouchedLastFrame = false;
	}
}

void TriggerZone::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection) {
	//on exit
	if(!mTouchedLastFrame) {
		//do nothing if entity is dead
		auto entity_wp = mGameState->getEntity(mExitName);
		if(entity_wp.expired())
			return;

		std::shared_ptr<Entity> entity_sp(entity_wp);
		entity_sp->swapEnabled();
		mTouchedLastFrame = true;
	}
	else {
		mTouchedLastFrame = true;
	}
}