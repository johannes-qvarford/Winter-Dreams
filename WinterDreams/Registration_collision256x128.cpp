#include "RegistrationCommonHeaders.h"
#include "SolidZone.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto solid_sp = std::shared_ptr<CollisionZone>(new SolidZone(sf::FloatRect(position.x, position.y, 4 * X_STEP, 4 * -Y_STEP)));
	state->addCollisionZone(solid_sp);//all hitbox sizes are now inverted.
}

static ObjectTypeRegistration reg("collision256x128", regCallback);