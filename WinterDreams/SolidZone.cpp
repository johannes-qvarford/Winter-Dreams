#include "SolidZone.h"
#ifdef DEBUG_SOLIDZONE
#include "GameToScreen.h"
#include "FileStructure.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#endif
#include "Player.h"

static const char* SolidZone_IMAGE_FILENAME_1 = "collision128x64/flat64.png";
static const char* SolidZone_IMAGE_FILENAME_2 = "collision256x128/flat128.png";
static const char* SolidZone_IMAGE_FILENAME_3 = "collision512x256/flat256.png";
static const float STEP = 35.77708763999664f;

SolidZone::SolidZone(sf::Rect<float> HitBox, bool startsEnabled):
	CollisionZone(startsEnabled, HitBox, false)
{
#ifdef DEBUG_SOLIDZONE
	auto& hitBox = getHitBox();
	mSprite.setPosition(hitBox.left, hitBox.top);

	static auto mTexture1 = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + SolidZone_IMAGE_FILENAME_1);
	static auto mTexture2 = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + SolidZone_IMAGE_FILENAME_2);
	static auto mTexture3 = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + SolidZone_IMAGE_FILENAME_3);
	
	if(abs(hitBox.width - STEP) < 1 && abs(hitBox.height - (-STEP)) < 1) {
		mSprite.setOrigin(0, 32);
		mSprite.setTexture(*mTexture1.get() );
		mSprite.setTextureRect(sf::IntRect(0, 0, 64, 48));
	}
	else if(abs(hitBox.width - STEP * 2) < 1 && abs(hitBox.height - (-STEP * 2) < 1)) {
		mSprite.setOrigin(0, 48);		
		mSprite.setTexture(*mTexture2.get() );
		mSprite.setTextureRect(sf::IntRect(0, 0, 128, 80));
	}
	else {
		mSprite.setOrigin(0, 80);
		mSprite.setTexture(*mTexture3.get() );
		mSprite.setTextureRect(sf::IntRect(0, 0, 256, 144));
	}
	mSprite.setPosition( GAME_TO_SCREEN * mSprite.getPosition() );
#endif
}

void SolidZone::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){
	
	if (getEnabled() == false)
		return;
	
	CollisionZone::onCollision(pe, intersection);

	Player* pl = dynamic_cast<Player*>(pe);
	if(pl == nullptr)
		return;

	sf::Rect<float>& hitBox = pe->getHitBox();
	sf::Vector2i& direction = pl->getDirection();
	sf::Rect<float>& myHitBox = getHitBox();
/*
// Specialfallan då höjden och bredden på intersection är lika stora (ish)
*/
	if (fabs(intersection.width - intersection.height) < 0.1f){
		/*
		// för att kunna skilja mellan de olika fallen används riktningen
		// Fall 1, då man åkar pixelperfekt upp i skärmkordinater
		*/
		if (direction.x < 0 && direction.y < 0){
//			hitBox.left -= intersection.width;
			//* 0.5 kommer ifrån att den måste fortfarande ha intersection för att fungera
			hitBox.top += intersection.height * 0.5f;
		}
		/*
		// Fall 2, då man åker pixelperfekt vänster i skärmkordinater
		*/
		else if (direction.x < 0 && direction.y > 0){
//			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5f;
		}
		/*
		// Fall 3, då man åker pixelperfekt höger i skärmkordinater
		*/
		else if (direction.x > 0 && direction.y < 0){
//			hitBox.left += intersection.width;
			//* 0.5 kommer ifrån att den måste fortfarande ha intersection för att fungera
			hitBox.top -= intersection.height * 0.5f;
		}
		/*
		// Fall 4, då man åker pixelperfekt neråt i skärmkordinater
		*/
		else if (direction.x > 0 && direction.y > 0){
//			hitBox.left += intersection.width;
			//* 0.5 kommer ifrån att den måste fortfarande ha intersection för att fungera
			hitBox.top += intersection.height * 0.5f;
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
	
	auto window_p = WindowManager::get().getWindow();
	if(!getEnabled())
		return;

	sf::Vertex vertices[] =
	{
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top), sf::Color::Blue, sf::Vector2f( 0,  0)),
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top + mHitBox.height), sf::Color::Blue, sf::Vector2f( 0, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top + mHitBox.height), sf::Color::Blue, sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top), sf::Color::Blue, sf::Vector2f(10,  0))
	};
	
	auto& window = *WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	//translate to screen coordinates
	states.transform *= GAME_TO_SCREEN;

	window.draw(vertices, 4, sf::Quads, states);
	//	window_p->draw(mSprite);
#endif
}
