#include "Player.h"

#include "DamageHitBox.h"
#include "GameState.h"
#include "GameToScreen.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "PropertyManager.h"
#include <cmath>


Player::Player(sf::FloatRect initialPosition) :
	GraphicalEntity( true ),
	mInventory (Inventory() ),
	mMovementMode(NORMAL),
	mLightLevel( 5 ),
	mHitBox( initialPosition.left, initialPosition.top, X_STEP , -Y_STEP ) //All hitbox heights are now inverted, ask Johannes.
{
	mAnimationMap.insert( std::pair<std::string, Animation> ("placeholder", Animation(FS_DIR_OBJECTANIMATIONS + "player/placeholder.png", 64, 64, 3, 10) ) );
	mCurrentAnimation_p = &mAnimationMap.find("placeholder")->second;
}

Player::~Player() {}

void Player::update(GameState* gameState_p){

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		gameState_p->addGraphicalEntity(std::shared_ptr<DamageHitBox>( new DamageHitBox(2, mHitBox, DamageHitBox::PICKAXE ) ) );
	}
		//Get the length of tempDir
	auto tempLenght = std::sqrt(tempDir.x * tempDir.x + tempDir.y * tempDir.y);
		//Normalize tempDir if it's length is greater then 0
	if( abs(tempLenght) > 0 ){
		tempDir.x = tempDir.x / tempLenght;
		tempDir.y = tempDir.y / tempLenght;
	}
		//Extend tempDir by the avatars move speed
	tempDir *= MOVE_SPEED;
		//Adjust the avatars position by tempDir
	adjustPosition( tempDir );	
}

void Player::drawSelf(){
		//Get the current animation's sprite
	auto& sprite = mCurrentAnimation_p->getCurrentSprite();

	sprite.setOrigin(0 , 48);
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


////////////////////////////////////////////////////////////////////////////////
struct AnimSpecs{
	AnimSpecs(	const std::string filePath, 
				unsigned int spriteWidth, 
				unsigned int spriteHeight, 
				unsigned int numberOfSprites, 
				unsigned int framesPerSprite,
				unsigned int xOrigin,
				unsigned int yOrigin	) :
		mWidth  ( spriteWidth ),
		mHeight ( spriteHeight ),
		mNrOfSprites( numberOfSprites ),
		mFramesPerSprite ( framesPerSprite ),
		mFilePath ( filePath ),
		mXOrigin (xOrigin ),
		mYOrigin (yOrigin )
		{ }

	unsigned int mWidth, mHeight, mNrOfSprites;
	unsigned int mFramesPerSprite, mXOrigin, mYOrigin;
	std::string mFilePath;
};

class PlayerSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PlayerSpecs& get();
	
	float mMoveSpeed;
	int mNumberOfAnimations;	
	std::map<std::string, AnimSpecs> mAnimSpecsMap;

private:
	PlayerSpecs() {}						//Singleton-pattern
	PlayerSpecs(const PlayerSpecs& p);		//No copies
	PlayerSpecs& operator=(PlayerSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
PlayerSpecs::PlayerSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& player = obj.get_child( "objects.player" );

	mMoveSpeed = player.get<float>( "walkspeed" );
	mNumberOfAnimations = player.get<int>( "numberofanimations" );
	
	auto& animations = player.get_child( "animations" );
	auto defaultWidth = animations.get<int>( "width" );
	for(auto iter = animations.begin(), end = animations.end(); iter != end; ++iter){
		
		/////////////////////////////////////
	}
}

PlayerSpecs& PlayerSpecs::get() { 
	static PlayerSpecs p;
	return p;
}