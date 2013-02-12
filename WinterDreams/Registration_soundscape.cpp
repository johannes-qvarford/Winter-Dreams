#include "RegistrationCommonHeaders.h"
#include "SoundScape.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto innerradius = properties.get<float>("innerradius");
	auto rangedecay = properties.get<int>("rangedecay");
	auto soundfile = properties.get<std::string>("soundfile");
	auto volume = properties.get<float>("volume");
	auto loop = properties.get<bool>("loop");
	auto soundtype = properties.get<std::string>("soundtype");
	auto name = pt.get<std::string>("name");
	auto collisionBox = sf::FloatRect(position.x, position.y, -1, 1);
	auto soundScape_sp = std::shared_ptr<CollisionZone>(new SoundScape(collisionBox, innerradius, rangedecay, volume, loop, soundfile, !startdisabled, soundtype));
	subLevel_p->mapEntityToName(name, soundScape_sp);
	subLevel_p->addCollisionZone(soundScape_sp);
}

static ObjectTypeRegistration reg("soundscape", regCallback);
