#ifndef INCLUDED_COUNTER
#define INCLUDED_COUNTER

#include "Entity.h"

#include <string>
#include <list>

////////////////////////////////////////////////////////////
// /Counter is a script that acts a bit like a TriggerZone,
// /in that it can disable/enable/swap entities state.
// /Unlike TriggerZone, it only does this when its counter 
// /reaches a certain number.
////////////////////////////////////////////////////////////
class Counter : public Entity {
public:
	enum ActivateReason {
		REASON_LS,
		REASON_LE,
		REASON_GR,
		REASON_GE,
		REASON_EQ,
		REASON_NE
	};

	enum ActivateAction {
		ACTION_SWAP,
		ACTION_ENABLE,
		ACTION_DISABLE
	};

	////////////////////////////////////////////////////////////
	// /Create a counter that activates when its count reaches a certain amount.
	// /by default, it activates when its count is equal to the amount.
	////////////////////////////////////////////////////////////
	Counter(const std::list<std::string>& entList, int count, bool startsEnabled, SubLevel* subLevel_p, ActivateAction action = ACTION_SWAP, ActivateReason reason = REASON_EQ);

	////////////////////////////////////////////////////////////
	// /Set the count of the counter.
	////////////////////////////////////////////////////////////
	void setCount(int count);

	////////////////////////////////////////////////////////////
	// /Get the count of the counter.
	////////////////////////////////////////////////////////////
	int getCount() const{return mCount;};

	void update(SubLevel* subLevel_p){}

private:

	SubLevel* mSubLevel_p;

	std::list<std::string> mEntList;

	ActivateReason mReason;

	ActivateAction mAction;

	int mCount;

	int mCurrentCount;
};

#endif