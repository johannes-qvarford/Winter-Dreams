#ifndef INCLUDED_NPC
#define INCLUDED_NPC

#include "GraphicalEntity.h"
#include "Animation.h"

#include <SFML/Graphics/Rect.hpp>
#include <string>


//////////////////////////////////////////////////////
// /NPC is an enemy that walks along a path.
// /When it collides with the player, it lowers
// /her light level.
//////////////////////////////////////////////////////
class NPC : GraphicalEntity {
public:

	//////////////////////////////////////////////////////
	// /Create an NPC that walks along a named path,
	// /that starts at a certain position.
	//////////////////////////////////////////////////////
	NPC(const std::string& pathName, const sf::FloatRect& initialPosition, bool startsEnabled);

	//////////////////////////////////////////////////////
	// /Move the NPC from point to point along the path.
	// /If not currently along the path; move to the 
	// /nearest point.
	//////////////////////////////////////////////////////
	void update(GameState* state);

	//////////////////////////////////////////////////////
	// /Draw the NPC.
	//////////////////////////////////////////////////////
	void drawSelf();

private:

	Animation* mCurrentAnimation_p;

	std::map<std::string, Animation> mAnimationMap;

	bool mFoundPath;

	std::string mPathName;

	const std::vector<sf::Vector2f>* mPath_p;

	int mNextPoint;

	sf::FloatRect mHitBox;
};

#endif