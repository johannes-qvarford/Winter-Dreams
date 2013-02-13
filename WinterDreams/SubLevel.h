#ifndef INCLUDED_SUBLEVEL
#define INCLUDED_SUBLEVEL

#include <memory>
#include <list>
#include <vector>

namespace sf {
	class Texture;
}

class GraphicalEntity;
class PhysicalEntity;
class CollisionZone;
class Script;
class AiPath;
class Entity;
class LevelState;

////////////////////////////////////////////////////////////
// /SubLevel represent a part of a level.
// /It contains a player and a camera, but its entities doesn't
// /ask the SubLevel for them. Instead, they ask its owner(its LevelState)
////////////////////////////////////////////////////////////
class SubLevel {
public:

	////////////////////////////////////////////////////////////
	// /Constructor.
	////////////////////////////////////////////////////////////
	SubLevel(LevelState* levelState_p);

	////////////////////////////////////////////////////////////
	// /Destructor.
	////////////////////////////////////////////////////////////
	~SubLevel();


	////////////////////////////////////////////////////////////
	// /Get the SubLevels parent.
	////////////////////////////////////////////////////////////
	LevelState* getLevel();
		
	////////////////////////////////////////////////////////////
	// /Call update on each entity. Checks collisions between
	// /GraphicalEntities and CollissionZones, and then render
	// /all GraphicalEntities and Scripts.
	////////////////////////////////////////////////////////////
	void update();

	////////////////////////////////////////////////////////////
	// /Draw background, map, all graphical entities and all scripts. 
	////////////////////////////////////////////////////////////
	void render();
	
	////////////////////////////////////////////////////////////
	// /Add a graphical entity to the sub level. 
	////////////////////////////////////////////////////////////
	void addGraphicalEntity(std::shared_ptr<GraphicalEntity> graphicalEntity_sp);
	
	////////////////////////////////////////////////////////////
	// /Add a collision zone to the sub level. 
	////////////////////////////////////////////////////////////
	void addCollisionZone(std::shared_ptr<CollisionZone> graphicalEntity_sp);

	////////////////////////////////////////////////////////////
	// /Add a script to the sub level.
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

	////////////////////////////////////////////////////////////
	// /Map a name to an entity.
	////////////////////////////////////////////////////////////
	void mapEntityToName(const std::string& name, std::weak_ptr<Entity> entity_wp);

	////////////////////////////////////////////////////////////
	// /Map a name to a path.
	////////////////////////////////////////////////////////////
	void mapAiPathToName(const std::string& name, const std::vector<sf::Vector2f>& path);

	////////////////////////////////////////////////////////////
	// /Get entity mapped to a name.
	////////////////////////////////////////////////////////////
	std::weak_ptr<Entity> getEntity(const std::string& name);

	////////////////////////////////////////////////////////////
	// /Get path mapped to a name.
	////////////////////////////////////////////////////////////
	const std::vector<sf::Vector2f>& getAiPath(const std::string& name);

private:

	
	typedef std::list<std::shared_ptr<GraphicalEntity> > GraphicalEntities;
	typedef std::list<std::shared_ptr<Script> > Scripts;
	typedef std::list<std::shared_ptr<CollisionZone> > CollisionZones;
	typedef std::map<std::string, std::weak_ptr<Entity> > EntityMap;
	typedef std::map<std::string, std::vector<sf::Vector2f> > AiPathMap;
	typedef std::pair<std::shared_ptr<sf::Texture>, sf::Vector2f> PositionedTexture;
	

private:
	
	////////////////////////////////////////////////////////////
	// /Check collisions between a GraphicalEntity
	// /and the other PhysicalEntities. 
	////////////////////////////////////////////////////////////
	void checkCollisions(std::shared_ptr<GraphicalEntity> graphical_sp);

	////////////////////////////////////////////////////////////
	// /Remove all entities that are not active anymore.
	////////////////////////////////////////////////////////////
	void deleteInactives();

	LevelState* mLevelState_p;

	EntityMap mNameToEntity;

	AiPathMap mNameToAiPath;

	CollisionZones mCollisionZones;

	GraphicalEntities mGraphicalEntities;

	Scripts mScripts;			

	PositionedTexture mMapTexture;

	PositionedTexture mBackgroundTexture;

	sf::View mView;

	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	SubLevel(const SubLevel& game);

	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	SubLevel& operator=(const SubLevel& game);
};

#endif