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
#include <list>
#include <cmath>
#include <iostream>

static void addHitBox( SubLevel* subLevel_p, Player* player, int dmgAmount, const std::string& type);
static int convert( int value );

class PlayerSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PlayerSpecs& get();


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
	mHitBox( initialPosition.left, initialPosition.top, X_STEP , -Y_STEP ) //All hitbox heights are now inverted, ask Johannes.
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

	updateCurrentAnimation();
}

void Player::drawSelf(){
		//Get the current animation's sprite
	auto& sprite = mCurrentAnimation_p->getCurrentSprite();
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
	
	//Create a temporary vector that will store the directions
	//corresponding to the keys pressed.
	mDirection=sf::Vector2i(0,0);

	//Find the actual keys pressed.
	sf::Vector2f stick = InputManager::get().getStick();

		//Extend tempDir by the avatars move speed
	stick *= static_cast<float>(mMoveSpeed);
		//Adjust the avatars position by tempDir
	adjustPosition( stick );
	if( stick.x > 0 ||stick.y > 0)
		mDirection=sf::Vector2i(static_cast<int>(stick.x),static_cast<int>(stick.y));
	else
		mDirection=sf::Vector2i(static_cast<int>(stick.x*2),static_cast<int>(stick.y*2));
}

void Player::assignMoveAnimations(SubLevel* subLevel_p) {
	/////////////////////////////////////////////////////////////////
	if( mDirection.x > 0) {
		if( mDirection.y > 0 )
			mCurrentAnimation_p = &mAnimationMap.find("front")->second;
		if( mDirection.y < 0 )
			mCurrentAnimation_p = &mAnimationMap.find("right")->second;
		if( mDirection.y == 0 )
			mCurrentAnimation_p =  &mAnimationMap.find("frontright")->second;
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

	if( InputManager::get().isSelectDown() && mActionCooldown <= 0 ){
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

void Player::updateCurrentAnimation() {	
	mCurrentAnimation_p->updateAnimation();
}

void Player::setFacingDirection(sf::Vector2i dir){
	if( mDirection.x > 0 || mDirection.y > 0)
		mFacingDir = dir;
}

sf::Vector2i Player::getFacingDirection() const {
	return mFacingDir;
}

void Player::addLightSource(SubLevel* subLevel_p){
	auto ID = WindowManager::get().getNextLightID();

	auto pos = getPosition();
	pos.x += mHitBox.width/2.f;
	pos.y += mHitBox.height/2.f;

	auto& brightness = PlayerSpecs::get().mBrightness;
	auto& distance = PlayerSpecs::get().mLightToDistance.find( mLightLevel )->second;

	subLevel_p->setLightPoint( ID, pos, brightness, distance );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

static void addHitBox( SubLevel* subLevel_p, Player* player, int dmgAmount, const std::string& type) {
	auto& hitBox = player->getHitBox();
	auto& faceDir = player->getFacingDirection();

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
