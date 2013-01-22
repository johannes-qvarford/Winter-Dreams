#ifndef INCLUDED_ITEMDISPLAY
#define INCLUDED_ITEMDISPLAY

#include "Script.h"
class Inventory;

/*
//	ItemDisplay is a script that draws the items in an inventory.
*/
class ItemDisplay : Script {
public:

	ItemDisplay(const Inventory& inventory);

	/*
	//	ItemDisplay doesn't do anything on updates.
	*/
	void update(GameState* gameState, int milliseconds);

	/*
	//	ItemDisplay draws all items in the inventory.
	*/
	void draw() const;

private:

	Inventory* mInventory_p;
};

#endif