#include "Crystal.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "DamageHitBox.h"
#include "FileStructure.h"

Crystal::Crystal( const sf::FloatRect& position, bool startEnabled ) : 
	GraphicalEntity( startEnabled ),
//	mSolidZone( SolidZone( position, startEnabled ) ),
	mHP		( 6 )

{
	mAnimationMap.insert(std::pair<std::string, Animation>("placeholder", Animation(FS_DIR_OBJECTANIMATIONS + "crystal 128x64/placeholder_crystal.png", 48, 0, 1, 1) ) );
	mCurrentAnimation = &mAnimationMap.find("placeholder")->second;
}

Crystal::~Crystal() { }

void Crystal::update(GameState* gameState_p) {}

void Crystal::drawSelf() {
	auto& windowManager = WindowManager::get();

	auto& sprite = mCurrentAnimation->getCurrentSprite();
	sprite.setOrigin( 0, 48);

	auto& box = mSolidZone->getHitBox();
	auto pos = sf::Vector2f(box.left, box.top);
	sprite.setPosition( GAME_TO_SCREEN * pos );

	windowManager.getWindow()->draw( sprite, *windowManager.getStates() );
}

void Crystal::adjustHealth(int adjustment){
	mHP += adjustment;
}

sf::FloatRect& Crystal::getHitBox() {
	return mSolidZone->getHitBox();
}

void Crystal::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection) {
	mSolidZone.onCollision( entityCollidedWith_p, intersection );

	if( dynamic_cast<DamageHitBox*>( entityCollidedWith_p ) ) {
		auto dmgHitBox = dynamic_cast<DamageHitBox*>( entityCollidedWith_p );
		dmgHitBox->setAlive(false);
		//PLAY SOUND
	}
}

