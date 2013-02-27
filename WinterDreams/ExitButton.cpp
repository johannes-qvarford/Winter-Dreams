#include "ExitButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"

class ExitButtonSpecs {
public:

	static ExitButtonSpecs& get(){ static ExitButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	ExitButtonSpecs() {
		//exit(int) is a c function. use some other name
		auto& exit_ = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.exit");
		mXOffset = exit_.get<float>("xoffset");
		mYOffset = exit_.get<float>("yoffset");
		mFilename = exit_.get<std::string>("filename");
	}
};

ExitButton::ExitButton():
	Button(sf::Vector2f(ExitButtonSpecs::get().mXOffset, ExitButtonSpecs::get().mYOffset), ExitButtonSpecs::get().mFilename),
	mUpdated(false)
{
}

void ExitButton::activate() {
	
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;

		auto& stateMgr = StateManager::get();

		stateMgr.freezeState();
		stateMgr.popState();
	}
}
