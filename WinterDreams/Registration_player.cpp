#include "RegistrationCommonHeaders.h"
#include "Player.h"
#include "InventoryDisplay.h"
#include "Camera.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	//create entities
	auto player = std::shared_ptr<Player>(new Player(sf::FloatRect(position.x, position.y, 0, 0), pt.get<int>("properties.lightlevel", 3),!pt.get<bool>("properties.startdisabled", false) ) );
	auto camera = std::shared_ptr<Camera>(new Camera(player));
	auto display = std::shared_ptr<InventoryDisplay>(new InventoryDisplay( std::static_pointer_cast<Player>(player) ));
	
	auto level_p = subLevel_p->getLevel();
	
	level_p->setPlayer(player);
	level_p->setCamera(camera);
	level_p->setInventoryDisplay(display);
}

static ObjectTypeRegistration reg("player", regCallback);