#include "StartGameButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"

StartGameButton::StartGameButton(const sf::Vector2f& initialPosition, std::shared_ptr<sf::Font> font_sp, const sf::Text& text):
	Button(initialPosition, ResourceManager::get().getTexture(FS_DIR_UI + "placeholder.png"), text, font_sp)
{
}

void StartGameButton::activate() {
	Button::activate();

	auto& first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");

	auto loadingState_p = new LoadingState(first_level_name);
	auto& stateMgr = StateManager::get();

	stateMgr.freezeState();
	stateMgr.popState();
	stateMgr.pushState(loadingState_p);
	stateMgr.unfreezeState();
}
