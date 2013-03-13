#include "MenuState.h"
#include "PlayButton.h"
#include "SettingsButton.h"
#include "CreditsButton.h"
#include "ExitButton.h"
#include "Cursor.h"
#include "QRDisplay.h"
#include "ResumeButton.h"
#include "MainMenuButton.h"
#include "ResolutionButton.h"
#include "SoundVolumeSlider.h"

#include "ResourceManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "PropertyManager.h"

#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Sound.hpp>

#include <boost/foreach.hpp>
#include <memory>

#define foreach BOOST_FOREACH

class MainMenuStateSpecs {
public:

	static MainMenuStateSpecs& get(){ static MainMenuStateSpecs sSpecs; return sSpecs; }

	std::string mBgFilename;
	std::string mFrameFilename;
	std::string mTitleFilename;
	float mTitleXOffset;
	float mTitleYOffset;

	std::string mBgmMusic;

	float mFrameXOffset;

	float mFrameYOffset;

private:

	MainMenuStateSpecs() {
		auto& mms = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu");
		mBgFilename = mms.get<std::string>("bgfilename");
		mFrameFilename = mms.get<std::string>("frame.filename");
		mFrameXOffset = mms.get<float>("frame.xoffset");
		mFrameYOffset = mms.get<float>("frame.yoffset");
		mTitleFilename = mms.get<std::string>("title.filename");
		mTitleXOffset = mms.get<float>("title.xoffset");
		mTitleYOffset = mms.get<float>("title.yoffset");

		mBgmMusic = mms.get<std::string>("bgmmusic");
	}
};

class IngameMenuStateSpecs {
public:

	static IngameMenuStateSpecs& get(){ static IngameMenuStateSpecs sInspecs; return sInspecs; }

	std::string mFrameFilename;

	float mFrameXOffset;

	float mFrameYOffset;

private:

	IngameMenuStateSpecs() {
		auto& igms = PropertyManager::get().getGeneralSettings().get_child("ui.ingamemenu");
		mFrameFilename = igms.get<std::string>("frame.filename");
		mFrameXOffset = igms.get<float>("frame.xoffset");
		mFrameYOffset = igms.get<float>("frame.yoffset");
	}
};

class SettingsMenuStateSpecs {
public:
	
	static SettingsMenuStateSpecs& get(){ static SettingsMenuStateSpecs sSetspecs; return sSetspecs;}
	std::string mFrameFilename;
	float mFrameXOffset;
	float mFrameYOffset;
private:
	SettingsMenuStateSpecs() {
		auto& sms = PropertyManager::get().getGeneralSettings().get_child("ui.settingsmenu");
		mFrameFilename = sms.get<std::string>("frame.filename");
		mFrameXOffset = sms.get<float>("frame.xoffset");
		mFrameXOffset = sms.get<float>("frame.yoffset");
	}
};

class CreditsMenuStateSpecs{
public:
	static CreditsMenuStateSpecs& get(){ static CreditsMenuStateSpecs s; return s;	}

	std::string mBackgroundFileName;

	std::string  mFrameFileName;
	sf::Vector2f mFrameOffset;
private:
	CreditsMenuStateSpecs() {
		auto& cms = PropertyManager::get().getGeneralSettings().get_child("ui.credits");

		mBackgroundFileName = cms.get<std::string>("bgfilename");

		mFrameFileName = cms.get<std::string>("frame.filename");
		mFrameOffset.x = cms.get<float>("frame.xoffset");
		mFrameOffset.y = cms.get<float>("frame.yoffset");
	}
};

MenuState* MenuState::makeMainMenuState() {
	auto& res = ResourceManager::get();
	auto& specs = MainMenuStateSpecs::get();

	auto state_p = new MenuState();

	auto widgets = std::vector<std::shared_ptr<Widget> >();
	
	//create buttons
	auto play_sp = std::make_shared<PlayButton>();
	auto settings_sp = std::make_shared<SettingsButton>();
	auto credits_sp = std::make_shared<CreditsButton>();
	auto exit_sp = std::make_shared<ExitButton>();
	auto title_sp = std::make_shared<Button>(sf::Vector2f(specs.mTitleXOffset, specs.mTitleYOffset), specs.mTitleFilename);
	auto frame_sp = std::make_shared<Button>(sf::Vector2f(specs.mFrameXOffset, specs.mFrameYOffset), specs.mFrameFilename);
	auto qr_sp = std::make_shared<QRDisplay>();
	
	widgets.push_back(play_sp);
	widgets.push_back(settings_sp);
	widgets.push_back(credits_sp);
	widgets.push_back(exit_sp);
	widgets.push_back(qr_sp);
	
	//create a cursor
	auto cursor_sp = std::make_shared<Cursor>(widgets);

	//add a background image.
	auto bg_sp = res.getTexture(FS_DIR_UI + specs.mBgFilename);

	//add some music
	auto music_sp = ResourceManager::get().getSoundBuffer( FS_DIR_MUSIC + specs.get().mBgmMusic );

	state_p->addWidget(play_sp);
	state_p->addWidget(settings_sp);
	state_p->addWidget(credits_sp);
	state_p->addWidget(exit_sp);
	state_p->addWidget(frame_sp);
	state_p->addWidget(title_sp);
	state_p->addWidget(cursor_sp);
	state_p->setBackground(bg_sp);
	state_p->addMusic(music_sp);
	state_p->addWidget(qr_sp);

	return state_p;
}

MenuState* MenuState::makeIngameMenuState(sf::Texture background) {
	auto& res = ResourceManager::get();
	auto& specs = IngameMenuStateSpecs::get();

	auto state_p = new MenuState();

	auto widgets = std::vector<std::shared_ptr<Widget >> ();

	//create buttons
	auto resume_sp = std::make_shared<ResumeButton>();
	auto settings_sp = std::make_shared<SettingsButton>();
	auto mainmenu_sp = std::make_shared<MainMenuButton>();
	auto frame_sp = std::make_shared<Button>(sf::Vector2f(specs.mFrameXOffset, specs.mFrameYOffset), specs.mFrameFilename);

	widgets.push_back(resume_sp);
	widgets.push_back(settings_sp);
	widgets.push_back(mainmenu_sp);

	//create a curson
	auto cursor_sp = std::make_shared<Cursor>(widgets);

	//add a background image
	auto bg_sp = std::make_shared<sf::Texture>(background);

	state_p->addWidget(resume_sp);
	state_p->addWidget(settings_sp);
	state_p->addWidget(mainmenu_sp);
	state_p->addWidget(frame_sp);
	state_p->addWidget(cursor_sp);
	state_p->setBackground(bg_sp);

	return state_p;
}

MenuState* MenuState::makeSettingsMenuState(sf::Texture background) {
	auto& res = ResourceManager::get();
	auto& specs = SettingsMenuStateSpecs::get();

	auto state_p = new MenuState();

	auto widgets = std::vector<std::shared_ptr<Widget >> ();

	//create buttons
//	auto resolution_sp = std::make_shared<ResolutionButton>();
	//auto vsync_sp = std::make_shared<VSyncButton>();
//	auto sndvol_sp = std::make_shared<SoundVolumeSlider>();
	/*auto mscvol_sp = std::make_shared<MusicVolumeSlider>();
	auto vicvol_sp = std::make_shared<VoiceVolumeSlider>();
	auto subtitle_sp = std::make_shared<SubtitleButton>();
	auto up_sp = std::make_shared<UpKeyButton>();
	auto down_sp = std::make_shared<DownKeyButton>();
	auto left_sp = std::make_shared<LeftKeyButton>();
	auto right_sp = std::make_shared<RightKeyButton>();
	auto use_sp = std::make_shared<UseKeyButton>();
	auto switch_sp = std::make_shared<SwitchKeyButton>();
	auto resume_sp = std::make_shared<ResumeButton>();*/

//	widgets.push_back(resolution_sp);
//	widgets.push_back(sndvol_sp);

	auto cursor_sp = std::make_shared<Cursor>(widgets);

	//add a background image
	auto bg_sp = std::make_shared<sf::Texture>(background);

//	state_p->addWidget(resolution_sp);
//	state_p->addWidget(sndvol_sp);

	return state_p;
}

MenuState* MenuState::makeCreditsMenuState(){
	auto& res = ResourceManager::get();
	auto& specs = CreditsMenuStateSpecs::get();

	auto state_p = new MenuState();
	auto widgets = std::vector<std::shared_ptr<Widget >> ();

	auto resume_sp = std::make_shared<ResumeButton>();
	auto frame_sp = std::make_shared<Button>(specs.mFrameOffset, specs.mFrameFileName);
	resume_sp->setBounds( sf::FloatRect( 1.2f, 1.2f, 0.1f, 0.1f ) );

	widgets.push_back(resume_sp);

	auto cursor_sp = std::make_shared<Cursor>(widgets);
	auto bg_sp = res.getTexture( FS_DIR_UI + specs.mBackgroundFileName );

	state_p->setBackground( bg_sp );
	state_p->addWidget(resume_sp);
	state_p->addWidget(cursor_sp);
	state_p->addWidget(frame_sp);

	return state_p;
}


MenuState::MenuState():
	mIsFreezing(false)
{
	mMusic = std::make_shared<sf::Sound>();
}


MenuState::~MenuState()
{
	mMusic->stop();
}

void MenuState::update() {
	auto status = mMusic->getStatus();
	if( status != sf::Sound::Playing){
		mMusic->play();
		mMusic->setVolume( 50.f );
	}

	if(mIsFreezing == false) {
		foreach(auto& widget_sp, mWidgets) {
			widget_sp->update(this);
		}
	}
}

void MenuState::render() {
	auto& window = *WindowManager::get().getRenderWindow();


#ifdef DEBUG_MAINMENUSTATE
	{
		static bool b = false;
		static int waiting = 0;
		waiting++;


		if(b == false)
			WindowManager::get().setVideoMode(800, 600, sf::Style::Default);
		b = true;

		static auto curWidgetIt = mWidgets.begin();

		//auto mp = sf::Mouse::getPosition(window);
		//auto normalPos = sf::Vector2f(float(mp.x) / window.getSize().x, float(mp.y) / window.getSize().y); 

		//find a new widget
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && waiting % 60) {

			curWidgetIt++;
			curWidgetIt = (curWidgetIt != mWidgets.end() ? curWidgetIt : mWidgets.begin());

			//foreach(auto widget_sp, mWidgets) {
			//	auto& box = widget_sp->getBounds();
			//	if(box.contains(normalPos)) {
			//		curWidget = widget_sp.get();
			//	}
			//}

		}

		auto& box = (*curWidgetIt)->getBounds();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
			box.left -= 0.001;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
			box.left += 0.001;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
			box.top -= 0.001;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			box.top += 0.001;
		}

		//if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		//	curWidget = nullptr;
		//}

		std::cout << box.left << ' ' << box.top << std::endl;
	}


#endif
	auto bgSprite = sf::Sprite();
	bgSprite.setScale(float(window.getSize().x) / 1920, float(window.getSize().y) / 1080);
	bgSprite.setTexture(*mBackground_sp);
	window.draw(bgSprite);

	foreach(auto widget_sp, mWidgets) {
		window.draw(*widget_sp);
	}
}

void MenuState::onFreeze() {
	mIsFreezing = true;
}

void MenuState::onUnfreeze() {
	mIsFreezing = true;
}

void MenuState::onEndUnfreeze() {
	mIsFreezing = false;
}

void MenuState::setBackground(std::shared_ptr<sf::Texture> texture_sp) {
	mBackground_sp = texture_sp;
}

void MenuState::addWidget(std::shared_ptr<Widget> widget_sp) {
	mWidgets.push_back(widget_sp);
}

void MenuState::addMusic(std::shared_ptr<sf::SoundBuffer> music_sp) {
	mMusicBuffer = music_sp;
	mMusic->setBuffer( *music_sp );
	mMusic->setLoop( true );
}
