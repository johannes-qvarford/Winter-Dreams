#ifndef INCLUDED_GAMESTATE
#define INCLUDED_GAMESTATE

#include "State.h"
#include <memory>
#include <list>

#include <SFML/System/Vector2.hpp>

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
	void update(int milliseconds);
	
	/*
	//	Adds an Entity-pointer to mEntities.
	*/
	void addPhysicalEntity(std::shared_ptr<PhysicalEntity> physicalEntity_p);
	
	/*
	//	Adds a Script-pointer to mScripts.
	*/
	void addScript(std::shared_ptr<Script> script_p);
	
	/*
	//	Adds the map texture.
	*/
	void setMapTexture(std::shared_ptr<sf::Texture> texture_p, const sf::Vector2f& position);
	
	/*
	//	Adds the background texture.
	*/
	void setBackgroundTexture(std::shared_ptr<sf::Texture> texture_p, const sf::Vector2f& position);


	typedef std::list<std::shared_ptr<PhysicalEntity> > PhysicalEntities;
	typedef std::list<std::shared_ptr<Script> > Scripts;
	typedef std::pair<std::shared_ptr<sf::Texture>, sf::Vector2f> PositionedTexture;

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

	PhysicalEntities mPhysicalEntities;	//GameStates list of Entity-pointers
	Scripts mScripts;					//GameStates list of Script-pointers
	PositionedTexture mMapTexture;	//GameStates map texture + position
	PositionedTexture mBackgroundTexture;	//GameStates background texture + position
	sf::View mView;

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
