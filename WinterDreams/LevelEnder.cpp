#include "LevelEnder.h"
#include "LoadingVideoState.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "PropertyManager.h"
#include "StateManager.h"

LevelEnder::LevelEnder( bool startsEnabled, const sf::FloatRect& hitBox, bool once):
	CollisionZone(startsEnabled, hitBox, once),
	mSubLevel_p(nullptr)
{
}

void LevelEnder::update(SubLevel* subLevel_p) {
	//we need sublevel in onCollision.
	mSubLevel_p = subLevel_p;
}

void LevelEnder::onCollision(PhysicalEntity* physical_p, const sf::FloatRect& intersection) {
	
	if(getEnabled()) {
		CollisionZone::onCollision(physical_p, intersection);

		auto& levelName = mSubLevel_p->getLevel()->getLevelName();

		//look for next level name, based on current level name.
		auto nextLevelName = PropertyManager::get().getGeneralSettings().get<std::string>(
			"levels." + levelName + ".next_level");

		auto lvs_p = new LoadingVideoState(nextLevelName);

		auto& sm = StateManager::get();
		sm.unfreezeState(200);
		sm.popState();
		sm.pushState(lvs_p);
		sm.unfreezeState(10);
	}
}

void LevelEnder::drawSelf() {
}