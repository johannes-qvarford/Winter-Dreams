//#ifndef INCLUDED_LEVELPORTAL
//#define INCLUDED_LEVELPORTAL
//
//#include "CollisionZone.h"
//#include <string>
////////////////////////////////////////////////////////////////
//// /A level portal teleports the players view to another level (or
//// /somewhere else on the same level.
////////////////////////////////////////////////////////////////
//class LevelPortal : public CollisionZone {
//public:
//	////////////////////////////////////////////////////////////////////////////
//	// /The levelportal has a hitbox, which also represents the portals position.
//	// /
//	// /The portal then needs to know which level it teleports the view too, and
//	// /which portal on the targetLevel it should teleport the view too.
//	// /
//	// /If the target level is left blank, the level on which the level is currently
//	// /positioned will be used as target level.
//	// /If the targetPortal is left blank, the portal will only function as an
//	// /exit portal.
//	////////////////////////////////////////////////////////////////////////////
//	LevelPortal(sf::FloatRect position, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce);
//	~LevelPortal();
//	//////////////////////////////////////////////////////////////
//	// /Checks the type of the entity with which it collides. If it
//	// /is a player, the player is teleported to the targetPortal
//	//////////////////////////////////////////////////////////////
//	void onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection);
//
//private:
//	std::string mTargetLevel;
//	std::string mTargetPortal;
//};
//
//#endif