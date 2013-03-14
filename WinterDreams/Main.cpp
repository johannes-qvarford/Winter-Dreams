#include "StateManager.h"
#include "VideoState.h"
#include "LevelState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"
#include "InputManager.h"
#include "MenuState.h"

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
		}
		else if(level_or_menu == "menu") {
			auto menu = MenuState::makeMainMenuState();
			auto charmVideoState_p = new VideoState("Charm_logotype_3_1_deluxe.ogv", "MonoLight.ogg");
		//	auto ukontrollVideoState_p = new VideoState("ukontroll-logo-sting-2013-720p.ogv");
			StateManager::get().pushState( menu );
		//	StateManager::get().pushState(ukontrollVideoState_p);
			StateManager::get().pushState(charmVideoState_p);
			StateManager::get().unfreezeState(10);
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
