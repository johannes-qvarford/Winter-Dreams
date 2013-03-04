#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include "Script.h"
class SubLevel;

////////////////////////////////////////////////////////////
// /Timer is a script that ticks a certain amount of frames
// /when it gets enabled.
// /When its alarm goes of, it changes the state of a list
// /of entities, and disables itself.
////////////////////////////////////////////////////////////
class Timer : public Script {
public:
	enum TriggerAction {
		ACTION_SWAP,
		ACTION_ENABLE,
		ACTION_DISABLE
	};
	////////////////////////////////////////////////////////////
	// /Create a timer that ticks a certain amount of frames
	// /when it gets enabled.
	// /When its alarm goes of, it changes the state of a list
	// /of entities, and disables itself.
	// /its timer can never be higher than a certain amount of frames.
	////////////////////////////////////////////////////////////
	Timer(int maxFrames, int alarmFrames, const std::list<std::string>& entList, TriggerAction action, bool startsEnabled);

	////////////////////////////////////////////////////////////
	// /Tick if enabled; change entities, disable and reset if
	// /alarm goes of.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	////////////////////////////////////////////////////////////
	// /Do nothing.
	////////////////////////////////////////////////////////////
	void draw() const;

	////////////////////////////////////////////////////////////
	// /Add a certain amount of frames to the timer.
	// /Never increases the time beyond the max, and don't change
	// /if not enabled.
	////////////////////////////////////////////////////////////
	void addTime(int frames);

private:

	bool mEnabledLastFrame;

	int mMaxFrames;

	int mAlarmFrames;

	int mCurrentFrames;

	std::list<std::string> mEntList;

	TriggerAction mAction;
};

#endif