#include "RegistrationCommonHeaders.h"
#include "GameToScreen.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name");
	auto& polyline = pt.get_child("polyline");

	std::vector<sf::Vector2f> path;
	
	//iterate over list
	for(auto it = polyline.begin(), end = polyline.end(); it != end; ++it) {
		auto& pos = it->second;
		sf::Vector2f vec;
		//get position
		vec.x = pos.get<int>("x") * X_STEP / 32;
		vec.y = pos.get<int>("y") * Y_STEP / 32;
		vec += position;
		path.push_back(vec);
	}

	subLevel_p->mapAiPathToName(name, path);
}

static ObjectTypeRegistration reg("ai_path", regCallback);
