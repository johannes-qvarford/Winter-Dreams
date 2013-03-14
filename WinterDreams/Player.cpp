#include "Player.h"

#include "Crystal.h"
#include "DamageHitBox.h"
#include "SubLevel.h"
#include "GameToScreen.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "FootStep.h"
#include <list>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

static void addHitBox( SubLevel* subLevel_p, Player* player, int dmgAmount, const std::string& type);
static int convert( int value );
static sf::Vector2i getSmartDirection(const sf::Vector2f& moveVec );

class PlayerSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PlayerSpecs& get();
	int mFramesPerStep;
	int mInvulFrames;
	float mBlinkFrameDistance;
	float mMoveSpeed;
	float mBrightness;
	std::list<AnimationSpecs> mAnimSpecList;
	std::map< int, float > mLightToDistance;

private:
	PlayerSpecs();							//Singleton-pattern
	PlayerSpecs(const PlayerSpecs& p);		//No copies
	PlayerSpecs& operator=(PlayerSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
PlayerSpecs::PlayerSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& player = obj.get_child( "objects.player" );
	mFramesPerStep = player.get<int>("framesperstep");
	mInvulFrames = player.get<int>( "invulnerabilityframes" );
	mBlinkFrameDistance = player.get<float>( "blinkframedistance" );
	mMoveSpeed = player.get<float>( "walkspeed" );
	mBrightness = player.get<float>("brightness");
	
	AnimationSpecs::parse( player, mAnimSpecList );

	auto& lightsize = player.get_child("lightsize");
	for(auto it = lightsize.begin(), end = lightsize.end(); it != end; ++it){
			//Iterate over the childe tree and extract the data
		auto& firstName = it->first;
		auto secondValue = std::stof( it->second.data() );
		auto firstValue = std::stoi(firstName);
			//Store the data in the ligt to distance-map
		mLightToDistance.insert( std::pair<int, float>(firstValue, secondValue) );
	}
}
////////////////////////////////////////////////////////////////////////////////
PlayerSpecs& PlayerSpecs::get() { 
	static PlayerSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////
Player::Player(sf::FloatRect initialPosition, int lightLevel, bool startEnabled) :
	GraphicalEntity( startEnabled ),
	mActionCooldown( 0 ),
	mIsActionActive( false),
	mInventory (Inventory() ),
	mMoveSpeed( PlayerSpecs::get().mMoveSpeed ),
	mMovementMode(NORMAL),
	mLightLevel( lightLevel ),
	mDirection( 0,0),
	mFacingDir( 1,1),
	mHitBox( initialPosition.left, initialPosition.top, X_STEP , -Y_STEP ), //All hitbox heights are now inverted, ask Johannes.
	mFramesSinceLastBlink(0),
	mFramesSinceLastHit(1000),
	mCurrentLightIntensity( PlayerSpecs::get().mLightToDistance.find( mLightLevel )->second ),
	mTargetLightIntensity( mCurrentLightIntensity ),
	mDeltaLightIntensity(0),
	mTargetLightLastFrame( mTargetLightIntensity),
	mIsInvisible(false),
	mFrameCount(0),
	mFramesPerStep(36),
	mRightFoot(true)
{
	using namespace std;
	auto& p = PlayerSpecs::get();
		//construct the animation map
	Animation::fromListToMap(p.mAnimSpecList, FS_DIR_OBJECTANIMATIONS + "player/", &mAnimationMap);
	mCurrentAnimation_p = &mAnimationMap.begin()->second;
	


}

Player::~Player() {}

void Player::update(SubLevel* subLevel_p){

	/////////////////////////////////////////////////////////
	// If the player is enabled, perform a series of 
	// functions. Otherwize, only update the players animation
	/////////////////////////////////////////////////////////
	if( getEnabled() ){
		addLightSource(subLevel_p);

		updateMovement(subLevel_p);
			/////////////////////////////////////////////////////////
			// If any component of the movement direction is greater then 1,
			// we will assign the facingDirection to the same values and
			// moving direction.
			//
			// If all components of the movement direction is 0, the 
			// player did not move and is hence facing the same direction
			// as the previous frame.
			/////////////////////////////////////////////////////////
		if( mDirection.x != 0 || mDirection.y != 0){
			assert( mDirection.x != 0 || mDirection.y != 0 ); //ASSERT!
			int x = 0;
			int y = 0;
			if (mDirection.x != 0)
				x = convert( mDirection.x );
			if (mDirection.y != 0)
				y = convert( mDirection.y );

			mFacingDir = sf::Vector2i( x, y );
			assert( x != 0 || y != 0 ); //ASSERT!
		}

		assignMoveAnimations(subLevel_p);
		updateActions(subLevel_p);
	}

	updateInvulnerable();
	updateCurrentAnimation();
}

void Player::drawSelf(){
	auto& specs = PlayerSpecs::get();

	//blink if your invulnerable(and it has been a certain number of frames since the last blink).
	if(mIsInvisible)
		return;
	
	//Get the current animation's sprite
	auto sprite = mCurrentAnimation_p->getCurrentSprite();
	//Assign the sprite a position (in Screen Coordinates)
	sprite.setPosition( GAME_TO_SCREEN * getPosition() );
	//Draw the sprite
	WindowManager::get().getWindow()->draw( sprite ,*WindowManager::get().getStates() );
}

sf::FloatRect& Player::getHitBox(){
	return mHitBox;
}

void Player::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection){
}

sf::Vector2f Player::getPosition(){
	return sf::Vector2f(mHitBox.left, mHitBox.top);
}

void Player::setPosition(const sf::Vector2f& position){
	mHitBox.left = position.x;
	mHitBox.top = position.y;
}

void Player::adjustPosition(const sf::Vector2f& positionAdjustment){
	mHitBox.left += positionAdjustment.x;
	mHitBox.top += positionAdjustment.y;
}

int Player::getCurrentLightLevel() const{
	return mLightLevel;
}

void Player::setCurrentLightLevel(const int lightLevel){
	mLightLevel = lightLevel;

	if( mLightLevel < 1)
		mLightLevel = 1;
	if( mLightLevel > 10 )
		mLightLevel = 10;
}

void Player::adjustCurrentLightLevel(const int lightLevelAdjustment){
	auto i = mLightLevel;
	i += lightLevelAdjustment;
	setCurrentLightLevel( i );
}

const Inventory& Player::getInventory() const{
	return mInventory;
}

Inventory* Player::changeInventory(){
	return &mInventory;
}

MovementMode Player::getCurrentMovementMode(){
	return mMovementMode;
}

void Player::setMovementMode(MovementMode movementMode){
	mMovementMode=movementMode;
}

sf::Vector2i Player::getDirection(){
	return mDirection;
}

void Player::updateMovement(SubLevel* subLevel_p) {
	if( mActionCooldown > 0 )
		return;

	//Create a temporary vector that will store the directions
	//corresponding to the keys pressed.
	mDirection=sf::Vector2i(0,0);

	//Find the actual keys pressed.
	sf::Vector2f stick = InputManager::get().getStick();

		//Extend tempDir by the avatars move speed
	stick *= static_cast<float>(mMoveSpeed);
		//Adjust the avatars position by tempDir
	adjustPosition( stick );

	//TODO::Calculate rotation for setting direction on the avatar.

	if( abs(stick.x) > 0.1 || abs(stick.y) > 0.1 )
	mDirection = getSmartDirection( stick );
					//if( stick.x > 0 ||stick.y > 0)
					//	mDirection=sf::Vector2i(static_cast<int>(stick.x),static_cast<int>(stick.y));
					//else
					//	mDirection=sf::Vector2i(static_cast<int>(stick.x*2),static_cast<int>(stick.y*2));

	if (mDirection != sf::Vector2i(0, 0))
		mFrameCount++;

	// creates a new footstep
	if (mFrameCount == mFramesPerStep){
		mFrameCount = 0;
		sf::Vector2f offset;
		//north and south
		if ((mFacingDir == sf::Vector2i(-1, -1) || mFacingDir == sf::Vector2i(1, 1)) && mRightFoot)
			offset = sf::Vector2f(7, -7);
		//northeast and southwest
		if ((mFacingDir == sf::Vector2i(0, -1) || mFacingDir == sf::Vector2i(0, 1)) && mRightFoot)
			offset = sf::Vector2f(7, 0);
		//east and west
		if ((mFacingDir == sf::Vector2i(1, -1) || mFacingDir == sf::Vector2i(-1, 1)) && mRightFoot)
			offset = sf::Vector2f(3, 3);
		//northwest and southeast
		if ((mFacingDir == sf::Vector2i(1, 0) || mFacingDir == sf::Vector2i(-1, 0)) && mRightFoot)
			offset = sf::Vector2f(0, -7);

		mRightFoot = !mRightFoot;
		auto footStep_sp = std::shared_ptr<FootStep>(new FootStep(sf::Vector2f(mHitBox.left + 11, mHitBox.top - 15) + offset, mFacingDir, "ice", 124));
		subLevel_p->addScript(footStep_sp);

	}
}


void Player::assignMoveAnimations(SubLevel* subLevel_p) {

	/////////////////////////////////////////////////////////////////
	if( mDirection.x > 0) {
		if( mDirection.y > 0 )
			mCurrentAnimation_p = &mAnimationMap.find("front")->second;
		if( mDirection.y < 0 )
			mCurrentAnimation_p = &mAnimationMap.find("right")->second;
		if( mDirection.y == 0 )
			mCurrentAnimation_p = &mAnimationMap.find("frontright")->second;
	}
	if( mDirection.x < 0) {
		if( mDirection.y > 0 )
			mCurrentAnimation_p = &mAnimationMap.find("left")->second;
		if( mDirection.y < 0 )
			mCurrentAnimation_p = &mAnimationMap.find("back")->second;
		if( mDirection.y == 0 )
			mCurrentAnimation_p = &mAnimationMap.find("backleft")->second;
	}
	if( mDirection.x == 0 ) {
		if( mDirection.y > 0 )
			mCurrentAnimation_p = &mAnimationMap.find("frontleft")->second;
		if( mDirection.y < 0 )
			mCurrentAnimation_p = &mAnimationMap.find("backright")->second;
		if( mDirection.y == 0 )				
			mCurrentAnimation_p->resetAnimation(); //If the character did not move, idle.
	}
}


void Player::updateActions(SubLevel* subLevel_p) {
	if( mActionCooldown > 0 ){
		mActionCooldown--;
	}

	if( InputManager::get().isBDown() && mActionCooldown <= 0 ){
		mInventory.equipNext();
		mActionCooldown = 5;
	}

	if( InputManager::get().isADown()  && mActionCooldown <= 0 ){
		if( mInventory.getCurrentEquip() == "pickaxe" ){
			addHitBox( subLevel_p, this, 1, mInventory.getCurrentEquip() );
			mActionCooldown = 40;
		}
	}
}

void Player::updateInvulnerable() {
	
	auto& specs = PlayerSpecs::get();
	
	//have we been hit recently, and is it time to blink?
	if(isVulnerable() == false) {
		++mFramesSinceLastBlink;
		if(mFramesSinceLastBlink > specs.mBlinkFrameDistance) {
			mFramesSinceLastBlink -= int(specs.mBlinkFrameDistance);
			mIsInvisible = true;
		}
		else
			mIsInvisible = false;
	}
	else
		mIsInvisible = false;

	++mFramesSinceLastHit;
}

void Player::updateCurrentAnimation() {
	mCurrentAnimation_p->updateAnimation();
}

void Player::setFacingDirection(sf::Vector2i dir){
	//if( mDirection.x > 0 || mDirection.y > 0)
		mFacingDir = dir;
	mDirection = dir;
	assignMoveAnimations(nullptr);
}

sf::Vector2i Player::getFacingDirection() const {
	return mFacingDir;
//	return mDirection;
}

void Player::addLightSource(SubLevel* subLevel_p){
	auto ID = WindowManager::get().getNextLightID();

	auto pos = getPosition();
	pos.x += mHitBox.width/2.f;
	pos.y += mHitBox.height/2.f;

	mTargetLightIntensity = PlayerSpecs::get().mLightToDistance.find( mLightLevel )->second;

	if(mTargetLightIntensity != mTargetLightLastFrame)
		mDeltaLightIntensity = (mTargetLightIntensity - mTargetLightLastFrame) / 50;

	//If current value is not close to the target value, increase current value by the delta value
	if( abs(mTargetLightIntensity - mCurrentLightIntensity) > 0.01)
		mCurrentLightIntensity += mDeltaLightIntensity;

	auto& brightness = PlayerSpecs::get().mBrightness;
	auto& distance = mCurrentLightIntensity;

	subLevel_p->setLightPoint( ID, pos, brightness, distance );

	mTargetLightLastFrame = mTargetLightIntensity;
}

bool Player::isVulnerable() const{
	//has it passed enough time?
	auto& specs = PlayerSpecs::get();
	return mFramesSinceLastHit > specs.mInvulFrames;
}

void Player::setInvulnerable() {
	//we're been hit!
	mFramesSinceLastHit = 0;
	mFramesSinceLastBlink = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

static void addHitBox( SubLevel* subLevel_p, Player* player, int dmgAmount, const std::string& type) {
	auto& hitBox = player->getHitBox();
	auto faceDir = player->getFacingDirection();

	auto x = hitBox.left;
	auto y = hitBox.top;
	auto width = hitBox.width;
	auto height = hitBox.height;		

	auto newPosX = x + (width*faceDir.x )/2;
	auto newPosY = y + (-height*faceDir.y )/2;
	
	sf::FloatRect rect( newPosX, newPosY, width, height);

	std::shared_ptr<GraphicalEntity> dmgHitBox( new DamageHitBox(rect, dmgAmount, type) );

	subLevel_p->addGraphicalEntity( dmgHitBox );
}

static int convert( int value) {
	return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}

static sf::Vector2i getSmartDirection(const sf::Vector2f& moveVec ) {
	auto vec = moveVec;	
	if( abs(vec.x) < 0.01f )
		vec.x = 0.01f;

	sf::Vector2i dir(0,0);

	bool rightAligned = vec.x > 0.f;
	float angle;
	angle = atan( vec.y / vec.x );
	angle *= 180.f/ static_cast<float>(M_PI);

	if( angle > 67.5f )
		dir = sf::Vector2i(0,1);
	else if( angle > 22.5f )
		dir = sf::Vector2i(1,1);
	else if( angle > -22.5f )
		dir = sf::Vector2i(1,0);
	else if( angle > -67.5f )
		dir = sf::Vector2i(1,-1);
	else
		dir = sf::Vector2i(0,-1);

	if(!rightAligned){
		dir.x *= -1; dir.y*= -1;
	}

	return dir;
}
