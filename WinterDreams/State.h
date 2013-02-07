#ifndef INCLUDED_STATE
#define INCLUDED_STATE

/*
//	State represent a state that the game can be in. It can be updated by a higher order class.
*/
class State {
public:

	/*
	//	Virtual destructor.
	*/
	virtual ~State(){}

	/*
	//	Update the state as if a number of milliseconds have passed.
	*/
	virtual void update() = 0;

	virtual void render() = 0;

	virtual bool isRepeatable() const {return true; }
};

#endif