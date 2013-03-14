#include "PromptToMainButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "MenuState.h"
#include <SFML\Graphics\Texture.hpp>

class ReturnToMainSpecs {
public:

	static ReturnToMainSpecs& get(){ static ReturnToMainSpecs sSpecs; return sSpecs; }

	std::string mFilename;

private:

	ReturnToMainSpecs() {
		//exit(int) is a c function. use some other name
		auto& rtms = PropertyManager::get().getGeneralSettings().get_child("ui.ingamemenu.mainmenu");
		mFilename = rtms.get<std::string>("filename");
	}
};

PromptToMainButton::PromptToMainButton(const sf::Vector2f& position, std::shared_ptr<sf::Texture> bgTexture):
	Button(position, ReturnToMainSpecs::get().mFilename),
	mBgTexture( bgTexture ),
	mUpdated(false)
{ }

PromptToMainButton::~PromptToMainButton(){}

void PromptToMainButton::activate() {
	
	if(InputManager::get().isADown() || InputManager::get().isStartDown() ){

		auto& stateMgr = StateManager::get();

			//TODO: ADD PROMPT STATE
		stateMgr.freezeState(30);
		stateMgr.pushState(MenuState::makePromptQuitMenuState(mBgTexture) );
		stateMgr.unfreezeState(30);
	}
}
