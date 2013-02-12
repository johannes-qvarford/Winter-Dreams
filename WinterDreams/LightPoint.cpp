#include "LightPoint.h"

#include "GameState.h"
#include "GameToScreen.h"
#include "Player.h"
#include "PropertyManager.h"
#include "WindowManager.h"
#include "FileStructure.h"

class LightPointSpecs{
public:

	static LightPointSpecs& get(){ static LightPointSpecs sSpecs; return sSpecs; }

	const std::list<AnimationSpecs>& getAnimSpecList(){ return mAnimSpecList; }

private:

	std::list<AnimationSpecs> mAnimSpecList;

	LightPointSpecs() {  
		auto& obj = PropertyManager::get().getObjectSettings();
		auto& player = obj.get_child( "objects.light" );
		AnimationSpecs::parse( player, mAnimSpecList );
	};

	LightPointSpecs(const LightPointSpecs&);//no copy

	LightPointSpecs& operator=(const LightPointSpecs&);//no copy
};

LightPoint::LightPoint(const sf::FloatRect& initialPosition, int lightLevel, bool once, bool startEnabled):
	GraphicalEntity(startEnabled),
	mCurrentAnimation_p(NULL),
	mAnimationMap(),
	mHitBox(initialPosition.left, initialPosition.top, X_STEP, -Y_STEP),
	mLightLevel(lightLevel),
	mOnce(once)
{
	auto& lp = LightPointSpecs::get();
	Animation::fromListToMap(lp.getAnimSpecList(), FS_DIR_OBJECTANIMATIONS + "npc/", &mAnimationMap);
	auto it = mAnimationMap.find("placeholder");
	mCurrentAnimation_p = &it->second;
}

void LightPoint::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection) {
	//don't do anything if not enabled
	if(getEnabled() == false)
		return;

	//don't do anything if not colliding with player
	auto player_p = dynamic_cast<Player*>(entityCollidedWith_p);
	if(player_p == nullptr)
		return;

	//destroy self if only supposed to be activated once
	if(mOnce)
		setAlive(false);

	//get the highest light level; the light points, or the players
	auto playerLightLevel = player_p->getCurrentLightLevel();
	auto newLightLevel = std::max(mLightLevel, playerLightLevel);

	//adjust light level
	auto adjustLightLevel = newLightLevel - playerLightLevel;
	player_p->adjustCurrentLightLevel(adjustLightLevel);
}

void LightPoint::update(SubLevel* subLevel_p) {
	//do nothing
}

void LightPoint::drawSelf() {
	auto& manager = WindowManager::get();
	auto& window = *manager.getWindow();
	auto& states = *manager.getStates();
	auto sprite = mCurrentAnimation_p->getCurrentSprite();
	auto& hitBox = getHitBox();
	auto position = GAME_TO_SCREEN * sf::Vector2f(hitBox.left, hitBox.top);

	sprite.setPosition(position);

	window.draw(sprite, states);
}

sf::FloatRect& LightPoint::getHitBox() {
	return mHitBox;
}