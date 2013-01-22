#ifndef INCLUDED_INVENTORY
#define INCLUDED_INVENTORY

#include <map>
#include <string>

class Inventory{
public:
	/*
    //  Returns the number of items you possess.
    */
	unsigned short hasItem(string item);
	/*
    //  Adds a number of item to the inventory.
    */
	void giveItem(string item, unsigned short number);
    /*
	//  Removes a number of item to the inventory.
    //  If the item hasn't been mapped, does nothing.
    */
	void takeItem(string item, unsigned short number);
private:
	map<std::string, unsigned short> mInventoryMap;
};

#endif