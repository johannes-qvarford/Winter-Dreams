#include "RegistrationCommonHeaders.h"
#include "TriggerZone.h"

#include "GameToScreen.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto adjustedWidth = pt.get<int>("width") * (X_STEP / 32);
	auto adjustedHeight = pt.get<int>("height") * (Y_STEP / 32);
	
	auto box = sf::FloatRect(position.x + adjustedWidth, position.y + adjustedHeight, adjustedWidth, -adjustedHeight);
	
	auto properties = pt.get_child("properties");
	
	auto startdisabled = properties.get<bool>("startdisabled", true);
	auto once = properties.get<bool>("once", false);
	auto entertrigger = properties.get<std::string>("entertrigger","");
	auto exittrigger = properties.get<std::string>("exittrigger","");
	auto minlightlevel = properties.get<int>("minlightlevel", 0);

	state->addCollisionZone(std::shared_ptr<CollisionZone>(
		new TriggerZone(box, entertrigger, exittrigger, minlightlevel, once, !startdisabled)));
}

static ObjectTypeRegistration reg("trigger", regCallback);