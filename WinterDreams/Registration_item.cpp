#include "RegistrationCommonHeaders.h"
#include "Item.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
		//Read the ptree for info whether it should be enabled or not
	auto startEnabled = !pt.get<bool>("startdisabled", false);
		//Create a rect to position the item
	auto itemName = pt.get<std::string>("itemname");
		//Get the items name (an item with this name will be added to the players
		//inventory when the item's picked up)
	sf::FloatRect rect( position.x, position.y, X_STEP, -Y_STEP);
	auto item = std::shared_ptr<Item>( new Item(rect, itemName, startEnabled) );
	subLevel_p->addGraphicalEntity( item );
}

static ObjectTypeRegistration reg("item", regCallback);