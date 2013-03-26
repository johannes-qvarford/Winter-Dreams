#ifndef INCLUDED_SUBLEVEL
#define INCLUDED_SUBLEVEL

#include <memory>
#include <list>
#include <vector>

namespace sf {
	class Texture;
}

class Drawable;
class Collidable;
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
	
	enum DrawableType {
		DRAW_WORLD,
		DRAW_SCREEN
	};
	////////////////////////////////////////////////////////////
	// /Add a drawable to the sub level.
	// /If its a world drawable, sort according to its layer and position.
	// /If not, draw directly on screen, no matter the order.
	////////////////////////////////////////////////////////////
	void addDrawable(std::shared_ptr<Drawable> draw_sp, DrawableType type);
	
	enum CollidableType {
		SEEK_SEEKER,
		SEEK_RECIEVER
	};
	////////////////////////////////////////////////////////////
	// /Add a collidable to the sub level.
	// /If its a seeker, collisions against recivers will be checked every frame.
	// /If not, it will collide with seekers.
	// /NOTE: A seeker is not automaticly a reciever.
	////////////////////////////////////////////////////////////
	void addCollidable(std::shared_ptr<Collidable> col_sp, CollidableType type);

	////////////////////////////////////////////////////////////
	// /Add an entity to the sub level.
	////////////////////////////////////////////////////////////
	void addEntity(std::shared_ptr<Entity> ent_sp);
	
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

	////////////////////////////////////////////////////////////
	// /Adds a light to the light spot shader. 
	// /
	// /The lightID tells the shader which light to manipulate.
	// /xPos is the lightsource's x-position, in world coordinates.
	// /yPos is the lightsource's y-position, in world coordinates.
	// /brightness regulates the circle that is fully lit.
	// /maxDis regulates the total circle size
	////////////////////////////////////////////////////////////
	void setLightPoint(const int& lightID,  const sf::Vector2f& position, const float& brightness, const float& maxDis);

private:

	
	typedef std::list<std::shared_ptr<Drawable> > Drawables;
	typedef std::list<std::shared_ptr<Collidable> > Collidables;
	typedef std::list<std::shared_ptr<Entity> > Entities;
	typedef std::map<std::string, std::weak_ptr<Entity> > EntityMap;
	typedef std::map<std::string, std::vector<sf::Vector2f> > AiPathMap;
	typedef std::pair<std::shared_ptr<sf::Texture>, sf::Vector2f> PositionedTexture;
	

private:
	
	////////////////////////////////////////////////////////////
	// /Check collisions between a Collidable(must be a seeker)
	// /and the other Collidables. 
	////////////////////////////////////////////////////////////
	void checkCollisions(std::shared_ptr<Collidable> col_sp);

	////////////////////////////////////////////////////////////
	// /Remove all entities that are not active anymore.
	////////////////////////////////////////////////////////////
	void deleteInactives();

	LevelState* mLevelState_p;

	EntityMap mNameToEntity;

	AiPathMap mNameToAiPath;

	Entities mEntities;

	Drawables mWorldDrawables;

	Drawables mScreenDrawables;
	
	Collidables mSeekers;

	Collidables mRecievers;

	PositionedTexture mMapTexture;

	PositionedTexture mBackgroundTexture;

	sf::View mView;

	std::shared_ptr<sf::Shader> mLightCircleShader;

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