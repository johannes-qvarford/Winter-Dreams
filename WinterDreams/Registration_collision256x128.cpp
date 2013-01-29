#include "RegistrationCommonHeaders.h"
#include "Wall.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	state->addPhysicalEntity(std::make_shared<Wall>(position.x, position.y, 4 * X_STEP, 4 * -Y_STEP));//all hitbox heights are now inverted. Ask Johannes
}

static ObjectTypeRegistration reg("collision256x128", regCallback);