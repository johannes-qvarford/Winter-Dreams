#include "SolidZone.h"
#ifdef DEBUG_SOLIDZONE
#include "GameToScreen.h"
#include "FileStructure.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#endif
#include "Player.h"

static const char* SolidZone_IMAGE_FILENAME_1 = "collision128x64/placeholder.png";
static const char* SolidZone_IMAGE_FILENAME_2 = "collision256x128/placeholder.png";
static const char* SolidZone_IMAGE_FILENAME_3 = "collision512x256/placeholder.png";
static const float STEP = 35.77708763999664f;

SolidZone::SolidZone(sf::Rect<float> HitBox, bool startsEnabled):
	CollisionZone(startsEnabled, HitBox, false)

{
#ifdef DEBUG_SOLIDZONE
	mSprite.setPosition(mHitBox.left, mHitBox.top);
	
	if(mHitBox.width < STEP + 1) {
		mSprite.setOrigin(0, 48);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS +  SolidZone_IMAGE_FILENAME_1);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	}
	else if(mHitBox.width < (STEP * 2) + 1) {
		mSprite.setOrigin(0, 96);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + SolidZone_IMAGE_FILENAME_2);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
	}
	else {
		mSprite.setOrigin(0, 192);
		mTexture = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + SolidZone_IMAGE_FILENAME_3);
		mSprite.setTexture(*mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
	}
	
#endif
}

void SolidZone::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){
	CollisionZone::onCollision(pe, intersection);

	if (getEnabled() == false)
		return;

	Player* pl = dynamic_cast<Player*>(pe);
	if(pl == nullptr)
		return;

	sf::Rect<float>& hitBox = pe->getHitBox();
	sf::Vector2i& direction = pe->getDirection();
	sf::Rect<float>& myHitBox = getHitBox();
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
		if (hitBox.top + hitBox.height > myHitBox.top + myHitBox.height)
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
		if (hitBox.left > myHitBox.left)
			hitBox.left += intersection.width;
		else
			hitBox.left -= intersection.width;
	}
}

void SolidZone::drawSelf(){
	#ifdef DEBUG_SOLIDZONE
	auto& mgr = WindowManager::get();
	auto window_p = mgr.getWindow();

	auto tempSprite = mSprite;
	tempSprite.setPosition(GAME_TO_SCREEN * tempSprite.getPosition());

	window_p->draw(tempSprite);
#endif
}
