#include "StateManager.h"
#include "GameState.h"
#include "LoadingState.h"

#include <string>


static const char * const FIRST_LEVEL_FILENAME = "changeme.json";

int main()
{
	auto gameState_p = new GameState();
	auto loadState_p = new LoadingState(gameState_p, FIRST_LEVEL_FILENAME);

	/*
		Push the game state first, and the load state second.
		The load state will be updated until it has initialized the game state,
		at which point it will be popped from the StateManager.
	*/
	StateManager::get().pushState(gameState_p);
	StateManager::get().pushState(loadState_p);

	StateManager::get().run();
	return 0;
}