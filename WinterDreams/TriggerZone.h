#ifndef INCLUDED_TRIGGERZONE
#define INCLUDED_TRIGGERZONE

#include "CollisionZone.h"

class GameState;

///////////////////////////////////////////////
// /Trigger zones are entities with a set collision box.
///////////////////////////////////////////////
class TriggerZone : public CollisionZone {
public:

	///////////////////////////////////////////////
	// /Create a TriggerZone, that can trigger once, only at a certain lightlevel.
	///////////////////////////////////////////////
	TriggerZone(const sf::FloatRect& hitBox, bool triggerOnce, const std::string& onEnterName, const std::string& onExitName, int lightLevel); 
	
	void update(GameState* state);

	///////////////////////////////////////////////
	// /swap onEnterNames enabled state when a player
	// /goes from not touching the zone, to touching the zone.
	// /Swap onExitNames enabled state when a player
	// /goes from touching the zone, to not touching the zone.
	///////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection);

private:

	GameState* mGameState;

	bool mTouchedLastFrame;

	int mLightLevel;

	std::string mEnterName;

	std::string mExitName;
};

#endif