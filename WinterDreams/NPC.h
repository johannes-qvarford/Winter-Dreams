#ifndef INCLUDED_NPC
#define INCLUDED_NPC

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

#include "Animation.h"

#include <SFML/Graphics/Rect.hpp>
#include <string>


//////////////////////////////////////////////////////
// /NPC is an enemy that walks along a path.
// /When it collides with the player, it lowers
// /her light level.
//////////////////////////////////////////////////////
class NPC : public Entity, public Collidable, public Drawable, public BaseHitBoxHaveable{
public:

	//////////////////////////////////////////////////////
	// /Create an NPC that walks along a named path,
	// /that starts at a certain position.
	//////////////////////////////////////////////////////
	NPC(const std::string& pathName, const sf::FloatRect& initialPosition, int damage, bool startsEnabled);

	//////////////////////////////////////////////////////
	// /Move the NPC from point to point along the path.
	// /If not currently along the path; move to the 
	// /nearest point.
	//////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	//////////////////////////////////////////////////////
	// /Draw the NPC.
	//////////////////////////////////////////////////////
	void draw();

	void onCollision(Collidable * col_p, const sf::FloatRect& intersection);

private:

	sf::Sound mSound;

	std::shared_ptr<sf::SoundBuffer> mSoundBuffer;

	int mDamage;

	bool mFirstFrame;

	Animation* mCurrentAnimation_p;

	std::map<std::string, Animation> mAnimationMap;

	bool mFoundPath;

	std::string mPathName;

	const std::vector<sf::Vector2f>* mPath_p;

	int mNextPoint;
};

#endif