#ifndef INCLUDED_INVENTORY
#define INCLUDED_INVENTORY

#include <map>
#include <string>

class Inventory{
public:
	//Returns the number of items you possess
	unsigned short hasItem(string item);
	//Adds number of an item
	void giveItem(string item, unsigned short number);
	//Removes number of an item
	void takeItem(string item, unsigned short number);
private:
	map<std::string, unsigned short> mInventoryMap;
};

#endif