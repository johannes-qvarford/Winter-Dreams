#include "LevelState.h"
#include "SubLevel.h"

#include <cassert>

LevelState::LevelState(const std::string& levelName):
	mSubLevels(),
	mCurrentSubLevel(),
	mPlayer_sp(),
	mCamera_sp(),
	mInventoryDisplay_sp(),
	mNextSpot(1),
	mFinishedSpot(0),
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
	//l�s inputManager
	//Kalla statemanagers updateFadingOut()
	auto it = mSubLevels.find(name);
	assert(it != mSubLevels.end());
	mCurrentSubLevel = it;
	//N�r mStateOfManager = NORMAL; s�tt mDirection till down och kalla p� updateFadingIn()
	//l�supp inputmanager
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
	for (unsigned int i = 0; i < mRegSoundVecSound.size();){
		if(mRegSoundVecSound[i].expired())
			mRegSoundVecSound.erase(mRegSoundVecSound.begin() + i);
		else {
			auto sound_sp = mRegSoundVecSound[i].lock();
			if(sound_sp->getStatus() == sf::Sound::Playing)
				mRegSoundVecSound[i].lock()->pause();
			i++;
		}
	}
	for (unsigned int i = 0; i < mRegSoundVecMusic.size(); i++){
		if(mRegSoundVecMusic[i].expired())
			mRegSoundVecMusic.erase(mRegSoundVecMusic.begin() + i);
		else {
			auto music_sp = mRegSoundVecMusic[i].lock();
			music_sp->setVolume(music_sp->getVolume() / 2);
			i++;
		}
	}
}

void LevelState::onUnfreeze(){

	for (unsigned int i = 0; i < mRegSoundVecSound.size();){
		if(mRegSoundVecSound[i].expired())
			mRegSoundVecSound.erase(mRegSoundVecSound.begin() + i);
		else {	
			auto sound_sp = mRegSoundVecSound[i].lock();
			if(sound_sp->getStatus() == sf::Sound::Paused)
				mRegSoundVecSound[i].lock()->play();
			i++;
		}
	}
	for (unsigned int i = 0; i < mRegSoundVecMusic.size(); i++){
		if(mRegSoundVecMusic[i].expired())
			mRegSoundVecMusic.erase(mRegSoundVecMusic.begin() + i);
		else {
			auto music_sp = mRegSoundVecMusic[i].lock();
			music_sp->setVolume(music_sp->getVolume() * 2);
			i++;
		}
	}
}

int LevelState::requestNarratorSpot(){
	mNextSpot++;

	return mNextSpot - 1;
}

bool LevelState::isSpotAvailable(int spot){
	if (spot == mFinishedSpot + 1)
		return true;
	else
		return false;
}

void LevelState::finishSpot(int spot){
	mFinishedSpot = spot;
}