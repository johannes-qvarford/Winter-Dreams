#ifndef INCLUDED_GAMESTATE
#define INCLUDED_GAMESTATE

#include "State.h"
#include <memory>
#include <list>

#include <SFML/System/Vector2.hpp>

namespace sf {
	class Texture;
}

class GraphicalEntity;
class PhysicalEntity;
class CollisionZone;
class Script;

////////////////////////////////////////////////////////////
// /GameState is the state where the game is avalible to the player.
// /Data ought to be loaded into GameState by a LoadingState before 
// /a being assigned "current state" by the StateManager.
////////////////////////////////////////////////////////////
class GameState : public State {
public:

	////////////////////////////////////////////////////////////
	// /GameStates constructor does not create anything. GameStates
	// /has to be assigned GraphicalEntitys, Scripts, CollisionZones and Textures via
	// /the functions addGraphicalEntity, addCollisionZones,  addScript, setBackgroundTexture
	// /and setMapTexture.
	////////////////////////////////////////////////////////////
	GameState();

	////////////////////////////////////////////////////////////
	//	GameStates destructor.
	////////////////////////////////////////////////////////////
	~GameState();
	
	////////////////////////////////////////////////////////////
	// /Call update on each entity. Checks collisions between
	// /GraphicalEntities and CollissionZones, and then render
	// /all GraphicalEntities and Scripts.
	////////////////////////////////////////////////////////////
	void update();
	
	////////////////////////////////////////////////////////////
	// /Add a graphical entity to the state. 
	////////////////////////////////////////////////////////////
	void addGraphicalEntity(std::shared_ptr<GraphicalEntity> graphicalEntity_sp);
	
	////////////////////////////////////////////////////////////
	// /Add a collision zone to the state. 
	////////////////////////////////////////////////////////////
	void addCollisionZone(std::shared_ptr<CollisionZone> graphicalEntity_sp);

	////////////////////////////////////////////////////////////
	// /Add a script to the state.
	////////////////////////////////////////////////////////////
	void addScript(std::shared_ptr<Script> script_sp);
	
	////////////////////////////////////////////////////////////
	// /Set the map texture at a position in screencoordinates.
	////////////////////////////////////////////////////////////
	void setMapTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position);
	
	////////////////////////////////////////////////////////////
	// /Set the background texture at a position in screencoordinates.
	////////////////////////////////////////////////////////////
	void setBackgroundTexture(std::shared_ptr<sf::Texture> texture_sp, const sf::Vector2f& position);

	typedef std::list<std::shared_ptr<GraphicalEntity> > GraphicalEntities;
	typedef std::list<std::shared_ptr<Script> > Scripts;
	typedef std::list<std::shared_ptr<CollisionZone> > CollisionZones;
	typedef std::pair<std::shared_ptr<sf::Texture>, sf::Vector2f> PositionedTexture;

private:
	
	////////////////////////////////////////////////////////////
	// /Check collisions between a GraphicalEntity
	// /and the other PhysicalEntities. 
	////////////////////////////////////////////////////////////
	void checkCollisions(std::shared_ptr<GraphicalEntity> graphical_sp);

	////////////////////////////////////////////////////////////
	// /Draw background, map, all graphical entities and all scripts. 
	////////////////////////////////////////////////////////////
	void render();

	////////////////////////////////////////////////////////////
	// /Remove all entities that are not active anymore.
	////////////////////////////////////////////////////////////
	void deleteInactives();

	CollisionZones mCollisionZones;

	GraphicalEntities mGraphicalEntities;

	Scripts mScripts;			

	PositionedTexture mMapTexture;

	PositionedTexture mBackgroundTexture;

	sf::View mView;

	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	GameState(const GameState& game);

	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	GameState& operator=(const GameState& game);
};

#endif
