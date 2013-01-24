#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class GameState;
////////////////////////////////////////////////////////////
// /Entity is the base class for all entitys and scripts in the entire system.
////////////////////////////////////////////////////////////
class Entity{
public:
	virtual ~Entity(){};
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of Entity has to define an update function.
	// /Update is supposed to represent the action performed by 
	// /concrete subclasses each update-loop.
	////////////////////////////////////////////////////////////
	virtual void update(GameState* gameState_p, int milliseconds) = 0;
	////////////////////////////////////////////////////////////
	// /Assigns the entitys status.
	// /The entity will be removed if it is assigned false
	////////////////////////////////////////////////////////////
	void setActive(bool active){mActive=active;}
	////////////////////////////////////////////////////////////
	// /Returns true if the entity is active
	// /Else, returns false
	////////////////////////////////////////////////////////////
	bool isActive(){return mActive;}
private:
	bool mActive = 1; //Keeps track of the entitys active-status
};

#endif