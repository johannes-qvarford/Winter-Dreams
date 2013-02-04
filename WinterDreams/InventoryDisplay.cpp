#include "PrecompiledHeader.h"

#include "InventoryDisplay.h"
#include "Player.h"
#include "WindowManager.h"
#include "PropertyManager.h"
#include "FileStructure.h"
////////////////////////////////////////////////////////////////////////////////
class ScriptSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static ScriptSpecs& get();
	
	float mXPosMultiplier;
	float mYPosMultiplier;

	std::list<AnimationSpecs> mAnimSpecList;

private:
	ScriptSpecs();							//Singleton-pattern
	ScriptSpecs(const ScriptSpecs& p);		//No copies
	ScriptSpecs& operator=(ScriptSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
ScriptSpecs::ScriptSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& itemdisp = obj.get_child( "objects.player.itemdisplay" );
	
	mXPosMultiplier = itemdisp.get<float>("xposition");
	mYPosMultiplier = itemdisp.get<float>("yposition");

	AnimationSpecs::parse( itemdisp, mAnimSpecList );
}
////////////////////////////////////////////////////////////////////////////////
ScriptSpecs& ScriptSpecs::get() { 
	static ScriptSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////

InventoryDisplay::InventoryDisplay(std::weak_ptr<Player> player) :
	Script( true ),
	mPlayer_wp( player )
{
		//get a copy to the windows size
	auto winSize = WindowManager::get().getWindow()->getSize();
	auto& specs = ScriptSpecs::get();
		//assign the center of the display box
	mXPos = winSize.x * specs.mXPosMultiplier;
	mYPos = winSize.y * specs.mYPosMultiplier;
		//get a reference to the scriptspecs
	auto& p = ScriptSpecs::get();
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
	for( auto iter = mAnimationMap.begin(), end = mAnimationMap.end(); iter != end; ++iter) {
		sf::Vector2f pos( mXPos, mYPos );
		iter->second.setPosition( pos );
	}
}

InventoryDisplay::~InventoryDisplay()
{}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void InventoryDisplay::draw() const{
	auto& window = *WindowManager::get().getWindow();
	auto& rendState = *WindowManager::get().getStates();
	
	window.draw( mBoxAnimation->getCurrentSprite(), rendState );
	window.draw( mCurrentItemAnimation->getCurrentSprite(), rendState );
}

void InventoryDisplay::update(GameState* gameState_p){
	if( !mPlayer_wp.expired() ) {
		auto player = mPlayer_wp.lock();
		
		auto& inventory = player->getInventory();

	}/////////////////////////////////////////////////////////
}/////////////////////////////////////////////////////////