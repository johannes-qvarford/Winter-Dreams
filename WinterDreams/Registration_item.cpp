#include "RegistrationCommonHeaders.h"
#include "Item.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto properties = pt.get_child("properties");

	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto variety = properties.get<std::string>("variety");
	sf::FloatRect rect( position.x, position.y, X_STEP, -Y_STEP);
	auto item_sp = std::shared_ptr<Item>( new Item(rect, variety, !startdisabled) );
	
	subLevel_p->addEntity(item_sp);
	subLevel_p->addDrawable(item_sp, SubLevel::DRAW_WORLD);
	subLevel_p->addCollidable(item_sp, SubLevel::SEEK_RECIEVER);
}

static ObjectTypeRegistration reg("item", regCallback);