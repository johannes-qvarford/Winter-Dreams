#ifndef INCLUDED_SCRIPT
#define INCLUDED_SCRIPT
#include "Animation.h"
#include "Entity.h"

////////////////////////////////////////////////////////////
// /Scripts are Entities that do not exist physically in the game world.
// /They can be effects, like rain, but not the light-ring surrounding the player.
// /(Because it has a position in the world)
////////////////////////////////////////////////////////////
class Script : public Entity {
public:
	Script( bool startEnabled ) : Entity( startEnabled ) { }
	////////////////////////////////////////////////////////////
	// /Draw an effect.
	// /The effect is expected to be on the screen, not inside the game world.
	////////////////////////////////////////////////////////////
	virtual void draw() const = 0;
};

#endif