#include "Player.h"
#include "WindowManager.h"

Player::Player(sf::Vector2f initialPosition){
	mPosition=initialPosition;
	mInventory=Inventory();
}

void Player::update(GameState* gameState_p, int milliseconds){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		Player::adjustPosition(sf::Vector2f(4,0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		Player::adjustPosition(sf::Vector2f(0,-4));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		Player::adjustPosition(sf::Vector2f(-4,0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		Player::adjustPosition(sf::Vector2f(0,4));
	}
}

void Player::drawSelf(){
	WindowManager::get().getWindow()->draw(mCurrentAnimation.getCurrentSprite(),*WindowManager::get().getStates());
}

const sf::FloatRect& Player::getHitBox() const{
	return mHitBox;
}

void Player::onCollision(PhysicalEntity* entityCollidedWith_p){

}

const sf::Vector2f& Player::getPosition() const{
	return mPosition;
}

void Player::setPosition(const sf::Vector2f& position){
	mPosition=position;
}

void Player::adjustPosition(const sf::Vector2f& positionAdjustment){
	mPosition+=positionAdjustment;
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