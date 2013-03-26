#include "TimeAdder.h"
#include "Timer.h"
#include "SubLevel.h"


TimeAdder::TimeAdder(int addFrames, const std::string& timerName, bool startsEnabled):
	Entity(startsEnabled),
	mAddFrames(addFrames),
	mTimerName(timerName)
{
}

void TimeAdder::update(SubLevel* subLevel_p) {
	auto enabledThisFrame = getEnabled();

	if(enabledThisFrame == true) {
		setEnabled(false);

		if(auto entity_sp = subLevel_p->getEntity(mTimerName).lock()) {
			auto timer_sp = std::static_pointer_cast<Timer>(entity_sp);

			timer_sp->addTime(mAddFrames);
		}
	}
}