#include "RegistrationCommonHeaders.h"
#include "LevelPortal.h"
#include <string>

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto name = pt.get<std::string>("name", "");
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto once = properties.get<bool>("once", true);
	auto height = pt.get<float>("height");
	auto width = pt.get<float>("width");
	auto levelname = properties.get<std::string>("levelname");
	auto targetportal = properties.get<std::string>("targetportal", "");
	auto exitdirection = properties.get<std::string>("exitdirection", "se");
	
	auto adjustedHeight = ( height / 32 ) * -Y_STEP;
	auto adjustedWidth = ( width / 32 ) * X_STEP;
	sf::Vector2i direction;
	if (exitdirection == "se")
		direction = sf::Vector2i(1, 0);
	else if (exitdirection == "sw")
		direction = sf::Vector2i(0, 1);
	else if (exitdirection == "ne")
		direction = sf::Vector2i(0, -1);
	else if (exitdirection == "nw")
		direction = sf::Vector2i(-1, 0);
	sf::FloatRect rect( position.x, position.y, adjustedWidth, adjustedHeight );
	
	//WARNING: ignores once
	auto portal_sp = std::make_shared<LevelPortal>(rect, subLevel_p, levelname, targetportal, !startdisabled, direction);

	subLevel_p->addEntity(portal_sp);
	subLevel_p->addDrawable(portal_sp, SubLevel::DRAW_WORLD);
	subLevel_p->addCollidable(portal_sp, SubLevel::SEEK_RECIEVER);

	if( name != "" )
		subLevel_p->mapEntityToName( name, portal_sp );
}

static ObjectTypeRegistration reg("levelportal", regCallback);
