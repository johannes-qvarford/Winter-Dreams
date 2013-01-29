#include "RegistrationCommonHeaders.h"
#include "Player.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	state->addPhysicalEntity(std::make_shared<Player>(position));
}

static ObjectTypeRegistration reg("collision128x64", regCallback);