#include "RegistrationCommonHeaders.h"
#include "SoundScape.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto name = pt.get<std::string>("name", "");
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", true);
	auto innerradius = properties.get<float>("innerradius", 1);
	auto rangedecay = properties.get<int>("rangedecay", 10);
	auto soundfile = properties.get<std::string>("soundfile");
	auto volume = properties.get<float>("volume", 100);
	auto loop = properties.get<bool>("loop", false);
	auto threeD = properties.get<bool>("sound3d", false);
	auto fadein = properties.get<int>("fadein", 0);
	auto soundtype = properties.get<std::string>("soundtype", "sound");

	auto collisionBox = sf::FloatRect(position.x, position.y, -1, 1);
	auto soundScape_sp = std::shared_ptr<CollisionZone>(new SoundScape(collisionBox, innerradius, rangedecay, volume, loop, soundfile, !startdisabled, soundtype, fadein, threeD, subLevel_p));
	
	if(name != "")
		subLevel_p->mapEntityToName(name, soundScape_sp);
	
	subLevel_p->addCollisionZone(soundScape_sp);
}

static ObjectTypeRegistration reg("soundscape", regCallback);
