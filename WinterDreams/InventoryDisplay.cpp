#include "PrecompiledHeader.h"

#include "InventoryDisplay.h"
#include "Player.h"
#include "WindowManager.h"
#include "PropertyManager.h"
#include "FileStructure.h"
////////////////////////////////////////////////////////////////////////////////
class InvDispSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static InvDispSpecs& get();
	
	int mXPos, mYPos, mIconsAppart;

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
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& itemdisp = obj.get_child( "objects.player.itemdisplay" );
		//Get the constants out of the ptree
	mXPos = itemdisp.get<int>("xfromleft");
	mYPos = itemdisp.get<int>("yfromtop");
	mIconsAppart = itemdisp.get<int>("iconsappart");
		//Parse the specs for the animations into the AnimationSpecsList
	AnimationSpecs::parse( itemdisp, mAnimSpecList );

		//Iterate over the animations to get each icons index.
		//This will be used to decide how they will be ordered
	auto& anims = itemdisp.get_child("animations");
	for( auto iter = anims.begin(), end = anims.end(); iter != end; ++iter) {
		std::string s = iter->first;
		int index = iter->second.get<int>("iconindex", -1);
			//If the animation didn't have an index, add it to the auxIcon list
		if(index < 0 )
			mAuxIconList.push_back(s);
			//Else, insert the pair
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

}

InventoryDisplay::~InventoryDisplay()
{}

/////////////////////////////////////////////////////////
void InventoryDisplay::draw() const{
	auto& window = *WindowManager::get().getRenderWindow();
	auto& rendState = *WindowManager::get().getStates();

	if( !mPlayer_wp.expired() ) {
		auto player = mPlayer_wp.lock();
		auto inventory = player->getInventory();

		int nrOfItems = 0;
			//Iterate over all possible items and ask it the player has any of them in it's inventory
		for( auto iter = mAnimationMap.begin(), end = mAnimationMap.end(); iter != end; ++iter) {		
				//Check if the player has the item in it's inventory
			if( inventory.hasItem( iter->first ) > 0 ) {
				++nrOfItems;

				auto& sprite = iter->second.getCurrentSprite();
				window.draw( sprite, rendState);
			}
		}
			//If the player had at least one item, draw the itembox
		if( nrOfItems > 0 ){
			auto& sprite = mBoxAnimation_p->getCurrentSprite();
			window.draw( sprite, rendState );
		}
	}
}

void InventoryDisplay::update(SubLevel* subLevel_p){
	updateUI();		
}

void InventoryDisplay::updateUI() {
	auto& spec = InvDispSpecs::get();
	auto& win = *WindowManager::get().getRenderWindow();
	auto& centPos = win.getView().getCenter();
		//The vector describing the top left corner of the screen
	auto centDif = sf::Vector2f( static_cast<float>(win.getSize().x / 2), static_cast<float>(win.getSize().y / 2));
		//The vector describing the distance from the window boarder to the first item
	auto cornDif = initPos;
		//The vector for the first icon (this one is invisible)
	auto firstIconPos = centPos - centDif +	cornDif;
	
		//Assign the equipment items positions
	for( auto iter =  mAnimationMap.begin(), end = mAnimationMap.end(); iter != end; ++iter) {
		auto& name = iter->first;
		if( spec.mEquipIconIndices.find(name) == spec.mEquipIconIndices.end() )
			continue;
			//Get the icons index
		auto& index = spec.mEquipIconIndices.find(name)->second;
			//Calculate it's offest from the first icon
		auto offset = sf::Vector2f(static_cast<float>(spec.mIconsAppart * index), 0 );
			//Assign it's position
		iter->second.setPosition(firstIconPos + offset);
	}

		//Assign the aux icons positions
	{
		int index = 1;
		std::list<std::string> temp;
		auto& auxList = spec.mAuxIconList;
		auto& player = *mPlayer_wp.lock();
			//Iterate over the list of auxiliry items and check if the player has an
			// item of the type indicated by iter.
			//If it does, add that item to the temporary list named auxItems.
		for( auto iter = auxList.begin(), end = auxList.end(); iter != end; ++iter ){
			auto item = *iter;

			if( player.getInventory().hasItem( item ) > 0 ){
				temp.push_back( item );			
			}
		}
			//Sort temp list
		temp.sort();

			//Iterate over temp and calculate each items position
		for( auto iter = temp.begin(), end = temp.end(); iter != end; ++iter){
			auto offset = sf::Vector2f(0, static_cast<float>(spec.mIconsAppart * index) );
			mAnimationMap.find(*iter)->second.setPosition(firstIconPos + offset);
			++index;
		}
	}
	////Ask what item the player has equipped right now
	//auto index = spec.get().mIconIndices.find( /*CurrentItemName*/  );
	auto offset = sf::Vector2f(static_cast<float>(spec.mIconsAppart * /*index*/ 0), 0 );
	mBoxAnimation_p->setPosition( firstIconPos + offset );
	///////////////////////////////////////////////////
}