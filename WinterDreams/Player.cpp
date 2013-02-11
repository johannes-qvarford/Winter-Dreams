#include "Player.h"

#include "Crystal.h"
#include "DamageHitBox.h"
#include "GameState.h"
#include "GameToScreen.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "PropertyManager.h"
#include <list>
#include <cmath>
#include <iostream>

class PlayerSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PlayerSpecs& get();


	float mMoveSpeed;
	std::list<AnimationSpecs> mAnimSpecList;

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
	
	AnimationSpecs::parse( player, mAnimSpecList );
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
	mInventory (Inventory() ),
	mMoveSpeed( PlayerSpecs::get().mMoveSpeed ),
	mMovementMode(NORMAL),
	mLightLevel( lightLevel ),
	mHitBox( initialPosition.left, initialPosition.top, X_STEP , -Y_STEP ) //All hitbox heights are now inverted, ask Johannes.
{
	using namespace std;
	auto& p = PlayerSpecs::get();
	//construct the animation map
	Animation::makeAnimations("player/", p.mAnimSpecList, &mAnimationMap);
	mCurrentAnimation_p = &mAnimationMap.begin()->second;

	mInventory.giveItem("pickaxe", 1);
	mInventory.giveItem("wheel", 1);
	mInventory.giveItem("shoes", 1);
}

Player::~Player() {}

void Player::update(GameState* gameState_p){

	updateMovement(gameState_p);

	updateAnimations(gameState_p);

	updateActions(gameState_p);
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
	mLightLevel=lightLevel;
}

void Player::adjustCurrentLightLevel(const int lightLevelAdjustment){
	mLightLevel+=lightLevelAdjustment;
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

void Player::updateMovement(GameState* gameState_p) {
	
	//Create a temporary vector that will store the directions
	//corresponding to the keys pressed.
	sf::Vector2f tempDir(0,0);
	mDirection = sf::Vector2i(0, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		--tempDir.x;
		++tempDir.y;
		mDirection += sf::Vector2i(-1, 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		--tempDir.x;
		--tempDir.y;
		mDirection += sf::Vector2i(-1, -1);		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		++tempDir.x;
		--tempDir.y;
		mDirection += sf::Vector2i(1, -1);	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		++tempDir.x;
		++tempDir.y;
		mDirection += sf::Vector2i(1, 1);		
	}

	/////////////////////////////////////////////////////////////////
	//Get the length of tempDir
	auto tempLenght = std::sqrt(tempDir.x * tempDir.x + tempDir.y * tempDir.y);
		//Normalize tempDir if it's length is greater then 0
	if( abs(tempLenght) > 0 ){
		tempDir.x = tempDir.x / tempLenght;
		tempDir.y = tempDir.y / tempLenght;
	}
		//Extend tempDir by the avatars move speed
	tempDir *= static_cast<float>(mMoveSpeed);
		//Adjust the avatars position by tempDir
	adjustPosition( tempDir );
}

void Player::updateAnimations(GameState* gameState_p) {
	/////////////////////////////////////////////////////////////////
	if( mDirection.x >= 1) {
		if( mDirection.y == 1 )
			mCurrentAnimation_p = &mAnimationMap.find("front")->second;
		if( mDirection.y == -1 )
			mCurrentAnimation_p = &mAnimationMap.find("right")->second;
		if( mDirection.y == 0 )
			mCurrentAnimation_p =  &mAnimationMap.find("frontright")->second;
	}
	if( mDirection.x <= -1) {
		if( mDirection.y == 1 )
			mCurrentAnimation_p = &mAnimationMap.find("left")->second;
		if( mDirection.y == -1 )
			mCurrentAnimation_p = &mAnimationMap.find("back")->second;
		if( mDirection.y == 0 )
			mCurrentAnimation_p = &mAnimationMap.find("backleft")->second;
	}
	if( mDirection.x == 0 ) {
		if( mDirection.y >= 1 )
			mCurrentAnimation_p = &mAnimationMap.find("frontleft")->second;
		if( mDirection.y <= -1 )
			mCurrentAnimation_p = &mAnimationMap.find("backright")->second;
		if( mDirection.y == 0 )
				//If the character did not move, idle.
			mCurrentAnimation_p->resetAnimation();
	}

	mCurrentAnimation_p->updateAnimation();
}

void Player::updateActions(GameState* gameState_p) {
	--mActionCooldown;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && mActionCooldown <= 0) {
		auto rect = mHitBox;
		rect.left += mHitBox.width;
		rect.top += mHitBox.height;
		gameState_p->addGraphicalEntity(std::shared_ptr<DamageHitBox>( new DamageHitBox(rect, 2, DamageHitBox::PICKAXE ) ) );
		mActionCooldown = 60;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && mActionCooldown <= 0) {
		auto rect = mHitBox;
		rect.left += 40;
		rect.top += 40;
		gameState_p->addGraphicalEntity(std::shared_ptr<Crystal>( new Crystal(rect, true) ) );

		mActionCooldown = 60;
	}
}
