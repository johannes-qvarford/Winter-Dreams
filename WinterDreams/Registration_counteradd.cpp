#include "RegistrationCommonHeaders.h"
#include "CounterAdder.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	auto name = pt.get<std::string>("name", "");
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto addcount = properties.get<int>("addcount", 1);
	auto counter = properties.get<std::string>("counter");

	auto counteradd_sp = std::make_shared<CounterAdder>(addcount, !startdisabled, counter);
		
	subLevel_p->addScript(counteradd_sp);

	if( name != "" )
		subLevel_p->mapEntityToName( name, counteradd_sp );
}

static ObjectTypeRegistration reg("counter", regCallback);
