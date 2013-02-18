#include "RegistrationCommonHeaders.h"
#include "QueueItem.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");

	auto& name = pt.get<std::string>("name");
	//TODO: change true to false
	auto startdisabled = properties.get<bool>("startdisabled", true);
	auto& queue = properties.get<std::string>("queue");
	//should be entit(ies)
	auto& entity = properties.get<std::string>("entity");

	std::list<std::string> entList;
	splitString(entity, &entList);

	auto queueItem_sp = std::make_shared<QueueItem>(queue, entList, !startdisabled);

	if(name != "")
		subLevel_p->mapEntityToName(name, queueItem_sp);

	subLevel_p->addScript(queueItem_sp);
}

static ObjectTypeRegistration reg("queueitem", regCallback);