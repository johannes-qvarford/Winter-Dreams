#ifndef INCLUDED_TRIGGERZONE
#define INCLUDED_TRIGGERZONE

#include "Entity.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

class SubLevel;

///////////////////////////////////////////////
// /Trigger zones are entities with a set collision box.
///////////////////////////////////////////////
class TriggerZone : public Entity, public Collidable, public BaseHitBoxHaveable {
public:
	enum Action {
		ACTION_SWAP,
		ACTION_ENABLE,
		ACTION_DISABLE
	};

	///////////////////////////////////////////////
	// /Create a TriggerZone, that can trigger once or several times, only at a certain lightlevel, only when the player has certain items.
	// /When it triggers, it affect its targets in a certain way.
	// /If triggering only once, onExitName is ignored.
	///////////////////////////////////////////////
	TriggerZone(const sf::FloatRect& hitBox, Action action, const std::list<std::string>& onEnterNames, const std::list<std::string>& onExitNames, std::list<std::string> requiredItems, std::list<std::string> absenceRequiredItems, int lightLevel, SubLevel* subLevel_p, bool triggerOnce, bool startEnabled); 
	
	///////////////////////////////////////////////
	// /swap exit entities enabled state when a player
	// /hasn't touched the zone in a few frames.
	///////////////////////////////////////////////
	void update(SubLevel* state);

	///////////////////////////////////////////////
	// /draw the trigger zone in debug mode.
	///////////////////////////////////////////////
	void drawSelf();

	///////////////////////////////////////////////
	// /swap enter entities enabled state when a player
	// /goes from not touching the zone, to touching the zone.
	///////////////////////////////////////////////
	void onCollision(Collidable* col_p, const sf::Rect<float>& intersection);

private:

	bool mOnce;

	Action mAction;

	SubLevel* mSubLevel_p;

	int mUpdatesSinceLastTouch;

	bool mInZone;

	int mLightLevel;

	std::list<std::string> mEnterNames;

	std::list<std::string> mExitNames;

	std::list<std::string> mRequiredItems;

	std::list<std::string> mAbsenceRequiredItems;
};


#endif