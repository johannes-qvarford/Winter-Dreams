#ifndef INCLUDED_COUNTERADDER
#define INCLUDED_COUNTERADDER

#include "Entity.h"

////////////////////////////////////////////////////////////
// /CounterAdder is script that adds certain amount to a
// /counter when it gets enabled, at which it will disable 
// /itself.
// /
// /WARNING: Should use onChangeEnabled
////////////////////////////////////////////////////////////
class CounterAdder : public Entity {
public:

	////////////////////////////////////////////////////////////
	// /Create a counter adder.
	////////////////////////////////////////////////////////////
	CounterAdder(int amount, bool startsEnabled, const std::string& counterName);

	////////////////////////////////////////////////////////////
	// /add a certain amount to its counter, if going from enabled
	// /to disabled.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

private:

	int mAmount;

	std::string mCounterName;
};

#endif