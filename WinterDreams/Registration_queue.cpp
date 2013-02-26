#include "RegistrationCommonHeaders.h"
#include "Queue.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name","");
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto queuelength = properties.get<int>("queuelength");
	//should be queueitem(s)
	auto initialqueueitem = properties.get<std::string>("initialqueueitem");

	//get list of items
	std::list<std::string> queueItems;
	splitString(initialqueueitem, &queueItems);

	auto queue_sp = std::make_shared<Queue>(queuelength, queueItems, !startdisabled);

	//does it have a name?
	if(name != "")
		subLevel_p->mapEntityToName(name, queue_sp);

	subLevel_p->addScript(queue_sp);
}

static ObjectTypeRegistration reg("queue", regCallback);
