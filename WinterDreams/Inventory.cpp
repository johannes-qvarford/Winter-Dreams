#include "Inventory.h"

Inventory::Inventory() {}

unsigned short Inventory::hasItem(std::string item) const{

    auto it = mInventoryMap.find(item);
    if (it!=mInventoryMap.end()){
        return it->second;
    } else {
        return 0;
    }
}

void Inventory::giveItem(std::string item, unsigned short number){
    std::map<std::string, unsigned short>::iterator it;
    it=mInventoryMap.find(item);
    if (it!=mInventoryMap.end()){
        it->second+=number;
    } else {
        mInventoryMap.insert(std::pair<std::string, unsigned short>(item, number));
    }
}

void Inventory::takeItem(std::string item, unsigned short number){
    std::map<std::string, unsigned short>::iterator it;
    it=mInventoryMap.find(item);
    if (it!=mInventoryMap.end()){
        it->second-=number;
    }
}