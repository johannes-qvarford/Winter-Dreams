#include "Item.h"
#include "Player.h"
#include "PropertyManager.h"
#include "GameToScreen.h"
#include "FileStructure.h"
#include "WindowManager.h"

class ItemSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static ItemSpecs& get();

	std::list<AnimationSpecs> mAnimSpecs;

private:
	ItemSpecs();						//Singleton-pattern
	ItemSpecs(const ItemSpecs& p);		//No copies
	ItemSpecs& operator=(ItemSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
ItemSpecs::ItemSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& item = obj.get_child( "objects.items" );
	
	AnimationSpecs::parse( item, mAnimSpecs);
}
////////////////////////////////////////////////////////////////////////////////
ItemSpecs& ItemSpecs::get() { 
	static ItemSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////


Item::Item(sf::FloatRect position, std::string itemName, bool startEnabled) :
	GraphicalEntity( startEnabled ),
	mItemName( itemName ),
	mHitBox( position )	
{
	auto& animSpecs = ItemSpecs::get().mAnimSpecs;

	mAnimation = nullptr;
	for( auto iter = animSpecs.begin(), end = animSpecs.end(); iter != end; ++iter) {
		if( iter->mAnimName != mItemName )
			continue;

		auto w =	iter->mWidth;
		auto h =	iter->mHeight;
		auto yO =	iter->mYOrigin;
		auto xO =	iter->mXOrigin;
		auto nos =	iter->mNrOfSprites;
		auto fps =	iter->mFramesPerSprite;
		auto file = iter->mFileName;
		auto name = iter->mAnimName;

		mAnimation = new Animation(FS_DIR_OBJECTANIMATIONS +"item/"+ file , w, h, nos, fps, xO, yO);
	}
}

Item::~Item() {
	delete mAnimation;
}

void Item::drawSelf() {
	auto sprite = mAnimation->getCurrentSprite();
	sprite.setPosition(GAME_TO_SCREEN * sf::Vector2f( mHitBox.left, mHitBox.top ) );
	
	auto& window = *WindowManager::get().getWindow();
	auto& state  = *WindowManager::get().getStates();
	window.draw( sprite, state);
}

sf::FloatRect& Item::getHitBox()  { 
	return mHitBox; 
}

void Item::update(SubLevel* subLevel_p) { /* Do nothing */ } 

void Item::onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection) {
	if( dynamic_cast<Player*>( entityCollidedWith_p ) ){
			//If the item collided with an entity of player type, add the item to the players inventory
		auto player = dynamic_cast<Player*>( entityCollidedWith_p );
		player->changeInventory()->giveItem(mItemName, 1);
			//Set the item to dead, so the item is removed.
		setAlive( false );
	}
}
