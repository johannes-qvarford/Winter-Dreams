#include "Wall.h"
#include "Player.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "FileStructure.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

static const char* WALL_IMAGE_FILENAME_1 = "collision128x64/placeholder.png";
static const char* WALL_IMAGE_FILENAME_2 = "collision256x128/placeholder.png";
static const char* WALL_IMAGE_FILENAME_3 = "collision512x256/placeholder.png";
static const float STEP = 35.77708763999664f;

Wall::Wall(float x, float y, float width, float height):
mHitBox(x, y, width, height)
	
{
#ifdef DEBUG_WALL
	mSprite.setPosition(mHitBox.left, mHitBox.top);
	
	if(width < STEP + 1) {
		mSprite.setOrigin(0, 48);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS +  WALL_IMAGE_FILENAME_1);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	}
	else if(width < (STEP * 2) + 1) {
		mSprite.setOrigin(0, 96);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + WALL_IMAGE_FILENAME_2);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
	}
	else {
		mSprite.setOrigin(0, 192);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + WALL_IMAGE_FILENAME_3);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
	}
	
#endif
}
/*
// Kollisionshanteringen
*/
void Wall::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){

	Player* pl = dynamic_cast<Player*>(pe);
	if(pl == nullptr)
		return;

	sf::Rect<float>& hitBox = pe->getHitBox();
	sf::Vector2i& direction = pe->getDirection();

/*
// Specialfallan d� h�jden och bredden p� intersection �r lika stora (ish)
*/
	if (fabs(intersection.width - intersection.height) < 0.1f){
		/*
		// f�r att kunna skilja mellan de olika fallen anv�nds riktningen
		// Fall 1, d� man g�r emot v�ggen som �r fr�n botten v�nster till toppen h�ger
		*/
		if (direction.y < 0 && direction.x < 0){
			hitBox.left -= intersection.width;
			//* 0.5 kommer ifr�n att den m�ste fortfarande ha intersection f�r att fungera
			hitBox.top += intersection.height * 0.5;
		}
		/*
		// Fall 2, d� man g�r mot v�ggen som �r fr�n botten h�ger till toppen v�nster
		*/
		else if (direction.x < 0 && direction.y > 0){
			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5;
		}

	}
	/*
	// Alla de allm�nna fallen, d�r kollar vi om bredden �r st�rre �n h�jden
	*/
	else if (intersection.width > intersection.height){
		/*
		// Sedan kollar vi om y-v�rdet �r st�rre p� player �n p� v�ggen
		// H�jden p� player m�ste adderas f�r det nya koordinatsystemet
		// �r players y-v�rde st�rre, adddera intersections-h�jden
		// annars minska med intersections-h�jd
		*/
		if (hitBox.top + hitBox.height > mHitBox.top + mHitBox.height)
			hitBox.top += intersection.height;
		else
			hitBox.top -= intersection.height;
	}
	/*
	// om inte bredden �r st�rre �n h�jden eller h�jden och bredden �r lika s� �r h�jden st�rre �n bredden
	// om players x-v�rde �r st�rre �n v�ggens x-v�rde, addera players x-v�rde med intersection-bredden
	// annars minska players x-v�rde med intersection-bredden
	*/
	else {
		if (hitBox.left > mHitBox.left)
			hitBox.left += intersection.width;
		else
			hitBox.left -= intersection.width;
	}
}

/*
// Har tagit bort const d� funktioner returnerar en referens
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

/*
// Denna funktion anv�nds inte utan �r endast h�r f�r att den m�ste
*/
sf::Vector2i Wall::getDirection(){
	return sf::Vector2i(10, 0);

}