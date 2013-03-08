#include "RegistrationCommonHeaders.h"
#include "TimeAdder.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto name = pt.get<std::string>("name");	
	auto& properties = pt.get_child("properties");
	auto addtime = properties.get<float>("addtime", -1);
	auto timer = properties.get<std::string>("timer");
	auto startdisabled = properties.get<bool>("startdisabled");

	auto addFrames = secsToFrames(addtime);

	auto adder_sp = std::make_shared<TimeAdder>(addFrames, timer, !startdisabled);

	if(name != "")
		subLevel_p->mapEntityToName(name, adder_sp);

	subLevel_p->addScript(adder_sp);
}

static ObjectTypeRegistration reg("timeadder", regCallback);
