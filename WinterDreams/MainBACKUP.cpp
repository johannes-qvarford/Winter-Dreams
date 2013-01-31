#include "StateManager.h"
#include "GameState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"

#include <string>
#include <iostream>
#include <memory>

/*int main()
{
	try {
		auto gameState_p = new GameState();
		auto firstLevelFilename = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_filename");
		auto loadState_p = new LoadingState(gameState_p, FS_DIR_LEVELS + firstLevelFilename);
		/*
			Push the game state first, and the load state second.
			The load state will be updated until it has initialized the game state,
			at which point it will be popped from the StateManager.
		*/
		/*StateManager::get().pushState(gameState_p);
		StateManager::get().pushState(loadState_p);

		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return 0;

}*/