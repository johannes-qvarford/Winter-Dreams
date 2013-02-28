#include "StateManager.h"
#include "VideoState.h"
#include "LevelState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"
#include "InputManager.h"

#include <string>
#include <iostream>
#include <memory>
#include <cassert>

int main()
{
	try {

		auto& prop = PropertyManager::get();
		auto& settings = prop.getGeneralSettings();
		auto& level_or_menu = settings.get<std::string>("level_or_menu");
		//add LoadingState to the top of the stack.

		if(level_or_menu == "level") {
			//auto firstLevelname = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");
			//auto loadState_p = new LoadingState(firstLevelname);
			//StateManager::get().pushState(loadState_p);
		}
		else if(level_or_menu == "menu") {
			auto videoState_p = new VideoState("loading_ogg_test.ogg");
			StateManager::get().pushState(videoState_p);
			StateManager::get().unfreezeState();
		}
		else {
			assert(0);
		}

		//run the game.
		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}

	return 0;

}
