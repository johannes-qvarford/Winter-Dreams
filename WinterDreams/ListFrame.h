#ifndef INCLUDED_LISTFRAME
#define INCLUDED_LISTFRAME

#include "Widget.h"
#include <vector>
#include <memory>

////////////////////////////////////////////////////////////
// /A frame that moves over a list of Widgets when the user
// /presses KEY_UP and KEY_DOWN. It activates that current widget
// /when the user pushes a button except those two.
// /It calls onHover for the widgets it traverses.
////////////////////////////////////////////////////////////
class ListFrame : public Widget {
public:

	////////////////////////////////////////////////////////////
	// /Create a list frame at a position, that traverses a list
	// /of widgets. The list is from top to bottom, graphically.
	////////////////////////////////////////////////////////////
	ListFrame(const std::vector<std::shared_ptr<Widget> >& widgets);

	////////////////////////////////////////////////////////////
	// /Is always active, cannot be activated.
	////////////////////////////////////////////////////////////
	void activate();

	////////////////////////////////////////////////////////////
	// /Do nothing.
	////////////////////////////////////////////////////////////
	void onHover(bool doHover);

	////////////////////////////////////////////////////////////
	// /Check for button presses, and activate and hover over
	// /Widgets.
	////////////////////////////////////////////////////////////
	void update(MenuState* state_p);

	////////////////////////////////////////////////////////////
	// /draw a frame in debug mode.
	////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

	////////////////////////////////////////////////////////////
	// /Get the graphical bounds of the frame.
	////////////////////////////////////////////////////////////
	const sf::FloatRect& getBounds() const;

	////////////////////////////////////////////////////////////
	// /Get the graphical bounds of the frame.
	////////////////////////////////////////////////////////////
	sf::FloatRect* getBounds();

private:

	void resizeFrame();

	sf::FloatRect mBounds;

	std::vector<std::shared_ptr<Widget> > mWidgets;

	int mCooldownFrames;

	int mCurrentWidget;
};

#endif