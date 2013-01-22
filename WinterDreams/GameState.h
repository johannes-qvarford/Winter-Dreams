#ifndef INCLUDED_GAMESTATE
#define INCLUDED_GAMESTATE

#include "State.h"
#include <SFML\Graphics.hpp>
#include <list>

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
	//	GameStates destructor calls clearsEntitys,
	//	clearScripts and clearTextures.
	*/
	~GameState();
	/*
	//	Calls update on each entity in mEntitys,
	//	calls update on each script in mScripts
	//	then calls render.
	*/
	virtual void update();
	/*
	//	Adds an Entity-pointer to mEntitys.
	*/
	void addPhysicalEntity(PhysicalEntity* physicalEntity_p);
	/*
	//	Adds a Script-pointer to mScripts.
	*/
	void addScript(Script* script_p);
	/*
	//	Adds a const sf::Texture-reference to mForegroundTexture.
	//	GameManager HAVE NO responsibility over the texture.
	*/
	void addForegroundTexture(const sf::Texture& texture);
	/*
	//	Adds a const sf::Texture-reference to mBackgroundTextures.
	//	GameManager HAVE NO responsibility over the texture.
	*/
	void addBackgroundTexture(const sf::Texture& texture);
private:
	typedef std::list<PhysicalEntity*> PhysicalEntitys;
	typedef std::list<Script*> Scripts;
	typedef std::list<const sf::Texture&> Textures;
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
	/*
	//	Deletes all entitys stored in mEntitys and empties the list of pointers.
	*/
	void clearEntitys();
	/*
	// Deletes all entitys stored in mScripts and empies the list of pointers.
	*/
	void clearScripts();
	/*
	//	Empties mTextures of references. 
	//	Does NOT delete the textures.
	*/
	void clearTextures();

	PhysicalEntitys mPhysicalEntitys;//GameStates list of Entity-pointers
	Scripts mScripts;				//GameStates list of Script-pointers
	Textures mForegroundTextures;	//GameStates list of const sf::Texture-references for the background
	Textures mBackgroundTextures;	//GameStates list of const sf::Texture-references for the foreground
	/*
	//	GameState is not supposed to be copied
	*/
	GameState(const GameState& game);
};

#endif
