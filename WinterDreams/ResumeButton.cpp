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
		auto& resume = PropertyManager::get().getGeneralSettings().get_child("ui.ingamemenu.continue");

		mXOffset = resume.get<float>("xoffset");
		mYOffset = resume.get<float>("yoffset");
		mFilename = resume.get<std::string>("filename");
	}
};

ResumeButton::ResumeButton(const std::string& textureFileName, const unsigned int& fadeInTime, const unsigned int& fadeOutTime):
	Button(sf::Vector2f(ResumeButtonSpecs::get().mXOffset, ResumeButtonSpecs::get().mYOffset),
		  ( (textureFileName == "") ? ResumeButtonSpecs::get().mFilename : textureFileName) ), //Button's constructor end here
	mFadeInTime( fadeInTime ),
	mFadeOutTime( fadeOutTime ),
	mUpdated(false)
{
}

void ResumeButton::activate() {
	
	if(mUpdated == false && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();

		stateMgr.freezeState(mFadeOutTime);
		stateMgr.popState();
		stateMgr.unfreezeState(mFadeInTime);

		onHover(false);
	}
}
