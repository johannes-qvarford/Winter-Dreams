#include "Player.h"
#include "GameToScreen.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"

Player::Player(sf::Vector2f initialPosition) :
	mInventory (Inventory() ),
	mMovementMode(NORMAL),
	mHealth( 5 ),
	mHitBox( sf::FloatRect(initialPosition.x, initialPosition.y, X_STEP , -Y_STEP) )//All hitbox heights are now inverted, ask Johannes.
{
	mAnimationMap.insert( std::pair<std::string, Animation>("placeholder", Animation(FS_DIR_OBJECTANIMATIONS + "player/placeholder.png", 64, 64, 1, 1) ) );
	mCurrentAnimation_p = &mAnimationMap.find("placeholder")->second;
}

Player::~Player() {}

void Player::update(GameState* gameState_p, int milliseconds){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		Player::adjustPosition(sf::Vector2f(-10,0));
		//Set proper animation
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		Player::adjustPosition(sf::Vector2f(0,-10));
		//Set proper animation
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		Player::adjustPosition(sf::Vector2f(10,0));
		//Set proper animation
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		Player::adjustPosition(sf::Vector2f(0,10));
		//Set proper animation
	}
	//auto& window = *WindowManager::get().getWindow();
	//auto pos = GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top);       
	//pos -= sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	//auto size = sf::Vector2f(window.getSize().x, window.getSize().y );
	//auto rect = sf::FloatRect( pos, size );
	//auto view = sf::View( rect );
	//window.setView( view );
}

void Player::drawSelf(){
	auto& sprite = mCurrentAnimation_p->getCurrentSprite();
	sprite.setOrigin(0 , 48);
	sprite.setPosition( GAME_TO_SCREEN * getPosition() );
	WindowManager::get().getWindow()->draw( sprite ,*WindowManager::get().getStates());


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

int Player::getCurrentHealth() const{
	return mHealth;
}

void Player::setCurrentHealth(const int health){
	mHealth=health;
}

void Player::adjustCurrentHealth(const int healthAdjustment){
	mHealth+=healthAdjustment;
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