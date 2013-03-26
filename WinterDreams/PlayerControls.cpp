#include "PlayerControls.h"
#include "InputManager.h"
#include "SubLevel.h"

PlayerControls::PlayerControls(bool startsEnabled):
	Entity(startsEnabled)
{
}

void PlayerControls::update(SubLevel* subLevel_p) {
	if(getEnabled())
		InputManager::get().unlockInput();
	else
		InputManager::get().lockInput();
}