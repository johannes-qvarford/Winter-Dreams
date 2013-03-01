#include "RegistrationCommonHeaders.h"
#include "Timer.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name");
	auto startdisabled = properties.get<bool>("startdisabled");
	auto time_ = properties.get<float>("time");
	auto alarmtrigger = properties.get<std::string>("alarmtrigger");
	auto alarmaction = properties.get<std::string>("action");
	auto maxtime = properties.get<float>("maxtime");

	auto alarmFrames = secsToFrames(time_);
	auto maxFrames = secsToFrames(maxtime);
	auto entList = std::list<std::string>();
	splitString(alarmtrigger, &entList);

	auto action = 
		alarmaction == "swap"		?	Timer::ACTION_SWAP :
		alarmaction == "disable"	?	Timer::ACTION_DISABLE :
										Timer::ACTION_ENABLE;

	auto timer_sp = std::make_shared<Timer>(maxFrames, alarmFrames, entList, action, !startdisabled);

	if(name != "")
		subLevel_p->mapEntityToName(name, timer_sp);

	subLevel_p->addScript(timer_sp);
}

static ObjectTypeRegistration reg("timer", regCallback);
