#include "RegistrationCommonHeaders.h"

#include "LightPoint.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto brightness = properties.get<int>("brightness");
	auto once = properties.get<bool>("once", false);
	auto initialPosition = sf::FloatRect(position.x, position.y, 0, 0);

	auto light_sp = std::shared_ptr<LightPoint>(new LightPoint(initialPosition, brightness, once, !startdisabled));
	state->addGraphicalEntity(light_sp);//all hitbox sizes are now inverted.
}

static ObjectTypeRegistration reg("light", regCallback);