#include "RegistrationCommonHeaders.h"
#include "LevelEnder.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name", "");
	auto width = pt.get<int>("width");
	auto height = pt.get<int>("height");
	auto startdisabled = pt.get<bool>("startdisabled", false);

	auto adjustedWidth = width * (X_STEP / 32);
	auto adjustedHeight = -1 * height * (Y_STEP / 32);
	auto box = sf::FloatRect(position.x, position.y - adjustedHeight - Y_STEP, adjustedWidth, adjustedHeight);

	auto le_sp = std::make_shared<LevelEnder>(!startdisabled, box);

	if(name != "")
		subLevel_p->mapEntityToName(name, le_sp);

	subLevel_p->addCollisionZone(le_sp);
}

static ObjectTypeRegistration reg("levelend", regCallback);
