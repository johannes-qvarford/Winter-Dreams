#include "RegistrationCommonHeaders.h"
#include "LevelEnder.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name", "");

	auto adjustedWidth = pt.get<int>("width") * (X_STEP / 32);
	auto adjustedHeight = -pt.get<int>("height") * (Y_STEP / 32);
	auto startdisabled = pt.get<bool>("startdisabled", false);

	auto box = sf::FloatRect(position.x, position.y, adjustedWidth, adjustedHeight);

	auto le_sp = std::make_shared<LevelEnder>(!startdisabled, box, true);

	if(name != "")
		subLevel_p->mapEntityToName(name, le_sp);

	subLevel_p->addCollisionZone(le_sp);
}

static ObjectTypeRegistration reg("levelend", regCallback);
