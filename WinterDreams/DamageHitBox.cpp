#include "DamageHitBox.h"
#include "SubLevel.h"
#include "Crystal.h"
#include <SFML\Audio.hpp>

///////////////////////////////////////////////
// /Simply assigns the arguments to the correct member variable.
// /EXCEPTION: mLifeTime is assigned manually. Defines how many updates the hitbox lives.
///////////////////////////////////////////////
DamageHitBox::DamageHitBox(const sf::Rect<float>& hitBox, unsigned int damage, DamageType type) :
	GraphicalEntity( true ),
	mHitBox( hitBox ),
	mDamage( damage ),
	mDamageType( type ),
	mLifeTime(3)
#ifndef SHIPPING
	,
	mAnimation( "DEBUG.png", 64, 64, 1, 1, 0, 48 )
#endif
{ 
#ifndef SHIPPING
	mAnimation.setPosition( GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top) );
#endif
}

DamageHitBox::~DamageHitBox() { }
	///////////////////////////////////////////////
	// /Returns the hit box of the damage zone
	///////////////////////////////////////////////
sf::FloatRect& DamageHitBox::getHitBox() {
	return mHitBox;
}
	///////////////////////////////////////////////
	// /Counts down the hitbox life time. The hit box is set
	// /to inactive when it reaches 0.
	///////////////////////////////////////////////
void DamageHitBox::update(SubLevel* subLevel_p){
	if( mLifeTime <= 0 )
		setAlive( false );

	--mLifeTime;
}
	///////////////////////////////////////////////
	// /Draws a red circle representing the damage hitbox. Only for bug testing
	///////////////////////////////////////////////
void DamageHitBox::drawSelf(){ 
#ifndef SHIPPING
	WindowManager::get().getWindow()->draw( mAnimation.getCurrentSprite(), *WindowManager::get().getStates() );
#endif
}
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
void DamageHitBox::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection){
		//Checks if the DamageHitBox collides with an ice block and if the damage comes from a pickaxe.
	if( dynamic_cast<Crystal*>(entityCollidedWith_p) && mDamageType == PICKAXE){
			//Cast the pointer to a Crystal-pointer
		Crystal* crystal = dynamic_cast<Crystal*>(entityCollidedWith_p);
			//Adjust the Crystals health by the hitbox' damage
		crystal->adjustHealth(mDamage * -1);
			//Play sound for ice collision
	}
}