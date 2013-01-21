#ifndef INCLUDED_MENUSTATE
#define INCLUDED_MENUSTATE

#include "State.h"
#include <list>

class MenuState : public State{
public:
	typedef std::list<State*> States;

	MenuState(const States);
	MenuState(const MenuState& menu)
	virtual ~MenuState();

	virtual void update();
	State* getNewGameState();
	State* getLoadGameState(const unsigned int loadSlot);

private:
	States mStates;

};

#endif