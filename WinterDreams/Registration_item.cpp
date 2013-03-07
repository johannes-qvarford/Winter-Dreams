#include "RegistrationCommonHeaders.h"
#include "Item.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto properties = pt.get_child("properties");

	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto variety = properties.get<std::string>("variety");
	sf::FloatRect rect( position.x, position.y, X_STEP, -Y_STEP);
	auto item = std::shared_ptr<Item>( new Item(rect, variety, !startdisabled) );
	subLevel_p->addGraphicalEntity( item );
}

static ObjectTypeRegistration reg("item", regCallback);