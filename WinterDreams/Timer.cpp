#include "Timer.h"
#include "SubLevel.h"

#include <boost/foreach.hpp>
#include <cassert>

Timer::Timer(int maxFrames, int alarmFrames, const std::list<std::string>& entList, Timer::TriggerAction action, bool startsEnabled):
	Script(startsEnabled),
	mMaxFrames(maxFrames),
	mAlarmFrames(alarmFrames),
	mCurrentFrames(alarmFrames),
	mEntList(entList),
	mAction(action)
{
}

void Timer::update(SubLevel* subLevel_p) {
	auto enabledThisFrame = getEnabled();

	if(enabledThisFrame) {
		--mCurrentFrames;
		if(mEnabledLastFrame == false) {
			mCurrentFrames = mAlarmFrames;
		}

		if(mCurrentFrames <= 0) {
			BOOST_FOREACH(const auto& name, mEntList) {
				auto entity_sp = subLevel_p->getEntity(name).lock();
				if (entity_sp == nullptr)
					continue;

				switch(mAction) {
				case ACTION_SWAP: entity_sp->swapEnabled(); break;
				case ACTION_ENABLE: entity_sp->setEnabled(true); break;
				case ACTION_DISABLE: entity_sp->setEnabled(false); break;
				default: assert(0 && "not all Timer::TriggerActions in switch"); break;
				}
			}
			setEnabled(false);
		}
	}
}

void Timer::draw() const {
}

void Timer::addTime(int frames) {
	mCurrentFrames += frames;
	mCurrentFrames %= (mMaxFrames + 1);
}