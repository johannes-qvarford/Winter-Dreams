#include "FootStep.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "ResourceManager.h"

class FootStepSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static FootStepSpecs& get(){ static FootStepSpecs sSpecs; return sSpecs; };
	
	struct Info {
		std::string wfilename;
		std::string efilename;
		std::string sfilename;
		std::string nfilename;
		std::string nefilename;
		std::string nwfilename;
		std::string sefilename;
		std::string swfilename;
		std::string soundfilename;
	};

	std::map<std::string, Info> mGrounds;

private:
	FootStepSpecs() {
		auto &fs = PropertyManager::get().getObjectSettings().get_child("objects.player.footsteps");
		for (auto it = fs.begin(); it != fs.end(); it++){
			auto& pair = *it;
			auto& obj = pair.second;

			auto name = obj.get<std::string>("name");
			Info info;
			info.sefilename = obj.get<std::string>("sefilename");
			info.swfilename = obj.get<std::string>("swfilename");
			info.nefilename = obj.get<std::string>("nefilename");
			info.nwfilename = obj.get<std::string>("nwfilename");
			info.sfilename = obj.get<std::string>("sfilename");
			info.efilename = obj.get<std::string>("efilename");
			info.nfilename = obj.get<std::string>("nfilename");
			info.wfilename = obj.get<std::string>("wfilename");
			info.soundfilename = obj.get<std::string>("soundfilename");
			mGrounds.insert(std::pair<std::string, Info>(name, info));
		}
	}
	FootStepSpecs(const FootStepSpecs& p);		//No copies
	FootStepSpecs& operator=(FootStepSpecs& p);	//No copies
};

FootStep::FootStep(sf::Vector2f position, sf::Vector2i direction, std::string groundName, int lifeLength):
Script(true),
mHitBox(position, sf::Vector2f(1, -1)),
mBuffer_sp(),
mSound_sp(new sf::Sound()),
mICanHasInit(false)
{
	auto& fss = FootStepSpecs::get();
	auto it = fss.mGrounds.find(groundName);
	auto& info = it->second;

	//struct DirectionFilenamePair { sf::Vector2i mDir; std::string mName; };

	//DirectionFilenamePair pairs[] = { 
	//	{sf::Vector2i(1,1), "sfilename"},
	//	{sf::Vector2i(-1,-1), "nfilename"},
	//	{sf::Vector2i(-1,1), "wfilename"},
	//	{sf::Vector2i(1,-1), "efilename"},
	//	{sf::Vector2i(0,1), "swfilename"},
	//	{sf::Vector2i(1,0), "sefilename"},
	//	{sf::Vector2i(-1,0), "nwfilename"},
	//	{sf::Vector2i(0,-1), "nefilename"}
	//};

	//std::string filename;
	//for(int i = 0; i < 8; ++i ) {
	//	if(pairs[i].mDir == mDirection) {
	//		filename = pairs[i].mName;
	//		break;
	//	}
	//}
	//std::string file;
	//if (filename == "sfilename")
	//	file = info.sfilename;
	//if (filename == "nfilename")
	//	file = info.nfilename;
	//if (filename == "wfilename")
	//	file = info.wfilename;
	//if (filename == "efilename")
	//	file = info.efilename;
	//if (filename == "swfilename")
	//	file = info.swfilename;
	//if (filename == "sefilename")
	//	file = info.sefilename;
	//if (filename == "nwfilename")
	//	file = info.nwfilename;
	//if (filename == "nefilename")
	//	file = info.nefilename;
	//
	//mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + "player/" + file);
	mBuffer_sp = ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + info.soundfilename);
	mSound_sp->setBuffer(*mBuffer_sp);
	//Volymen är satt på noll för vi har fel ljud och orka höra pickaxe-ljud hela tiden...
	mSound_sp->setVolume(0);

}

void FootStep::update(SubLevel* subLevel_p){
	if (!mICanHasInit){
		subLevel_p->getLevel()->registerSound(mSound_sp, LevelState::SOUND);
		mSound_sp->play();
		mICanHasInit = true;
	}
	if (mSound_sp->getStatus() == sf::Sound::Stopped)
		setAlive(false);
}

void FootStep::draw() const{
	//sf::Sprite sprite;
	//sprite.setTexture(*mTexture);
	//sprite.setPosition(GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top));
	//auto window_p = WindowManager::get().getWindow();
	//auto states_p = WindowManager::get().getStates();
	//window_p->draw(sprite, *states_p);
}