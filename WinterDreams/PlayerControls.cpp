#include "PlayerControls.h"
#include "InputManager.h"
#include "SubLevel.h"

PlayerControls::PlayerControls(bool startsEnabled):
	Script(startsEnabled)
{
}

void PlayerControls::update(SubLevel* subLevel_p) {
	if(getEnabled())
		InputManager::get().unlockInput();
	else
		InputManager::get().lockInput();
}

void PlayerControls::draw() const {
}