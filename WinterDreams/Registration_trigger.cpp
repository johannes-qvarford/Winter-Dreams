#include "RegistrationCommonHeaders.h"
#include "TriggerZone.h"
#include "GameToScreen.h"

#include <boost/foreach.hpp>

//seperate required items from required absence of items
inline void seperateRequiredItems(const std::list<std::string>& inReqItems, std::list<std::string>* absReqItems_p, std::list<std::string>* reqItems_p) {
	BOOST_FOREACH(auto req, inReqItems) {
		if(req[0] != '!')
			reqItems_p->push_back(req);
		else
			absReqItems_p->push_back(req.substr(1, req.size() - 1));
	}
}

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto name = pt.get<std::string>("name");
	
	auto adjustedWidth = pt.get<int>("width") * (X_STEP / 32);
	auto adjustedHeight = -pt.get<int>("height") * (Y_STEP / 32);

	auto box = sf::FloatRect(position.x, position.y - adjustedHeight - Y_STEP, adjustedWidth, adjustedHeight);
	
	auto properties = pt.get_child("properties");
	
	auto startdisabled = properties.get<bool>("startdisabled", false);
	auto once = properties.get<bool>("once", false);
	auto entertrigger = properties.get<std::string>("entertrigger","");
	auto exittrigger = properties.get<std::string>("exittrigger","");
	auto minlightlevel = properties.get<int>("minlightlevel", 0);
	//should be requireditem(s)
	auto requireditem = properties.get<std::string>("requireditem","");

	std::list<std::string> enterTriggerList;
	std::list<std::string> exitTriggerList;
	std::list<std::string> items;
	splitString(entertrigger, &enterTriggerList);
	splitString(exittrigger, &exitTriggerList);
	splitString(requireditem, &items);

	std::list<std::string> requiredItems;
	std::list<std::string> absenceRequiredItems;
	seperateRequiredItems(items, &absenceRequiredItems, &requiredItems);

	auto triggerZone_sp = std::shared_ptr<CollisionZone>(
		new TriggerZone(box, enterTriggerList, exitTriggerList, requiredItems, absenceRequiredItems, minlightlevel, subLevel_p, once, !startdisabled));

	if(name != "")
		subLevel_p->mapEntityToName(name, triggerZone_sp);

	subLevel_p->addCollisionZone(triggerZone_sp);
}

static ObjectTypeRegistration reg("trigger", regCallback);
