#include "PrecompiledHeader.h"

#include "InventoryDisplay.h"
#include "Player.h"
#include "WindowManager.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "LevelState.h"
////////////////////////////////////////////////////////////////////////////////
class InvDispSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static InvDispSpecs& get();
	
	float mXPos, mYPos, mXIconOffset, mYIconOffset;

	std::list<AnimationSpecs>	mAnimSpecList;	//List of animation specs
	std::list<std::string>		mAuxIconList;	//List of icons of tempItems and the itemBox
	std::map<std::string, int>	mEquipIconIndices; //List of icons of equipItems.
	
private:
	InvDispSpecs();							//Singleton-pattern
	InvDispSpecs(const InvDispSpecs& p);		//No copies
	InvDispSpecs& operator=(InvDispSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
InvDispSpecs::InvDispSpecs() {
	auto& obj = PropertyManager::get().getGeneralSettings();
	auto& itemdisp = obj.get_child( "itemdisplay" );
		//Get the constants out of the ptree
	mXPos = itemdisp.get<float>("xfromleft");
	mYPos = itemdisp.get<float>("yfromtop");
	mXIconOffset = itemdisp.get<float>("xiconoffset");
	mYIconOffset = itemdisp.get<float>("yiconoffset");

		//Parse the specs for the animations into the AnimationSpecsList
	AnimationSpecs::parse( itemdisp, mAnimSpecList );
		/////////////////////////////////////////////////
		//Iterate over the animations to get each icons index.
		//This will be used to decide how they will be ordered
		/////////////////////////////////////////////////
	auto& anims = itemdisp.get_child("animations");
	for( auto iter = anims.begin(), end = anims.end(); iter != end; ++iter) {
		std::string s = iter->first;
		int index = iter->second.get<int>("iconindex", -1);
			/////////////////////////////////////////////////
			//If the animation didn't have an index, add it to the auxIcon list
			/////////////////////////////////////////////////
		if(index <= 0 ){
			mAuxIconList.push_back(s);
			continue;
		}
			/////////////////////////////////////////////////
			//Else, insert the pair
			/////////////////////////////////////////////////
		std::pair<std::string, int> pair(s, index);
		mEquipIconIndices.insert( pair );
	}
		//Sort the auxList
	mAuxIconList.sort();
}
////////////////////////////////////////////////////////////////////////////////
InvDispSpecs& InvDispSpecs::get() { 
	static InvDispSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////

InventoryDisplay::InventoryDisplay(std::weak_ptr<Player> player) :
	Script( true ),
	mPlayer_wp( player )
{
		//get a copy to the windows size
	auto winSize = WindowManager::get().getRenderWindow()->getSize();
	auto& specs = InvDispSpecs::get();
		//get a reference to the scriptspecs
	
	initPos = sf::Vector2f( float(specs.mXPos), float(specs.mYPos) );

	auto& p = InvDispSpecs::get();
	for( auto iter = p.mAnimSpecList.begin(), end = p.mAnimSpecList.end(); iter != end; ++iter){
		auto w =	iter->mWidth;
		auto h =	iter->mHeight;
		auto yO =	iter->mYOrigin;
		auto xO =	iter->mXOrigin;
		auto nos =	iter->mNrOfSprites;
		auto fps =	iter->mFramesPerSprite;
		auto file = iter->mFileName;
		auto name = iter->mAnimName;
			//Fill the animationmap 
		Animation anim(FS_DIR_OBJECTANIMATIONS +"itemdisplay/"+ file , w, h, nos, fps, xO, yO);
		mAnimationMap.insert( std::pair<std::string, Animation>( name , anim ) );
	}

	mBoxAnimation_p = &mAnimationMap.find("itembox")->second;
	mFrameAnimation_p = &mAnimationMap.find("frame")->second;
}

InventoryDisplay::~InventoryDisplay()
{}

/////////////////////////////////////////////////////////
void InventoryDisplay::draw() const{
	auto& window = *WindowManager::get().getRenderWindow();
	auto& rendState = *WindowManager::get().getStates();

	if( !mPlayer_wp.expired() ) {
		for( auto it = mItemSpriteList.begin(), end = mItemSpriteList.end(); it != end; ++it){
			auto sprite = *it;
			auto size = window.getSize();
			//we need to change from normalized coordiantes, to screen coordinates.
			sprite.setPosition(sprite.getPosition().x * window.getSize().x, sprite.getPosition().y * window.getSize().y);
			auto vec = sf::Vector2f(float(window.getSize().x) / 1920, float(window.getSize().y) / 1080);
			sprite.setScale(vec);
			//sprite.setScale(window.getSize().x / 1920, window.getSize().y / 1080);
			//			
//			sprite.setScale(0.1, 0.1);
			//auto pos = sprite.getPosition();
			window.draw( sprite , rendState );		
		}
	}
}

void InventoryDisplay::update(SubLevel* subLevel_p){
	updateUI();		
}

void InventoryDisplay::updateUI() {
		/////////////////////////////////////////////////////////
		//Clear the sprite list
		/////////////////////////////////////////////////////////
	mItemSpriteList.clear();

	auto& player = *mPlayer_wp.lock();
	auto& inventory =  player.getInventory();

	auto& spec = InvDispSpecs::get();
	auto& win = *WindowManager::get().getRenderWindow();
	auto centPos = sf::Vector2f(0, 0);
		/////////////////////////////////////////////////////////
		//The vector describing the top left corner of the screen
		/////////////////////////////////////////////////////////
	auto centDif = sf::Vector2f(0.0, 0.0);
		/////////////////////////////////////////////////////////
		//The vector describing the distance from the window boarder to the first item
		/////////////////////////////////////////////////////////
	auto cornDif = initPos;
		/////////////////////////////////////////////////////////
		//The vector for the first icon (this one is invisible)
		/////////////////////////////////////////////////////////
	auto firstIconPos = centPos - centDif +	cornDif;
		/////////////////////////////////////////////////////////
		//Assign the equipment items positions
		/////////////////////////////////////////////////////////
	for( auto iter =  mAnimationMap.begin(), end = mAnimationMap.end(); iter != end; ++iter) {
		auto& name = iter->first;
			/////////////////////////////////////////////////////////
			//If the named item doesn't exist in the mEquipIconIndices map
			// continue
			/////////////////////////////////////////////////////////
		if( spec.mEquipIconIndices.find(name) == spec.mEquipIconIndices.end() )
			continue;
			/////////////////////////////////////////////////////////
			//If the named item doesn't exist in the players inventory
			// continue
			/////////////////////////////////////////////////////////
		if( !inventory.hasItem( name ) )
			continue;

			/////////////////////////////////////////////////////////
			//Get the icons index
			/////////////////////////////////////////////////////////
		auto& index = spec.mEquipIconIndices.find(name)->second;
			/////////////////////////////////////////////////////////
			//Calculate it's offest from the first icon
			/////////////////////////////////////////////////////////
		auto offset = sf::Vector2f(static_cast<float>(spec.mXIconOffset * index), 0 );
			/////////////////////////////////////////////////////////
			//Assign it's position
			/////////////////////////////////////////////////////////
		iter->second.setPosition(firstIconPos + offset);
			/////////////////////////////////////////////////////////
			//Add the sprite to the list
			/////////////////////////////////////////////////////////
		mItemSpriteList.push_back( iter->second.getCurrentSprite() );
	}
		/////////////////////////////////////////////////////////
		//Assign the aux icons positions
		/////////////////////////////////////////////////////////
	{
		int index = 1;
		std::list<std::string> temp;
		auto& auxList = spec.mAuxIconList;
			/////////////////////////////////////////////////////////
			//Iterate over the list of auxiliry items and check if the player has an
			// item of the type indicated by iter.
			//If it does, add that item to the temporary list named auxItems.
			/////////////////////////////////////////////////////////
		for( auto iter = auxList.begin(), end = auxList.end(); iter != end; ++iter ){
			auto item = *iter;
			auto amount =  player.getInventory().hasItem( item );
			if( amount > 0 ){
				while( amount != 0 ){
				temp.push_back( item );
				--amount;
				}
			}
		}	
			/////////////////////////////////////////////////////////
			//Sort temp list
			/////////////////////////////////////////////////////////
		temp.sort();
			/////////////////////////////////////////////////////////
			//Iterate over temp and calculate each items position.
			//Insert the different items sprites into the spritelist
			//which will be drawn.
			/////////////////////////////////////////////////////////
		for( auto iter = temp.begin(), end = temp.end(); iter != end; ++iter){
			auto offset = sf::Vector2f(0, static_cast<float>(spec.mYIconOffset * index) );
				/////////////////////////////////////////////////////////
				//Draw the box indicating which item is equipped
				/////////////////////////////////////////////////////////
			mAnimationMap.find(*iter)->second.setPosition(firstIconPos + offset);
				/////////////////////////////////////////////////////////
				//Add the item's sprite to the list, then increase the idex
				/////////////////////////////////////////////////////////
			mItemSpriteList.push_back( mAnimationMap.find(*iter)->second.getCurrentSprite() );
			++index;
		}
	}
		/////////////////////////////////////////////////////////
		//Calculate position for the box indicating which item is equipped
		/////////////////////////////////////////////////////////
	{
		auto currentItem = mPlayer_wp.lock().get()->getInventory().getCurrentEquip();
		int index = 0;
			/////////////////////////////////////////////////////////
			// if the current item is equipable (if it isn't, no item
			// is equipped).
			/////////////////////////////////////////////////////////
		if( currentItem != "" ){
			index = spec.get().mEquipIconIndices.find( currentItem )->second;
		}
		else{
			index = -5;
		}
		auto offset = sf::Vector2f(static_cast<float>(spec.mXIconOffset * index), 0 );
		mBoxAnimation_p->setPosition( firstIconPos + offset );
			/////////////////////////////////////////////////////////
			//Add the box's sprite to the list
			/////////////////////////////////////////////////////////
		mItemSpriteList.push_back( mBoxAnimation_p->getCurrentSprite() );
	}
/////////////////////////////////////////////////////////
//Add the inventory fame's sprite to the list
////////////////////////////////////////////////////////
	mItemSpriteList.push_back( mFrameAnimation_p->getCurrentSprite() );
}
