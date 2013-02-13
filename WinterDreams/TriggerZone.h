#ifndef INCLUDED_TRIGGERZONE
#define INCLUDED_TRIGGERZONE

#include "CollisionZone.h"

class SubLevel;

///////////////////////////////////////////////
// /Trigger zones are entities with a set collision box.
///////////////////////////////////////////////
class TriggerZone : public CollisionZone {
public:

	///////////////////////////////////////////////
	// /Create a TriggerZone, that can trigger once or several times, only at a certain lightlevel.
	// /If triggering only once, onExitName is ignored.
	///////////////////////////////////////////////
	TriggerZone(const sf::FloatRect& hitBox, const std::list<std::string>& onEnterNames, const std::list<std::string>& onExitNames, int lightLevel, bool triggerOnce, bool startEnabled); 
	
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
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection);

private:

	SubLevel* mSubLevel_p;

	int mUpdatesSinceLastTouch;

	bool mInZone;

	int mLightLevel;

	std::list<std::string> mEnterNames;

	std::list<std::string> mExitNames;
};

#endif