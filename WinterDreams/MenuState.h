#ifndef INCLUDED_MENUSTATE
#define INCLUDED_MENUSTATE

#include "State.h"
#include <string>

/*
//	MenuState represents the main menu. 
*/
class MenuState : public State{
public:
	/*
	//	MenuStates constructor performs no actions
	*/
	MenuState(std::string filePath);
	/*
	//	MenuStates destructor performs no actions
	*/
	~MenuState();
	/*
	//	Each loop, if MenuState is the active state, update is called
	*/
	virtual void update(int milliseconds);

private:

	/*
	//	MenuState is not supposed to be copied
	*/
	MenuState(const MenuState& menu);
	/*
	//	MenuState is not supposed to be copied
	*/
	operator=(const MenuState& menu);
};

#endif