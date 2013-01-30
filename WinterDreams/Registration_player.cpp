#include "RegistrationCommonHeaders.h"
#include "Player.h"
#include "Camera.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto player = std::shared_ptr<PhysicalEntity>(new Player(position));
	auto camera = std::shared_ptr<Script>(new Camera(player));
	state->addPhysicalEntity(player);
	state->addScript(camera);
}

static ObjectTypeRegistration reg("player", regCallback);