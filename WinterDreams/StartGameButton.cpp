#include "StartGameButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"

static const char * const BOX_FILENAME = "placeholder_box.png"; 
static const char * const FONT_FILENAME = "arial.ttf";
static const char * const TEXT_STRING = "start game";

StartGameButton::StartGameButton(const sf::Vector2f& initialPosition):
	Button(initialPosition, TEXT_STRING, FS_DIR_UI + BOX_FILENAME, FS_DIR_FONTS + FONT_FILENAME, 20),
	mUpdated(false)
{
}

void StartGameButton::activate() {
	

	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;
		Button::activate();

		auto first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");

		auto loadingState_p = new LoadingState(first_level_name);
		auto& stateMgr = StateManager::get();

		stateMgr.freezeState();
		stateMgr.popState();
		stateMgr.pushState(loadingState_p);
		stateMgr.unfreezeState(10);

		onHover(false);
	}
}
