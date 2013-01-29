#include "RegistrationCommonHeaders.h"
#include "Wall.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	state->addPhysicalEntity(std::make_shared<Wall>(position.x, position.y, 2 * X_STEP, 2 * -Y_STEP));
}

static ObjectTypeRegistration reg("collision128x64", regCallback);