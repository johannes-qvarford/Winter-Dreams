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

SolidZone::SolidZone(const sf::FloatRect& hitBox, bool startsEnabled):
	Entity(startsEnabled),
	BaseHitBoxHaveable(hitBox)
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

void SolidZone::update(SubLevel* subLevel_p) {
}

void SolidZone::onCollision(Collidable* col_p, const sf::Rect<float>& intersection){
	
	if (getEnabled() == false)
		return;
	
	Player* pl_p = dynamic_cast<Player*>(col_p);
	if(pl_p == nullptr)
		return;

	sf::Rect<float>& hitBox = pl_p->getHitBox();
	sf::Vector2i direction = pl_p->getDirection();
/*
// Specialfallan d� h�jden och bredden p� intersection �r lika stora (ish)
*/
	if (fabs(intersection.width - intersection.height) < 0.1f){
		/*
		// f�r att kunna skilja mellan de olika fallen anv�nds riktningen
		// Fall 1, d� man �kar pixelperfekt upp i sk�rmkordinater
		*/
		if (direction.x < 0 && direction.y < 0){
//			hitBox.left -= intersection.width;
			//* 0.5 kommer ifr�n att den m�ste fortfarande ha intersection f�r att fungera
			hitBox.top += intersection.height * 0.5f;
		}
		/*
		// Fall 2, d� man �ker pixelperfekt v�nster i sk�rmkordinater
		*/
		else if (direction.x < 0 && direction.y > 0){
//			hitBox.left -= intersection.width;
			hitBox.top -= intersection.height * 0.5f;
		}
		/*
		// Fall 3, d� man �ker pixelperfekt h�ger i sk�rmkordinater
		*/
		else if (direction.x > 0 && direction.y < 0){
//			hitBox.left += intersection.width;
			//* 0.5 kommer ifr�n att den m�ste fortfarande ha intersection f�r att fungera
			hitBox.top -= intersection.height * 0.5f;
		}
		/*
		// Fall 4, d� man �ker pixelperfekt ner�t i sk�rmkordinater
		*/
		else if (direction.x > 0 && direction.y > 0){
//			hitBox.left += intersection.width;
			//* 0.5 kommer ifr�n att den m�ste fortfarande ha intersection f�r att fungera
			hitBox.top += intersection.height * 0.5f;
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

#ifdef DEBUG_SOLIDZONE
void SolidZone::draw(){

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
}

#endif
