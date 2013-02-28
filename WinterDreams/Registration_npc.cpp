#include "RegistrationCommonHeaders.h"
#include "NPC.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {

	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled",false); 
	auto ai_path = properties.get<std::string>("ai_path");
	auto initialPosition = sf::FloatRect(position.x, position.y, X_STEP, -Y_STEP);

	subLevel_p->addGraphicalEntity(std::shared_ptr<GraphicalEntity>(new NPC(ai_path, initialPosition, !startdisabled))); 
}

static ObjectTypeRegistration reg("npc", regCallback);
