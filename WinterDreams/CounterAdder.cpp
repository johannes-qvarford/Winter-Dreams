#include "CounterAdder.h"
#include "SubLevel.h"
#include "Counter.h"

CounterAdder::CounterAdder(int amount, bool startsEnabled, const std::string& counterName):
	Script(startsEnabled),
	mCounterName(counterName),
	mAmount(amount)
{
}

void CounterAdder::update(SubLevel* subLevel_p) {
	if(getEnabled()) {
		if(auto entity_sp = subLevel_p->getEntity(mCounterName).lock()) {
			auto counter_sp = std::static_pointer_cast<Counter>(entity_sp);
			counter_sp->setCount(counter_sp->getCount() + mAmount);
		}
		setEnabled(false);
	}

}