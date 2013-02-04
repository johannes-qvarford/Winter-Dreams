<<<<<<< HEAD
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
// Specialfallan då höjden och bredden på intersection är lika stora (ish)
*/
	if (fabs(intersection.width - intersection.height) < 0.1f){
		/*
		// för att kunna skilja mellan de olika fallen används riktningen
		// Fall 1, då man går emot väggen som är från botten vänster till toppen höger
		*/
		if (direction.y < 0 && direction.x < 0){
			hitBox.left -= intersection.width;
			//* 0.5 kommer ifrån att den måste fortfarande ha intersection för att fungera
			hitBox.top += intersection.height * 0.5f;
		}
		/*
		// Fall 2, då man går mot väggen som är från botten höger till toppen vänster
		*/
		else if (direction.x < 0 && direction.y > 0){
			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5f;
		}

	}
	/*
	// Alla de allmänna fallen, där kollar vi om bredden är större än höjden
	*/
	else if (intersection.width > intersection.height){
		/*
		// Sedan kollar vi om y-värdet är större på player än på väggen
		// Höjden på player måste adderas för det nya koordinatsystemet
		// är players y-värde större, adddera intersections-höjden
		// annars minska med intersections-höjd
		*/
		if (hitBox.top + hitBox.height > myHitBox.top + myHitBox.height)
			hitBox.top += intersection.height;
		else
			hitBox.top -= intersection.height;
	}
	/*
	// om inte bredden är större än höjden eller höjden och bredden är lika så är höjden större än bredden
	// om players x-värde är större än väggens x-värde, addera players x-värde med intersection-bredden
	// annars minska players x-värde med intersection-bredden
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
=======
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
// Specialfallan då höjden och bredden på intersection är lika stora (ish)
*/
	if (fabs(intersection.width - intersection.height) < 0.1f){
		/*
		// för att kunna skilja mellan de olika fallen används riktningen
		// Fall 1, då man går emot väggen som är från botten vänster till toppen höger
		*/
		if (direction.y < 0 && direction.x < 0){
			hitBox.left -= intersection.width;
			//* 0.5 kommer ifrån att den måste fortfarande ha intersection för att fungera
			hitBox.top += intersection.height * 0.5f;
		}
		/*
		// Fall 2, då man går mot väggen som är från botten höger till toppen vänster
		*/
		else if (direction.x < 0 && direction.y > 0){
			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5f;
		}

	}
	/*
	// Alla de allmänna fallen, där kollar vi om bredden är större än höjden
	*/
	else if (intersection.width > intersection.height){
		/*
		// Sedan kollar vi om y-värdet är större på player än på väggen
		// Höjden på player måste adderas för det nya koordinatsystemet
		// är players y-värde större, adddera intersections-höjden
		// annars minska med intersections-höjd
		*/
		if (hitBox.top + hitBox.height > myHitBox.top + myHitBox.height)
			hitBox.top += intersection.height;
		else
			hitBox.top -= intersection.height;
	}
	/*
	// om inte bredden är större än höjden eller höjden och bredden är lika så är höjden större än bredden
	// om players x-värde är större än väggens x-värde, addera players x-värde med intersection-bredden
	// annars minska players x-värde med intersection-bredden
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
>>>>>>> master
