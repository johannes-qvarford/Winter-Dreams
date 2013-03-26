#ifndef INCLUDED_INVENTORYDISPLAY
#define INCLUDED_INVENTORYDISPLAY

#include "Entity.h"
#include "Drawable.h"
#include "BaseHitBoxHaveable.h"

#include "Animation.h"
#include <memory>
#include <string>
#include <map>

class Player;
////////////////////////////////////////////////////////////
// /This class handles the drawing of the inventory box 
// /that displays what the player currently has equiped.
////////////////////////////////////////////////////////////
class InventoryDisplay : public Entity, public Drawable, public BaseHitBoxHaveable {
public:
	////////////////////////////////////////////////////////////
	// /The instance will need a shared pointer to know where to
	// /find the player.
	// /It will only store a weak pointer to the player.
	////////////////////////////////////////////////////////////
	InventoryDisplay(std::weak_ptr<Player> player);
	////////////////////////////////////////////////////////////
	// /Destructor. No dynamicly allocated membervariables.
	////////////////////////////////////////////////////////////
	~InventoryDisplay();
	////////////////////////////////////////////////////////////
	// /Draws the inventory box as well as the icon representing
	// /the item the player currently have equipped.
	////////////////////////////////////////////////////////////
	void draw();

	void update(SubLevel* subLevel_p);
private:
	sf::Vector2f initPos;

	std::weak_ptr<Player>			 mPlayer_wp;			//A pointer to the player
	Animation*						 mBoxAnimation_p;		//Animates the inventory box
	Animation*						 mFrameAnimation_p;		//Animates the frame around the items
	std::map<std::string, Animation> mAnimationMap;			//Holds all the different animations for the inventory
	std::list<sf::Sprite>			 mItemSpriteList;
	void updateUI();

	//No copies
	InventoryDisplay(const InventoryDisplay& i);
	//No copies
	InventoryDisplay& operator=(const InventoryDisplay& i);
};

#endif