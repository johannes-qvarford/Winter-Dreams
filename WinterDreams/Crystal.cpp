#include "Crystal.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "DamageHitBox.h"
#include "FileStructure.h"
#include "PropertyManager.h"
#include <string>

class CrystalSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static CrystalSpecs& get();


	int mHealth;
	std::list<AnimationSpecs> mAnimSpecList;

private:
	CrystalSpecs();							//Singleton-pattern
	CrystalSpecs(const CrystalSpecs& p);		//No copies
	CrystalSpecs& operator=(CrystalSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
CrystalSpecs::CrystalSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& crystal = obj.get_child( "objects.crystal" );
	mHealth = crystal.get<int>( "hp" );
	
	AnimationSpecs::parse( crystal, mAnimSpecList );
}
////////////////////////////////////////////////////////////////////////////////
CrystalSpecs& CrystalSpecs::get() { 
	static CrystalSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////

Crystal::Crystal( const sf::FloatRect& position, bool startEnabled, int imgVersion) : 
	GraphicalEntity( startEnabled ),
	mSolidZone( new SolidZone( position, startEnabled ) ),
	mVersion ( imgVersion ),
	mHP		( CrystalSpecs::get().mHealth )

{	

	assert( mVersion == 1 || mVersion == 2 || mVersion == 3 );

	using namespace std;
	auto& p = CrystalSpecs::get();
	for( auto iter = p.mAnimSpecList.begin(), end = p.mAnimSpecList.end(); iter != end; ++iter){
		auto w =	iter->mWidth;
		auto h =	iter->mHeight;
		auto yO =	iter->mYOrigin;
		auto xO =	iter->mXOrigin;
		auto nos =	iter->mNrOfSprites;
		auto fps =	iter->mFramesPerSprite;
		auto file = iter->mFileName;
		auto name = iter->mAnimName;

		Animation anim(FS_DIR_OBJECTANIMATIONS +"crystal/"+ file , w, h, nos, fps, xO, yO);
		mAnimationMap.insert( pair<string, Animation>( name , anim ) );
	}
	char VERSION[] = "solid0";

	VERSION[5] += rand()%3 + 1;

	mCurrentAnimation = &mAnimationMap.find( std::string(VERSION) )->second; //INSERT INITIAL CRYSTAL IMG HERE
		//Get a reference to the crystal's solidzone's hitbox
	auto& box = mSolidZone->getHitBox();
		//Create a positionvector from the solidzone's hitbox
	auto pos = sf::Vector2f(box.left, box.top);
		//Convert the position vector to screen coordinates
	pos = ( GAME_TO_SCREEN * pos );

	for( auto iter = mAnimationMap.begin(), end = mAnimationMap.end(); iter != end; ++iter) {
			//Set the position of each animation to the calculated position
		auto& anim = iter->second;
		anim.setPosition( pos );
	}
}

Crystal::~Crystal() { }

void Crystal::update(SubLevel* subLevel_p) {

	updateAnimation();

	if( mHP <= 0 )
		setAlive( false );
}

void Crystal::drawSelf() {
	auto& windowManager = WindowManager::get();
	auto& sprite = mCurrentAnimation->getCurrentSprite();

	windowManager.getWindow()->draw( sprite, *windowManager.getStates() );
}

void Crystal::adjustHealth(int adjustment){
	mHP += adjustment;
}

sf::FloatRect& Crystal::getHitBox() {
	return mSolidZone->getHitBox();
}

void Crystal::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection) {
		//First do the standard Solid Zone collisions
	mSolidZone->onCollision( entityCollidedWith_p, intersection );

		//Then, if crystal collided with a DamageHitBox
	if( dynamic_cast<DamageHitBox*>( entityCollidedWith_p ) ) {
		auto dmgHitBox = dynamic_cast<DamageHitBox*>( entityCollidedWith_p );
		
		if( dmgHitBox->getDamageType() == "pickaxe" ) {
				//Reduce crytal's HP by DamageHitBox's damage then set the hitbox to !enabled
			adjustHealth(dmgHitBox->getDamageAmount() * -1);
			dmgHitBox->disableNextFrame();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Crystal::updateAnimation(){

	if( mHP >= 3)
	{}
	else if (mHP == 2)
		mCurrentAnimation = &mAnimationMap.find( "broken" )->second;
	else
		mCurrentAnimation = &mAnimationMap.find( "destroyed" )->second;
}