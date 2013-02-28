#include "LevelState.h"
#include "SubLevel.h"

#include <cassert>

LevelState::LevelState(const std::string& levelName):
	mSubLevels(),
	mCurrentSubLevel(),
	mPlayer_sp(),
	mCamera_sp(),
	mInventoryDisplay_sp(),
	mLevelName(levelName)
{

}

LevelState::~LevelState() {
}

void LevelState::update() {
	auto& subLevel_sp = mCurrentSubLevel->second;
	subLevel_sp->update();
}

void LevelState::render() {
	auto& subLevel_sp = mCurrentSubLevel->second;
	subLevel_sp->render();
}

void LevelState::addSubLevel(const std::string& name, std::shared_ptr<SubLevel> subLevel_sp) {
	mSubLevels[name] = subLevel_sp;
}

void LevelState::switchSubLevel(const std::string& name) {
	auto it = mSubLevels.find(name);
	assert(it != mSubLevels.end());
	mCurrentSubLevel = it;
}

std::shared_ptr<SubLevel> LevelState::getSubLevel(const std::string& name) {
	auto it = mSubLevels.find(name);
	assert(it != mSubLevels.end());
	return it->second;
}

void LevelState::setPlayer(std::shared_ptr<Player> player_sp) {
	mPlayer_sp = player_sp;
}

void LevelState::setCamera(std::shared_ptr<Camera> camera_sp) {
	mCamera_sp = camera_sp;
}

void LevelState::setInventoryDisplay(std::shared_ptr<InventoryDisplay> display_sp) {
	mInventoryDisplay_sp = display_sp;
}

std::shared_ptr<Player> LevelState::getPlayer() {
	return mPlayer_sp;
}

std::shared_ptr<Camera> LevelState::getCamera() {
	return mCamera_sp;
}

std::shared_ptr<InventoryDisplay> LevelState::getInventoryDisplay() {
	return mInventoryDisplay_sp;
}

const std::string& LevelState::getLevelName() const {
	return mLevelName;
}

void LevelState::registerSound(std::shared_ptr<sf::Sound> sound, SoundType type){

	switch (type){
	case SOUND:
	case NARRATOR:
		{
			mRegSoundVecSound.push_back(sound);
			break;
		}

	case MUSIC:
		{
			mRegSoundVecMusic.push_back(sound);
			break;
		}
	}

}

void LevelState::onFreeze(){
	for (int i = 0; i < mRegSoundVecSound.size(); i++){
			mRegSoundVecSound[i]->pause();
	}
	for (int i = 0; i < mRegSoundVecMusic.size(); i++){
			auto sound_sp = mRegSoundVecMusic[i];
			sound_sp->setVolume(sound_sp->getVolume() * 0.5);
	}
}

void LevelState::onUnfreeze(){

	for (int i = 0; i < mRegSoundVecSound.size(); i++){
			mRegSoundVecSound[i]->play();
	}
	for (int i = 0; i < mRegSoundVecMusic.size(); i++){
			auto sound_sp = mRegSoundVecMusic[i];
			sound_sp->setVolume(sound_sp->getVolume() * 2);
	}
}
