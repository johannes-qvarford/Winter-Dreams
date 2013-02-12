#include "StateManager.h"
#include "LevelState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"

#include <string>
#include <iostream>
#include <memory>

int main()
{
	try {
		auto levelState_p = new LevelState();
		auto firstLevelname = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");
		auto loadState_p = new LoadingState(firstLevelname, levelState_p);
		/*
			Push the level state first, and the load state second.
			The load state will be updated until it has initialized the level state,
			at which point it will be popped from the StateManager.
		*/
		StateManager::get().pushState(levelState_p);
		StateManager::get().pushState(levelState_p);

		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return 0;

}
