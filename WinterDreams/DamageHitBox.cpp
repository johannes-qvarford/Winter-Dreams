#include "DamageHitBox.h"
#include "SubLevel.h"
#include "Crystal.h"
#include "FileStructure.h"
#include <SFML\Audio.hpp>
#include "PropertyManager.h"
#include "GameToScreen.h"


class HitBoxSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static HitBoxSpecs& get();

	int mLifeTime;
	std::list<AnimationSpecs> mAnimSpecList;

private:
	HitBoxSpecs();							//Singleton-pattern
	HitBoxSpecs(const HitBoxSpecs& p);		//No copies
	HitBoxSpecs& operator=(HitBoxSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
HitBoxSpecs::HitBoxSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& box = obj.get_child( "objects.hitbox" );
	mLifeTime = box.get<int>("lifetime");
	
	AnimationSpecs::parse( box, mAnimSpecList );
}
////////////////////////////////////////////////////////////////////////////////
HitBoxSpecs& HitBoxSpecs::get() { 
	static HitBoxSpecs p;
	return p;
}

///////////////////////////////////////////////
// /Simply assigns the arguments to the correct member variable.
// /EXCEPTION: mLifeTime is assigned manually. Defines how many updates the hitbox lives.
///////////////////////////////////////////////
DamageHitBox::DamageHitBox(const sf::Rect<float>& hitBox, unsigned int damage, std::string damageType) :
	GraphicalEntity( true ),
	mHitBox( hitBox ),
	mDamage( damage ),
	mDamageType( damageType ),
	mLifeTime(10),
	mCurrentAnimation_p( nullptr )
{ 
	auto& list = HitBoxSpecs::get().mAnimSpecList;

	for( auto it = list.begin(), end = list.end(); it != end; ++it){
		if( it->mAnimName != damageType )
			continue;

		auto w =	it->mWidth;
		auto h =	it->mHeight;
		auto yO =	it->mYOrigin;
		auto xO =	it->mXOrigin;
		auto nos =	it->mNrOfSprites;
		auto fps =	it->mFramesPerSprite;
		auto file = it->mFileName;
		auto name = it->mAnimName;

		mCurrentAnimation_p = new Animation( FS_DIR_OBJECTANIMATIONS +"player/" + file , w, h, nos, fps, xO, yO );
	}

	mCurrentAnimation_p->setPosition(GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top) );
}

DamageHitBox::~DamageHitBox() {
	delete mCurrentAnimation_p;
}
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
	mCurrentAnimation_p->updateAnimation();

	if( mCurrentAnimation_p->endOfAnimation() )
		setAlive( false );

}
	///////////////////////////////////////////////
	// /Draws a red circle representing the damage hitbox. Only for bug testing
	///////////////////////////////////////////////
void DamageHitBox::drawSelf(){ 
	WindowManager::get().getWindow()->draw( mCurrentAnimation_p->getCurrentSprite(), *WindowManager::get().getStates() );
}
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
void DamageHitBox::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection) { }

int DamageHitBox::getDamageAmount() const {
	return getEnabled() ? mDamage: 0;
}

std::string DamageHitBox::getDamageType() const {
	return mDamageType;
}