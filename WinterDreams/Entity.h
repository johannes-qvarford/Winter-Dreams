#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class GameState;
////////////////////////////////////////////////////////////
// /Entity is the base class for all entitys and scripts in the entire system.
////////////////////////////////////////////////////////////
class Entity {
public:
	Entity ( bool startEnabled = true ) :
		mIsEnabled	( startEnabled ), 
		mIsAlive	( true )
	{	}
	virtual ~Entity() {	}
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of Entity has to define an update function.
	// /Update is supposed to represent the action performed by 
	// /concrete subclasses each update-loop.
	////////////////////////////////////////////////////////////
	virtual void update(GameState* gameState_p) = 0;
	////////////////////////////////////////////////////////////
	// /Assigns the entitys life status.
	// /The entity will be removed if it is assigned false
	////////////////////////////////////////////////////////////
	void setAlive( bool alive ){mIsAlive = alive; }
	////////////////////////////////////////////////////////////
	// /Returns true if the entity is alive
	// /Else, returns false
	////////////////////////////////////////////////////////////
	bool getAlive() const { return mIsAlive; } 
	////////////////////////////////////////////////////////////
	// /Assigns whether the entity is enabled or not
	////////////////////////////////////////////////////////////
	void setEnabled( bool enabled ) { mIsEnabled = enabled; }
	////////////////////////////////////////////////////////////
	// /Returns true if the entity is enabled
	////////////////////////////////////////////////////////////
	bool getEnabled() const { return mIsEnabled; }
	////////////////////////////////////////////////////////////
	// /Toggles the entitys enabled-status
	////////////////////////////////////////////////////////////
	void swapEnabled() { mIsEnabled = !mIsEnabled; }
private:
	bool mIsAlive;	 //Keeps track of whether the entity is alive or not
	bool mIsEnabled; //Keeps track of whether the entity is enabled or not
	
};

#endif