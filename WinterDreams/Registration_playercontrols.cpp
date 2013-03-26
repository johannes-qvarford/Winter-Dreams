#include "RegistrationCommonHeaders.h"
#include "InputManager.h"
#include "PlayerControls.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto name = pt.get<std::string>("name");
	auto& properties = pt.get_child("properties");
	auto startdisabled = properties.get<bool>("startdisabled", false);

	auto controls_sp = std::make_shared<PlayerControls>(!startdisabled);
	//subLevel_p->addEntity(controls_sp);
}

static ObjectTypeRegistration reg("playercontrols", regCallback);