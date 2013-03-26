#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class SubLevel;
////////////////////////////////////////////////////////////
// /Entity is the base class for all entitys and scripts in the entire system.
////////////////////////////////////////////////////////////
class Entity {
public:
	Entity (bool startEnabled = true) :
		mIsEnabled( startEnabled ), 
		mIsAlive( true )
	{}
	virtual ~Entity() {}
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of Entity has to define an update function.
	// /Update is supposed to represent the action performed by 
	// /concrete subclasses each update-loop.
	////////////////////////////////////////////////////////////
	virtual void update(SubLevel* subLevel_p) = 0;
	////////////////////////////////////////////////////////////
	// /Assigns the entitys life status.
	// /The entity will be removed if it is assigned false
	////////////////////////////////////////////////////////////
	void setAlive(bool alive){mIsAlive = alive; }
	////////////////////////////////////////////////////////////
	// /Returns true if the entity is alive
	// /Else, returns false
	////////////////////////////////////////////////////////////
	bool getAlive() const { return mIsAlive; } 
	////////////////////////////////////////////////////////////
	// /Assigns whether the entity is enabled or not
	////////////////////////////////////////////////////////////
	void setEnabled(bool enabled) 
	{ setEnabled_impl(enabled); onChangeEnabled(mIsEnabled, enabled); }
	////////////////////////////////////////////////////////////
	// /Returns true if the entity is enabled
	////////////////////////////////////////////////////////////
	bool getEnabled() const { return mIsEnabled; }
	////////////////////////////////////////////////////////////
	// /Toggles the entitys enabled-status
	////////////////////////////////////////////////////////////
	void swapEnabled() 
	{  swapEnabled_impl(); onChangeEnabled(mIsEnabled, !mIsEnabled); }

	////////////////////////////////////////////////////////////
	// /This function is called whenever swapEnabled, or setEnabled is called.
	// /It is not called when swapEnabled_impl or setEnabled_impl is called.
	////////////////////////////////////////////////////////////
	virtual void onChangeEnabled(bool previous, bool current){};

protected:

	////////////////////////////////////////////////////////////
	// /SetEnabled_impl and swapEnabled_impl call onChangeEnabled.
	// /Use this if you want to change entities own state in its
	// /onChangeEnabled call(thus preventing infinite recursion).
	////////////////////////////////////////////////////////////
	void setEnabled_impl(bool enabled){ mIsEnabled = enabled; }
	void swapEnabled_impl(){ mIsEnabled = !mIsEnabled; }

private:
	bool mIsAlive;	 //Keeps track of whether the entity is alive or not
	bool mIsEnabled; //Keeps track of whether the entity is enabled or not
	
};

#endif