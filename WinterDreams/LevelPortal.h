#ifndef INCLUDED_LEVELPORTAL
#define INCLUDED_LEVELPORTAL

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

#include "Animation.h"
#include <string>

class LevelState;
//////////////////////////////////////////////////////////////
// /A level portal teleports the players view to another level (or
// /somewhere else on the same level.
// /WARNING: non-conformance: no enabledOnce!
//////////////////////////////////////////////////////////////
class LevelPortal : public Entity, public Drawable, public Collidable, public BaseHitBoxHaveable {
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
	LevelPortal(const sf::FloatRect& position, SubLevel* level, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, const sf::Vector2i& direction);
	
	//////////////////////////////////////////////////////////////
	// /A level portal has an animation pointer it needs to destroy
	//////////////////////////////////////////////////////////////	
	~LevelPortal();
	//////////////////////////////////////////////////////////////
	// /Checks the type of the entity with which it collides. If it
	// /is a player, the player is teleported to the targetPortal
	//////////////////////////////////////////////////////////////
	void onCollision(Collidable* pe, const sf::Rect<float>& intersection);

	void draw();

	void update(SubLevel* subLevel_p);

private:
	bool mIsWaiting;
	sf::Vector2i mDirection;
	int mWaitingFrames;
	SubLevel* mSubLevel_p;
	std::string mTargetLevel;
	std::string mTargetPortal;

	sf::Sound mSound;
	std::shared_ptr<sf::SoundBuffer> mSoundBuffer_sp;

	//No copies
	LevelPortal(const LevelPortal& lp);
	//No copies
	LevelPortal& operator=(const LevelPortal& lp);

	Animation*				 mCurrentAnimation_p; //The portals's current animation
};

#endif