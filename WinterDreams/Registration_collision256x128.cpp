#include "RegistrationCommonHeaders.h"
#include "SolidZone.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto solid_sp = std::shared_ptr<Collidable>(new SolidZone(sf::FloatRect(position.x, position.y, 4 * X_STEP, 4 * -Y_STEP), true));
	subLevel_p->addCollidable(solid_sp, SubLevel::SEEK_RECIEVER);//all hitbox sizes are now inverted.
}

static ObjectTypeRegistration reg("collision256x128", regCallback);