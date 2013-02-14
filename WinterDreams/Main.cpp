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
		//create the first LevelState, and put it on top of the stack.
		auto levelState_p = new LevelState();
		StateManager::get().pushState(levelState_p);

		//add LoadingState to the top of the stack, filling the first level state with information.
		auto firstLevelname = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");
		auto loadState_p = new LoadingState(firstLevelname, levelState_p);
		StateManager::get().pushState(loadState_p);

		//run the game.
		StateManager::get().run();
	}
	catch(std::exception& e) {
		std::cout << "exception thrown: " << e.what() << std::endl;
	}

	return 0;

}
