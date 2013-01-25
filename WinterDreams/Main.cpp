#include "StateManager.h"
#include "GameState.h"
#include "LoadingState.h"
#include "Player.h"
#include <SFML\System\Vector2.hpp>
#include "GameToScreen.h"

#include <string>
#include <memory>

static const char * const FIRST_LEVEL_FILENAME = "Resources/Levels/Level1.json";

int main()
{

	try {
		auto gameState_p = new GameState();
		auto loadState_p = new LoadingState(gameState_p, FIRST_LEVEL_FILENAME);
		gameState_p->addPhysicalEntity(std::shared_ptr<PhysicalEntity>(new Player(sf::Vector2f(28*X_STEP,32*X_STEP) ) ) );
		/*
			Push the game state first, and the load state second.
			The load state will be updated until it has initialized the game state,
			at which point it will be popped from the StateManager.
		*/
		StateManager::get().pushState(gameState_p);
		StateManager::get().pushState(loadState_p);

		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return 0;

}