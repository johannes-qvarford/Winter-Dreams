#ifndef INCLUDED_TIMEADDER
#define INCLUDED_TIMEADDER

#include "Entity.h"

////////////////////////////////////////////////////////////
// /TimeAdder is a script that increases a timers counter 
// /by one, every time it's enabled. It disables itself
// /automaticaly when that happens.
////////////////////////////////////////////////////////////
class TimeAdder : public Entity {
public:

	////////////////////////////////////////////////////////////
	// /Create a time adder that takes a number of frames to add
	// /to a timer, and the name of the timer itself.
	////////////////////////////////////////////////////////////
	TimeAdder(int addFrames, const std::string& timerName, bool startsEnabled);

	////////////////////////////////////////////////////////////
	// /if the time adder got enabled this frame, add time to
	// /its timer, and disable itself.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);


private:

	int mAddFrames;

	const std::string& mTimerName;
};

#endif