#include "Crystal.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "DamageHitBox.h"
#include "FileStructure.h"

Crystal::Crystal( const sf::FloatRect& position ) : 
	mHitBox ( sf::FloatRect( position.left, position.top, position.width, -position.height) ),
	mHP		( 6 )

{
	mAnimationMap.insert(std::pair<std::string, Animation>("placeholder", Animation(FS_DIR_OBJECTANIMATIONS + "crystal 128x64/placeholder_crystal.png", 48, 0, 1, 1) ) );
	mCurrentAnimation = &mAnimationMap.find("placeholder")->second;
}

Crystal::~Crystal() {}

void Crystal::update(GameState* gameState_p, int milliseconds) {}

void Crystal::drawSelf() {
	auto& windowManager = WindowManager::get();

	auto& sprite = mCurrentAnimation->getCurrentSprite();
	sprite.setOrigin( 0, 48);

	auto pos = sf::Vector2f(mHitBox.left, mHitBox.top);
	sprite.setPosition( GAME_TO_SCREEN * pos );

	windowManager.getWindow()->draw( sprite, *windowManager.getStates() );
}

void Crystal::adjustHealth(int adjustment){
	mHP += adjustment;
}

sf::FloatRect& Crystal::getHitBox() {
	return mHitBox;
}

void Crystal::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection) {
	Wall::onCollision( entityCollidedWith_p, intersection );

	if( dynamic_cast<DamageHitBox*>( entityCollidedWith_p ) ) {
		auto dmgHitBox = dynamic_cast<DamageHitBox*>( entityCollidedWith_p );
		dmgHitBox->setActive(false);
		//PLAY SOUND
	}
}

