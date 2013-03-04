#include "ResumeButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"

class ResumeButtonSpecs {
public:

	static ResumeButtonSpecs& get(){ static ResumeButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	ResumeButtonSpecs() {
		auto& resume = PropertyManager::get().getGeneralSettings().get_child("ui.ingamemenu.resume");
		mXOffset = resume.get<float>("xoffset");
		mYOffset = resume.get<float>("yoffset");
		mFilename = resume.get<std::string>("filename");
	}
};

ResumeButton::ResumeButton():
	Button(sf::Vector2f(ResumeButtonSpecs::get().mXOffset, ResumeButtonSpecs::get().mYOffset), ResumeButtonSpecs::get().mFilename),
	mUpdated(false)
{
}

void ResumeButton::activate() {
	
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();

		stateMgr.popState();
		stateMgr.unfreezeState(10);

		onHover(false);
	}
}
