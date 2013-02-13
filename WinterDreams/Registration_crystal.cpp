#include "RegistrationCommonHeaders.h"
#include "Crystal.h"

static void regCallback(SubLevel* subLevel, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
		//Read the ptree for info whether it should be enabled or not
	auto startEnabled = !pt.get<bool>("startdisabled",false);//not "startenabled"
		//Create a rect to position the crystal
	sf::FloatRect rect( position.x, position.y, X_STEP, -Y_STEP );
		//Create a shared pointer to a crystal
	auto crystal = std::shared_ptr<Crystal>( new Crystal( rect, startEnabled ) );
		//Add the crystal to the sub level
	subLevel->addGraphicalEntity( crystal );
}

static ObjectTypeRegistration reg("crystal", regCallback);