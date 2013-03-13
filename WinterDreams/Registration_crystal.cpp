#include "RegistrationCommonHeaders.h"
#include "Crystal.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {

	//auto& properties = pt.get_child("properties");
	//auto startdisabled = properties.get<bool>("startdisabled", false);
	//auto name = pt.get<std::string>("name");
	
	sf::FloatRect rect( position.x, position.y, X_STEP, -Y_STEP );

	auto crystal_sp = std::shared_ptr<Crystal>( new Crystal( rect, true ) );

	//if(name != "")
	//	subLevel_p->mapEntityToName(name, crystal_sp);

	subLevel_p->addGraphicalEntity(crystal_sp);
}

static ObjectTypeRegistration reg("crystal", regCallback);