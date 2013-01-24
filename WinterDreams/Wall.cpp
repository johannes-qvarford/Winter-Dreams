#include "Wall.h"
#include "Player.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

static const char* WALL_IMAGE_FILENAME_1 = "Images/Basic 64x32 tileset.png";
static const char* WALL_IMAGE_FILENAME_2 = "Images/Basic 128x64 tileset.png";
static const char* WALL_IMAGE_FILENAME_3 = "Images/Basic 256x128 tileset.png";
static const float STEP = 35.77708763999664f;

Wall::Wall(float x, float y, float width, float height):
mHitBox(x, y, width, height)
	
{
#ifdef DEBUG_WALL
	
	mSprite.setTexture(*mTexture);
	if(width < STEP + 1) {
		mSprite.setOrigin(32, 0);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_1);
	}
	else if(width < (STEP * 2) + 1) {
		mSprite.setOrigin(64, 0);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_2);
	}
	else {
		mSprite.setOrigin(128, 0);
		mTexture = ResourceManager::get().getTexture(WALL_IMAGE_FILENAME_3);
	}
#endif
}

void Wall::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){

	Player* pl = dynamic_cast<Player*>(pe);
	if(pl == nullptr)
		return;

	sf::Rect<float>& hitBox = pe->getHitBox();
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
	auto renderStates_p = mgr.getStates();

	window_p->draw(mSprite, *renderStates_p);
#endif
}