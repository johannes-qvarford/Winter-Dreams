#include "Wall.h"
#include "Player.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

static const char* WALL_IMAGE_FILENAME_1 = "Images/cube64.png";
static const char* WALL_IMAGE_FILENAME_2 = "Images/cube128.png";
static const char* WALL_IMAGE_FILENAME_3 = "Images/cube256.png";
static const float STEP = 35.77708763999664f;

Wall::Wall(float x, float y, float width, float height):
mHitBox(x, y, width, height)
	
{
#ifdef DEBUG_WALL
	mSprite.setPosition(mHitBox.left, mHitBox.top);
	
	if(width < STEP + 1) {
		mSprite.setOrigin(32, 32);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_1);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	}
	else if(width < (STEP * 2) + 1) {
		mSprite.setOrigin(64, 64);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_2);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
	}
	else {
		mSprite.setOrigin(128, 128);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_3);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
	}
	
#endif
}

void Wall::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){

	Player* pl = dynamic_cast<Player*>(pe);
	if(pl == nullptr)
		return;

	sf::Rect<float>& hitBox = pe->getHitBox();
	sf::Vector2f& direction = pe->getDirection();

	if (intersection.width == intersection.height){
		if (direction == sf::Vector2f(-10, -10)){
			hitBox.left -= intersection.width;
			hitBox.top += intersection.height * 0.5;
		}
		else if (direction == sf::Vector2f(-10, 10)){
			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5;
		}

	}
	


	if (intersection.width > intersection.height){
		if (hitBox.top > mHitBox.top)
			hitBox.top += intersection.height;
		else
			hitBox.top -= intersection.height;
	}
	else {
		if (hitBox.left > mHitBox.left)
			hitBox.left += intersection.width;
		else
			hitBox.left -= intersection.width;
	}
}

/*
// Har tagit bort const då funktioner returnerar en referens
*/

sf::Rect<float>& Wall::getHitBox(){
	return mHitBox;

}

void Wall::update(GameState* gameState, int milliseconds){
}

void Wall::drawSelf(){

#ifdef DEBUG_WALL
	auto& mgr = WindowManager::get();
	auto window_p = mgr.getWindow();

	auto tempSprite = mSprite;
	tempSprite.setPosition(GAME_TO_SCREEN * tempSprite.getPosition());

	window_p->draw(tempSprite);
#endif
}

sf::Vector2f& Wall::getDirection(){
	return sf::Vector2f(10, 0);

}