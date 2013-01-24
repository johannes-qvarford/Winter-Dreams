#ifndef INCLUDED_GAMESTATE
#define INCLUDED_GAMESTATE

#include "State.h"
#include <memory>
#include <list>

namespace sf {
	class Texture;
}

class PlayerRelated;
class PhysicalEntity;
class Script;
/*
//	GameState is the state where the game is avalible to the player.
//	Data ought to be loaded into GameState by a LoadingState before 
//	a being assigned "current state" by the StateManager.
*/
class GameState : public State {
public:
	/*
	//	GameStates constructor does not create anything. GameStates
	//	has to be assigned PhysicalEntitys, Scripts and Textures via
	//	the functions addPhysicalEntity, addScript and addTexture.
	*/
	GameState();
	/*
	//	GameStates destructor.
	*/
	~GameState();
	/*
	//	Calls update on each entity in mEntitys,
	//	calls update on each script in mScripts
	//	then calls render.
	*/
	virtual void update(int milliseconds);
	/*
	//	Adds an Entity-pointer to mEntities.
	*/
	void addPhysicalEntity(std::shared_ptr<PhysicalEntity> physicalEntity_p);
	/*
	//	Remove an Entity-pointer to mEntities.
	*/
	void removePhysicalEntity(PhysicalEntity* physicalEntity_p);
	/*
	//	Adds a Script-pointer to mScripts.
	*/
	void addScript(std::shared_ptr<Script> script_p);
	/*
	//	Remove a Script-pointer from mScripts.
	*/
	void removeScript(Script* script_p);
	/*
	//	Adds a Texture-pointer to mForegroundTextures.
	*/
	void addForegroundTexture(std::shared_ptr<sf::Texture> texture_p);
	/*
	//	Adds a Texture-pointer to mBackgroundTextures.
	*/
	void addBackgroundTexture(std::shared_ptr<sf::Texture> texture_p);

	typedef std::list<std::shared_ptr<PhysicalEntity> > PhysicalEntities;
	typedef std::list<std::shared_ptr<Script> > Scripts;
	typedef std::list<const std::shared_ptr<sf::Texture> > Textures;

private:
	
	/*
	//	Draws background, foreground, all physical entitys and all scripts. 
	*/
	void render();
	/*
	//	Iterates over mEntitys and removes all entitys tagged as inactive and removes their
	//	associated pointers.
	//	Iterates over mScripts and removes all scripts tagged as inactive and removes their
	//	associated pointers.
	*/
	void deleteInactives();

	PhysicalEntities mPhysicalEntities;//GameStates list of Entity-pointers
	Scripts mScripts;				//GameStates list of Script-pointers
	Textures mForegroundTextures;	//GameStates list of const sf::Texture-references for the background
	Textures mBackgroundTextures;	//GameStates list of const sf::Texture-references for the foreground
	/*
	//	GameState is not supposed to be copied
	*/
	GameState(const GameState& game);
	/*
	//	GameState is not supposed to be copied
	*/
	GameState& operator=(const GameState& game);
};

#endif
