#include "RegistrationCommonHeaders.h"
#include "Wall.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	state->addPhysicalEntity(std::make_shared<Wall>(position.x, position.y, X_STEP, -Y_STEP));//All hitbox heights are now inverted. Ask Johannes.
}
	
static ObjectTypeRegistration reg("collision64x32", regCallback);