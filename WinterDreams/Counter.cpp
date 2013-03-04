#include "Counter.h"
#include "SubLevel.h"

#include <boost/foreach.hpp>
#include <cassert>

Counter::Counter(const std::list<std::string>& entList, int count, bool startsEnabled, SubLevel* subLevel_p, Counter::ActivateAction action, Counter::ActivateReason reason):
	Script(startsEnabled),
	mSubLevel_p(subLevel_p),
	mEntList(entList),
	mReason(reason),
	mAction(action),
	mCount(count),
	mCurrentCount(0)
{
}

void Counter::setCount(int count) {
	if(getEnabled() == false)
		return;

	auto activate = false;

	switch(mReason) {
	case REASON_LS:
		activate = count < mCount ? true : false; break;
	case REASON_LE:
		activate = count <= mCount ? true : false; break;
	case REASON_GR:
		activate = count > mCount  ? true : false; break;
	case REASON_GE:
		activate = count >= mCount ? true : false; break;
	case REASON_EQ:
		activate = count == mCount ? true : false; break;
	case REASON_NE:
		activate = count != mCount ? true : false; break;
	}

	if(activate) {

		BOOST_FOREACH(const auto& name, mEntList) {

			if(auto entity_sp = mSubLevel_p->getEntity(name).lock()) {
				switch(mAction) {
				case ACTION_SWAP: entity_sp->swapEnabled(); break;
				
				case ACTION_ENABLE: entity_sp->setEnabled(true); break;
				case ACTION_DISABLE: entity_sp->setEnabled(false); break;
				default: assert(0 && "not all actions in switch");
				}
			}
		}
	}
}