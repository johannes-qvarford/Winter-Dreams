#ifndef INCLUDED_LEVELPORTAL
#define INCLUDED_LEVELPORTAL

#include "CollisionZone.h"
#include <string>

class LevelState;
//////////////////////////////////////////////////////////////
// /A level portal teleports the players view to another level (or
// /somewhere else on the same level.
//////////////////////////////////////////////////////////////
class LevelPortal : public CollisionZone {
public:
	////////////////////////////////////////////////////////////////////////////
	// /The levelportal has a hitbox, which also represents the portals position.
	// /
	// /The portal then needs to know which level it teleports the view too, and
	// /which portal on the targetLevel it should teleport the view too.
	// /
	// /If the targetPortal is left blank, the portal will only function as an
	// /exit portal.
	////////////////////////////////////////////////////////////////////////////
	LevelPortal(sf::FloatRect position, SubLevel* level, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce, sf::Vector2i direction);
	
	//////////////////////////////////////////////////////////////
	// /A level portal does call delete on it's LevelState-pointer
	//////////////////////////////////////////////////////////////	
	~LevelPortal();
	//////////////////////////////////////////////////////////////
	// /Checks the type of the entity with which it collides. If it
	// /is a player, the player is teleported to the targetPortal
	//////////////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection);

	void drawSelf();

	void update(SubLevel* subLevel_p);

private:
	bool mIsWaiting;
	sf::Vector2i mDirection;
	int mWaitingFrames;
	SubLevel* mSubLevel_p;
	std::string mTargetLevel;
	std::string mTargetPortal;

	//No copies
	LevelPortal(const LevelPortal& lp);
	//No copies
	LevelPortal& operator=(const LevelPortal& lp);
};

#endif