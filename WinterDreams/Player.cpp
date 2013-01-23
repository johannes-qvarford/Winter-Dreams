#include "Player.h"

Player::Player(sf::Vector2f initialPosition){
	mPosition=initialPosition;
	mInventory=Inventory();
}

void Player::update(GameState* gameState_p, int milliseconds){

}

void Player::drawSelf(){

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