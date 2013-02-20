#include "Inventory.h"
#include "PropertyManager.h"
#include <boost\property_tree\ptree.hpp>

static boost::property_tree::ptree ITEMS_PTREE;

Inventory::Inventory() {
	ITEMS_PTREE = PropertyManager::get().getObjectSettings().get_child("objects.items.animations");
	mCurrentItem = mInventoryList.end();
}

unsigned short Inventory::hasItem(std::string item) const{
	unsigned short amount = 0;
		/////////////////////////////////////////
		// Search the inventory list for elements
		// with the same name as the argument.
		//
		// If found, increase amount by 1
		/////////////////////////////////////////
    for(auto it = mInventoryList.begin(), end = mInventoryList.end(); it != end; ++it){
		if( *it == item )
			++amount;
	}

	return amount;
}

void Inventory::giveItem(std::string item, unsigned short){
		/////////////////////////////////////////
		// Add the argument item to the inventory
		// list
		/////////////////////////////////////////
	mInventoryList.push_front( item );
	
	if( getCurrentEquip() == "")
		equipNext();
}

void Inventory::takeItem(std::string item, unsigned short){
		/////////////////////////////////////////
		// Search the inventory list for an element
		// with the same name as the argument.
		//
		// If found, move the element to the back
		// of the list and pop the list.
		/////////////////////////////////////////    
	for( auto it = mInventoryList.begin(), end = mInventoryList.end(); it != end; ++it){
		if( *it == item ){
			mInventoryList.splice( end, mInventoryList, it );
			mInventoryList.pop_back();
				/////////////////////////////////////////
				// If it now is equal to end, break the loop
				/////////////////////////////////////////
			if(it == end)
				break;
		}
	}

	if( mInventoryList.size() == 0)
		mCurrentItem = mInventoryList.end();
}

void Inventory::equipNext() {
		/////////////////////////////////////////
		// If there is no items in the inventory,
		// end the function
		/////////////////////////////////////////
	if( mInventoryList.size() == 0 )
		return;

	auto tempIter = mCurrentItem;
		/////////////////////////////////////////
		// Iterate over the inventory list and search
		// for the next item which has an iconindex
		// (i.e can be equipped) then assign current
		// item to the found item.
		/////////////////////////////////////////
	do{
		if(tempIter == mInventoryList.end() )
			tempIter = mInventoryList.begin();
		
		else
			++tempIter;

		if( tempIter != mInventoryList.end() ){
			auto id = ITEMS_PTREE.get<int>(*tempIter + ".iconindex", 0);
			if( id > 0 )
				mCurrentItem = tempIter;
		}

	} while(tempIter != mCurrentItem);
}

std::string Inventory::getCurrentEquip() const{
		/////////////////////////////////////////
		// If mCurrentItem == mInventoryList.end(),
		//  return ""
		// Else if
		//	mCurrentItem.iconindex == 0
		//	return ""
		// Else
		//  return *mCurrentItem
		/////////////////////////////////////////
	return (mCurrentItem == mInventoryList.end() ) ? "" : (ITEMS_PTREE.get<int>(*mCurrentItem + ".iconindex", 0) == 0) ? "" : *mCurrentItem;
}
