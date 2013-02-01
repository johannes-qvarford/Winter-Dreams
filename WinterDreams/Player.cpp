#include "Player.h"

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
////////////////////////////////////////////////////////////////////////////////
struct AnimSpecs{
	AnimSpecs(	const std::string animName,
				const std::string fileName, 
				int spriteWidth, 
				int spriteHeight, 
				int numberOfSprites, 
				int framesPerSprite,
				int xOrigin,
				int yOrigin	) :
		mWidth  ( spriteWidth ),
		mHeight ( spriteHeight ),
		mNrOfSprites( numberOfSprites ),
		mFramesPerSprite ( framesPerSprite ),
		mAnimName ( animName ),
		mFileName ( fileName ),
		mXOrigin (xOrigin ),
		mYOrigin (yOrigin )
		{ }

	int mWidth, mHeight, mNrOfSprites;
	int mFramesPerSprite, mXOrigin, mYOrigin;
	std::string mFileName, mAnimName;
};
////////////////////////////////////////////////////////////////////////////////
class PlayerSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PlayerSpecs& get();
	
	bool mEnabled;
	int mLightLevel;
	float mMoveSpeed;
	std::list<AnimSpecs> mAnimSpecLits;

private:
	PlayerSpecs();							//Singleton-pattern
	PlayerSpecs(const PlayerSpecs& p);		//No copies
	PlayerSpecs& operator=(PlayerSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////

Player::Player(sf::FloatRect initialPosition) :
	GraphicalEntity( PlayerSpecs::get().mEnabled ),
	mInventory (Inventory() ),
	mMoveSpeed( PlayerSpecs::get().mMoveSpeed ),
	mMovementMode(NORMAL),
	mLightLevel( PlayerSpecs::get().mLightLevel ),
	mHitBox( initialPosition.left, initialPosition.top, X_STEP , -Y_STEP ) //All hitbox heights are now inverted, ask Johannes.
{
	using namespace std;
	auto& p = PlayerSpecs::get();
	for( auto iter = p.mAnimSpecLits.begin(), end = p.mAnimSpecLits.end(); iter != end; ++iter){
		auto w =	iter->mWidth;
		auto h =	iter->mHeight;
		auto yO =	iter->mYOrigin;
		auto xO =	iter->mXOrigin;
		auto nos =	iter->mNrOfSprites;
		auto fps =	iter->mFramesPerSprite;
		auto file = iter->mFileName;
		auto name = iter->mAnimName;

		Animation anim(FS_DIR_OBJECTANIMATIONS +"player/"+ file , w, h, nos, fps, xO, yO);
		mAnimationMap.insert( pair<string, Animation>( name , anim ) );
	}

	mCurrentAnimation_p = &mAnimationMap.begin()->second;
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
		gameState_p->addGraphicalEntity(std::shared_ptr<DamageHitBox>( new DamageHitBox(mHitBox, 2, DamageHitBox::PICKAXE ) ) );
	}
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
	std::cout<< mHitBox.left <<"," <<mHitBox.top <<" ";
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
PlayerSpecs::PlayerSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& player = obj.get_child( "objects.player" );

	mLightLevel = player.get<int>( "startlight" );
	mMoveSpeed = player.get<float>( "walkspeed" );
	mEnabled = player.get<bool>( "startenabled" );
	
	auto& animations = player.get_child( "animations" );
	for(auto iter = animations.begin(), end = animations.end(); iter != end; ++iter){
		if(iter->second.get<std::string>("filename") != "null"){
			auto w =	iter->second.get<int>	("width");
			auto h =	iter->second.get<int>	("height");
			auto yO =	iter->second.get<int>	("yorigin");
			auto xO =	iter->second.get<int>	("xorigin");
			auto nos =	iter->second.get<int>	("numberofsprites");
			auto fps =	iter->second.get<int>	("framespersprite");
			auto file = iter->second.get<std::string>	("filename");
			auto name = iter->first;
		
			AnimSpecs as(name, file, w, h, nos, fps, xO, yO);
			mAnimSpecLits.emplace_back( as );
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
PlayerSpecs& PlayerSpecs::get() { 
	static PlayerSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////