#include "RegistrationCommonHeaders.h"
#include "LevelPortal.h"
#include <string>

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	auto startEnabled = !pt.get<bool>("startdisable", false);
	auto once = pt.get<bool>("properties.once", false);
	auto height = ( pt.get<int>("height") / 32 ) * -Y_STEP;
	auto width = ( pt.get<int>("width") / 32 ) * X_STEP;
	auto targetLevel = pt.get<std::string>("properties.levelname");
	auto targetPortal = pt.get<std::string>("properties.targetportal", "");
	auto name = pt.get<std::string>("name", "");
	auto directionString = pt.get<std::string>("properties.exitdirection", "se");
	sf::Vector2i direction;
	if (directionString == "se")
		direction = sf::Vector2i(1, 0);
	else if (directionString == "sw")
		direction = sf::Vector2i(0, 1);
	else if (directionString == "ne")
		direction = sf::Vector2i(0, -1);
	else if (directionString == "nw")
		direction = sf::Vector2i(-1, 0);
	sf::FloatRect rect( position.x, position.y, width, height );
	std::shared_ptr<CollisionZone> portal_p( new LevelPortal(rect, subLevel_p, targetLevel, targetPortal, startEnabled, once, direction) );

	subLevel_p->addCollisionZone( portal_p );

	if( name != "" )
		subLevel_p->mapEntityToName( name, portal_p );
}

static ObjectTypeRegistration reg("levelportal", regCallback);
