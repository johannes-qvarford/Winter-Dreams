#include "Item.h"
#include "Player.h"
#include "PropertyManager.h"
#include "GameToScreen.h"
#include <SFML\Audio\Sound.hpp>
#include "FileStructure.h"
#include "WindowManager.h"
#include "ResourceManager.h"


class ItemSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static ItemSpecs& get();

	std::list<AnimationSpecs> mAnimSpecs;
	std::string mPickUpSound;

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
	mPickUpSound = item.get<std::string>("itemsound");
} 
////////////////////////////////////////////////////////////////////////////////
ItemSpecs& ItemSpecs::get() { 
	static ItemSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////


Item::Item(const sf::FloatRect& position, const std::string& itemName, bool startEnabled) :
	Entity( startEnabled ),
	BaseHitBoxHaveable(position),
	mItemName( itemName )
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
	mSoundBuffer = ResourceManager::get().getSoundBuffer( FS_DIR_SOUNDS + ItemSpecs::get().mPickUpSound );
}

Item::~Item() {
	delete mAnimation;
}

void Item::draw() {
	auto sprite = mAnimation->getCurrentSprite();
	sprite.setPosition(GAME_TO_SCREEN * sf::Vector2f( mHitBox.left, mHitBox.top ) );
	
	auto& window = *WindowManager::get().getWindow();
	auto& state  = *WindowManager::get().getStates();
	window.draw( sprite, state);
}

void Item::update(SubLevel* subLevel_p) {} 

void Item::onCollision(Collidable* col_p, const sf::Rect<float>& intersection) {
	static sf::Sound sound( *mSoundBuffer );
	sound.setVolume( PropertyManager::get().getUserSettings()->get<float>("volumes.soundVolume") * 0.4f );

	if( auto player_p = dynamic_cast<Player*>( col_p ) ){
			//If the item collided with an entity of player type, add the item to the players inventory
		player_p->changeInventory()->giveItem(mItemName);
			//Set the item to dead, so the item is removed.
		setAlive( false );
		sound.play();
	}
}
