#include "Wall.h"
#include "Player.h"
#include "ResourceManager.h"
#include "WindowManager.h"

static const WALL_IMAGE_FILENAME = "Basic 64x32 tileset.png";
static const float STEP = 35.77708763999664;

Wall::Wall(float x, float y, float width, float height):
mHitBox(x, y, width, height)
	
{
#ifdef DEBUG_WALL
	mTexture = ResourceMananger::get().getTexture(WALL_IMAGE_FILENAME);
	mSprite.setTexture(*mTexture);
	if(width < STEP + 1)
		mSprite.setOrigin(32, 0);
	else if(width < (STEP * 2) + 1)
		mSprite.setOrigin(64, 0);
	else
		mSprite.setOrigin(128, 0);
#endif
}

void Wall::onCollision(PhysicalEntity* pe){

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

const sf::Rect<float>& Wall::getHitBox(){
	return mHitBox;

}

void update(GameState* gameState, int milliseconds){


}

void Wall::drawSelf() const{

#ifdef DEBUG_WALL
	auto& mgr = WindowManager::get();
	auto window_p = mgr.getWindow();
	auto renderStates_p = mgr.getRenderStates();

	window_p->draw(mSprite, *renderStates_p);
#endif
}