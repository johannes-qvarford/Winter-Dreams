#include "LightPoint.h"

#include "GameToScreen.h"
#include "Player.h"
#include "SubLevel.h"
#include "PropertyManager.h"
#include "WindowManager.h"
#include "FileStructure.h"

class LightPointSpecs{
public:
	static LightPointSpecs& get(){ static LightPointSpecs sSpecs; return sSpecs; }
	const std::list<AnimationSpecs>& getAnimSpecList(){ return mAnimSpecList; }
	
	std::map< int, float > mLightToDistance;
	float mBrightness;

private:

	std::list<AnimationSpecs> mAnimSpecList;

	LightPointSpecs();
	LightPointSpecs(const LightPointSpecs&);//no copy
	LightPointSpecs& operator=(const LightPointSpecs&);//no copy
};

LightPointSpecs::LightPointSpecs(){

	auto& obj = PropertyManager::get().getObjectSettings();
	auto& light = obj.get_child( "objects.light" );
	AnimationSpecs::parse( light, mAnimSpecList );
	mBrightness = light.get<float>("brightness");

	auto& lightsize = light.get_child("lightsize");
	for(auto it = lightsize.begin(), end = lightsize.end(); it != end; ++it){
			//Iterate over the childe tree and extract the data
		auto& firstName = it->first;
		auto secondValue = std::stof( it->second.data() );
		auto firstValue = std::stoi(firstName);
			//Store the data in the ligt to distance-map
		mLightToDistance.insert( std::pair<int, float>(firstValue, secondValue) );
	}
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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
	addLightSource( subLevel_p );

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

void LightPoint::addLightSource(SubLevel* subLevel_p){
	auto ID = WindowManager::get().getNextLightID();

	auto pos = sf::Vector2f(mHitBox.left, mHitBox.top );;
	pos.x += mHitBox.width/2.f;
	pos.y += mHitBox.height/2.f;

	auto dist = LightPointSpecs::get().mLightToDistance.find( mLightLevel )->second;

	subLevel_p->setLightPoint( ID, pos, 1.0, dist );
}