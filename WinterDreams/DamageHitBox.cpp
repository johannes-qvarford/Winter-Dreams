#include "DamageHitBox.h"

#include "WindowManager.h"
#include "GameState.h"
#include "IceBlock.h"
#include "Wall.h"
///////////////////////////////////////////////
// /Simply assigns the arguments to the correct member variable.
// /EXCEPTION: mLifeTime is assigned manually. Defines how many updates the hitbox lives.
///////////////////////////////////////////////
DamageHitBox::DamageHitBox(unsigned int damage, const sf::Rect<float>& hitBox, DamageTypes type) :
	mHitBox( hitBox ),
	mDamage( damage ),
	mDamageType( type ),
	mLifeTime(20)
{
}
	///////////////////////////////////////////////
	// /Returns the hit box of the damage zone
	///////////////////////////////////////////////
sf::FloatRect& DamageHitBox::getHitBox() const{
	return mHitBox;
}
	///////////////////////////////////////////////
	// /Counts down the hitbox life time. The hit box is set
	// /to inactive when it reaches 0.
	///////////////////////////////////////////////
void DamageHitBox::update(GameState* gameState_p, int milliseconds){
	if( mLifeTime <= 0 )
		setActive(false);

	--mLifeTime;
}
	///////////////////////////////////////////////
	// /Draws a red circle representing the damage hitbox. Only for bug testing
	///////////////////////////////////////////////
void DamageHitBox::drawSelf(){
	WindowManager& window = WindowManager::get();
	sf::CircleShape c;
	c.setFillColor(sf::Color(sf::Uint8(255), sf::Uint8(0), sf::Uint8(0) ) );
	c.setPosition(mHitBox.left, mHitBox.top);

	window.getWindow()->draw(c);
}
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
void DamageHitBox::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection){
		//Checks if the DamageHitBox collides with an ice block and if the damage comes from a pickaxe.
	if( dynamic_cast<IceBlock*>(entityCollidedWith_p) && mDamageType == PICKAXE){
		IceBlock* ice = dynamic_cast<IceBlock*>(entityCollidedWith_p);
		ice->adjustHealth(mDamage * -1);
		//Play sound for ice collision
	}
		//Checks if the DamageHitBox collides with a wall
	else if( dynamic_cast<Wall*>(entityCollidedWith_p) ){
		//Play sounds for wall collision.
	}
		//In case of further collisions
	else {}
}