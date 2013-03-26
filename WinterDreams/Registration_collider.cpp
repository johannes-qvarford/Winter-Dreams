#include "RegistrationCommonHeaders.h"
#include "SolidZone.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto& properties = pt.get_child("properties");
	auto name = pt.get<std::string>("name","");
	auto startdisabled = properties.get<bool>("startdisabled",false);
	auto adjustedWidth = pt.get<int>("width") / 32.f * X_STEP;
	auto adjustedHeight = -pt.get<int>("height") / 32.f * Y_STEP;//need to negate height.

	auto box = sf::FloatRect(position.x, position.y - adjustedHeight - Y_STEP, adjustedWidth, adjustedHeight);
	
	auto solid_sp = std::shared_ptr<SolidZone>(new SolidZone(box, !startdisabled));
	
	//does it have a name?
	if(name != "")
		subLevel_p->mapEntityToName(name, solid_sp);

	subLevel_p->addCollidable(solid_sp, SubLevel::SEEK_RECIEVER);	//do not seek collisions
}

static ObjectTypeRegistration reg("collider", regCallback);
