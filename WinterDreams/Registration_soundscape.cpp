#include "RegistrationCommonHeaders.h"
#include "SoundScape.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto innerradius = properties.get<float>("innerradius");
	auto rangedecay = properties.get<float>("rangedecay");
	auto soundfile = properties.get<std::string>("soundfile");
	auto volume = properties.get<float>("volume");
	auto loop = properties.get<bool>("loop");

	auto collisionBox = sf::FloatRect(position.x, position.y, -1, 1);

	state->addCollisionZone(std::shared_ptr<CollisionZone>(new SoundScape(collisionBox, innerradius, rangedecay, volume, loop, soundfile, !startdisabled)));

}

static ObjectTypeRegistration reg("soundscape", regCallback);
