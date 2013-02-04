#ifndef INCLUDED_INVENTORYDISPLAY
#define INCLUDED_INVENTORYDISPLAY

#include "Script.h"
#include "Animation.h"
#include <memory>
#include <string>
#include <map>

class Player;
////////////////////////////////////////////////////////////
// /This class handles the drawing of the inventory box 
// /that displays what the player currently has equiped.
////////////////////////////////////////////////////////////
class InventoryDisplay : public Script {
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
	void draw() const;

	void update(GameState* gameState_p);
private:
	float mXPos;
	float mYPos;

	std::weak_ptr<Player>			 mPlayer_wp;			//A pointer to the player
	Animation*						 mBoxAnimation;			//Animates the inventory box
	Animation*						 mCurrentItemAnimation; //Holds a pointer to animation of the item the player currently has equiped. Can be NULL
	std::map<std::string, Animation> mAnimationMap;			//Holds all the different animations for the inventory
	

	//No copies
	InventoryDisplay(const InventoryDisplay& i);
	//No copies
	InventoryDisplay& operator=(const InventoryDisplay& i);
};

#endif