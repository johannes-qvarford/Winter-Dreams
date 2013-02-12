#include "LevelState.h"
#include "SubLevel.h"

#include <cassert>

LevelState::LevelState():
	mSubLevels(),
	mCurrentSubLevel(),
	mPlayer_sp(),
	mCamera_sp(),
	mInventoryDisplay_sp()
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
