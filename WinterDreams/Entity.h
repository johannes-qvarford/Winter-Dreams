#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class GameState;

class Entity{
public:
	virtual ~Entity();
	/*
	//	Each concrete subclass of Entity has to define an update function.
	//	Update is supposed to represent the action performed by 
	//	concrete subclasses each update-loop.
	*/
	virtual void update(GameState* game) = 0;
};

#endif