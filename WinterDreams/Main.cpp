#include "StateManager.h"
#include "LevelState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"

#include <string>
#include <iostream>
#include <memory>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


//////////
#include "GameToScreen.h"
#include <SFML/System/Vector2.hpp>

int main()
{
	try {

		//add LoadingState to the top of the stack.
		auto firstLevelname = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");
		auto loadState_p = new LoadingState(firstLevelname);
		StateManager::get().pushState(loadState_p);

		//run the game.
		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}

	return 0;

}
