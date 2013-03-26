#include "RegistrationCommonHeaders.h"
#include "NPC.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {

	auto name = pt.get<std::string>("name", "");
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled",false); 

	auto path = properties.get<std::string>("path","");
	if(path == "")
		path = properties.get<std::string>("ai_path");

	auto damage = properties.get<int>("damage", 1);

	auto initialPosition = sf::FloatRect(position.x, position.y, X_STEP, -Y_STEP);

	auto npc_sp = std::shared_ptr<NPC>(new NPC(path, initialPosition, damage, !startdisabled));

	if(name != "")
		subLevel_p->mapEntityToName(name, npc_sp);

	subLevel_p->addEntity(npc_sp);
	subLevel_p->addDrawable(npc_sp, SubLevel::DRAW_WORLD);
	subLevel_p->addCollidable(npc_sp, SubLevel::SEEK_RECIEVER);
}

static ObjectTypeRegistration reg("npc", regCallback);
