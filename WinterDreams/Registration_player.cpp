#include "RegistrationCommonHeaders.h"
#include "Player.h"
#include "InventoryDisplay.h"
#include "Camera.h"

static void regCallback(GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto player = std::shared_ptr<GraphicalEntity>(new Player(sf::FloatRect(position.x, position.y, 0, 0), pt.get<int>("properties.lightlevel"),!pt.get<bool>("properties.startdisabled", false) ) );
	auto camera = std::shared_ptr<Script>(new Camera(player));
	auto display = std::shared_ptr<Script>(new InventoryDisplay( std::static_pointer_cast<Player>(player) ));
	state->addGraphicalEntity(player);
	state->addScript(camera);
	
}

static ObjectTypeRegistration reg("player", regCallback);