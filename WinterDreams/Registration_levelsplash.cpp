#include "RegistrationCommonHeaders.h"
#include "LevelSplash.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");

	auto lifeTime = properties.get<float>("lifetime") * 60;
	auto fadeOutTime = properties.get<float>("fadeouttime") * 60;
	auto imgName = properties.get<std::string>("splashimgfile");

	auto splash = std::shared_ptr<LevelSplash>( new LevelSplash( imgName, static_cast<int>(lifeTime), static_cast<int>(fadeOutTime) ) );

	subLevel_p->addScript( splash );
}

static ObjectTypeRegistration reg("levelsplash", regCallback);