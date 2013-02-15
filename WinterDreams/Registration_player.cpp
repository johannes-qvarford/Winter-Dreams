#include "RegistrationCommonHeaders.h"
#include "Player.h"
#include "InventoryDisplay.h"
#include "Camera.h"

static void regCallback(SubLevel* subLevel_p, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	
	//create entities
	auto player = std::shared_ptr<GraphicalEntity>(new Player(sf::FloatRect(position.x, position.y, 0, 0), pt.get<int>("properties.lightlevel"),!pt.get<bool>("properties.startdisabled", false) ) );
	auto camera = std::shared_ptr<Script>(new Camera(player));
	auto display = std::shared_ptr<Script>(new InventoryDisplay( std::static_pointer_cast<Player>(player) ));
	
	auto level_p = subLevel_p->getLevel();
	
	level_p->setPlayer(std::static_pointer_cast<Player>(player));
	level_p->setCamera(std::static_pointer_cast<Camera>(camera));
	level_p->setInventoryDisplay(std::static_pointer_cast<InventoryDisplay>(display));
//	auto& name = pt.get<std::string>("name","");
//	if(name != "")
//		subLevel_p->mapEntityToName(name, std::weak_ptr<Entity>(std::static_pointer_cast<Entity>(player)));
//	subLevel_p->addScript(display);	
}

static ObjectTypeRegistration reg("player", regCallback);