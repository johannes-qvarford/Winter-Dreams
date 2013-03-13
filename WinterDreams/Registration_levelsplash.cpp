#include "RegistrationCommonHeaders.h"
#include "LevelSplash.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");

	auto lifetime = properties.get<float>("lifetime", 2.0);
	auto fadeouttime = properties.get<float>("fadeouttime", 2.0);
	auto splashimgfile = properties.get<std::string>("splashimgfile");
	auto startdisabled = properties.get<bool>("startdisabled", false);

	auto secLifeTime = int(lifetime * 60);
	auto secFadeOutTime = int(fadeouttime * 60);

	auto splash = std::shared_ptr<LevelSplash>( new LevelSplash(splashimgfile, secLifeTime, secFadeOutTime, !startdisabled) );

	subLevel_p->addScript( splash );
}

static ObjectTypeRegistration reg("levelsplash", regCallback);