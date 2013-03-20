#ifndef INCLUDED_ITEM
#define INCLUDED_ITEM

#include "GraphicalEntity.h"
#include "Animation.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>

class Item : public GraphicalEntity {
public:
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of GraphicalEntity has to define a
	// /function for drawing itself.
	////////////////////////////////////////////////////////////
	Item(sf::FloatRect position, std::string itemName, bool startEnabled);
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of GraphicalEntity has to define a
	// /function for drawing itself.
	////////////////////////////////////////////////////////////
	~Item();
	////////////////////////////////////////////////////////////
	// /Renders the item. Will render different animations depending
	// /on what animation the item was given at construction
	////////////////////////////////////////////////////////////
	void drawSelf();
	////////////////////////////////////////////////////////////
	// /Updates each frame. Does nothing by default.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
	////////////////////////////////////////////////////////////
	// /If the item collides with a player, the item will be
	// /tagged as inactive and added to the players inventory
	////////////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection);
	////////////////////////////////////////////////////////////
	// /Get the items hitbox
	////////////////////////////////////////////////////////////
	sf::FloatRect& getHitBox();
	////////////////////////////////////////////////////////////
	// /Since graphical entitys needs to be able to return it's
	// /direction, this function needs to be implemented. It 
	// /returns only returns 0,0
	////////////////////////////////////////////////////////////
	sf::Vector2i getDirection() { return sf::Vector2i(0,0); }

private:
	sf::FloatRect	mHitBox;
	std::string		mItemName;
	Animation*		mAnimation;
	std::shared_ptr<sf::SoundBuffer> mSoundBuffer;
};

#endif

