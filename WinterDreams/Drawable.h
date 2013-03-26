#ifndef INCLUDED_DRAWABLE
#define INCLUDED_DRAWABLE

#include "HitBoxHaveable.h"

/////////////////////////////////////////////////////////
// /Drawables are stuff that can be drawn.
// /Some stuff can be drawn directly on the screen, while
// /other stuff will be drawn in the world.
// /Drawables should not be concerned whether or not
// /they are drawn on the screen or not, this is controlled
// /on a higher level, in SubLevel.
// /Drawable which are drawn on the screen do not need a valid hitbox.
/////////////////////////////////////////////////////////
class Drawable : virtual public HitBoxHaveable {
public:

	/////////////////////////////////////////////////////////
	// Should be const, but is kept non-const for backward-compatibility.
	/////////////////////////////////////////////////////////
	virtual void draw() = 0;

	/////////////////////////////////////////////////////////
	// /Returns what layer the drawable should be drawn on.
	// /Only needed by stuff that is drawn in the world.
	/////////////////////////////////////////////////////////
	virtual int getLayer() {
		return 150;
	}
};


#endif