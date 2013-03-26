#ifndef INCLUDED_ITEM
#define INCLUDED_ITEM

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

#include "Animation.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>

class Item : public Entity, public Drawable, public Collidable, public BaseHitBoxHaveable {
public:
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of GraphicalEntity has to define a
	// /function for drawing itself.
	////////////////////////////////////////////////////////////
	Item(const sf::FloatRect& position, const std::string& itemName, bool startEnabled);
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of GraphicalEntity has to define a
	// /function for drawing itself.
	////////////////////////////////////////////////////////////
	~Item();
	////////////////////////////////////////////////////////////
	// /Renders the item. Will render different animations depending
	// /on what animation the item was given at construction
	////////////////////////////////////////////////////////////
	void draw();
	////////////////////////////////////////////////////////////
	// /Updates each frame. Does nothing by default.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
	////////////////////////////////////////////////////////////
	// /If the item collides with a player, the item will be
	// /tagged as inactive and added to the players inventory
	////////////////////////////////////////////////////////////
	void onCollision(Collidable* col_p, const sf::Rect<float>& intersection);

private:
	std::string		mItemName;
	Animation*		mAnimation;
	std::shared_ptr<sf::SoundBuffer> mSoundBuffer;
};

#endif

