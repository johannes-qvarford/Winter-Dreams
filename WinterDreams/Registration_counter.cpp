#include "RegistrationCommonHeaders.h"
#include "Counter.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	auto startEnabled = !pt.get<bool>("startdisable", false);
	auto name = pt.get<std::string>("name", "");
	auto counttrigger = pt.get<std::string>("properties.counttrigger");
	auto countamount = pt.get<int>("properties.countamount");

	auto entList = std::list<std::string>();
	splitString(counttrigger, &entList);

	auto counter_sp = std::make_shared<Counter>(entList, countamount, !startEnabled, subLevel_p);
		
	subLevel_p->addScript(counter_sp);

	if( name != "" )
		subLevel_p->mapEntityToName( name, counter_sp );
}

static ObjectTypeRegistration reg("counter", regCallback);
