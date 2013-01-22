#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class GameState;
/*
//	Entity is the base class for all entitys and scripts in the entire system.
*/
class Entity{
public:
	virtual ~Entity();
	/*
	//	Each concrete subclass of Entity has to define an update function.
	//	Update is supposed to represent the action performed by 
	//	concrete subclasses each update-loop.
	*/
	virtual void update(GameState* gameState_p) = 0;
};

#endif