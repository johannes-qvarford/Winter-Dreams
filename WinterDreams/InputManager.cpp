#include "InputManager.h"
#include "Network.h"

InputManager& InputManager::get(){
	static InputManager inputManager;
	return inputManager;
}

InputManager::InputManager():
	KEY_A(sf::Keyboard::X),
	KEY_B(sf::Keyboard::Z),
	KEY_START(sf::Keyboard::Return),
	KEY_SELECT(sf::Keyboard::Space),
	mIsUnlocked( true )	
{
}

sf::Vector2f InputManager::getStick(){
	sf::Vector2f stick(0,0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		stick += sf::Vector2f(-1, 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		stick += sf::Vector2f(-1, -1);		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		stick += sf::Vector2f(1, -1);	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		stick += sf::Vector2f(1, 1);		
	}
	float length = sqrt(stick.x*stick.x + stick.y*stick.y);
	if( abs(length) > 0 ){
		stick = sf::Vector2f(stick.x/length, stick.y/length);
	}
	return mIsUnlocked ? stick : sf::Vector2f(0,0);
}

bool InputManager::isADown(){
	return mIsUnlocked ? (sf::Keyboard::isKeyPressed(KEY_A)) : false;
}

bool InputManager::isBDown(){
	return mIsUnlocked ? (sf::Keyboard::isKeyPressed(KEY_B)) : false;
}

bool InputManager::isStartDown(){
	return mIsUnlocked ? (sf::Keyboard::isKeyPressed(KEY_START)) : false;
}

bool InputManager::isSelectDown(){
	return mIsUnlocked ? (sf::Keyboard::isKeyPressed(KEY_SELECT)) : false;
}

void InputManager::setKeyBinding(std::string binding, sf::Keyboard::Key key){
	if (binding == "KEY_A") {KEY_A = key;}
	else if (binding == "KEY_B") {KEY_B = key;}
	else if (binding == "KEY_START") {KEY_START = key;}
	else if (binding == "KEY_SELECT") {KEY_SELECT = key;}
}

void InputManager::lockInput() {
	mIsUnlocked = false;
}

void InputManager::unlockInput() {
	mIsUnlocked = true;
}