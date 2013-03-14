#include "LevelPortal.h"
#include "Player.h"
#include "Camera.h"
#include "LevelState.h"
#include "GameToScreen.h"
#include "SubLevel.h"

#include "WindowManager.h"
#include "InputManager.h"
#include "SubLevelFade.h"
#include "PropertyManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"

class PortalSpecs{
public:	
	////////////////////////////////////////////////////////////////////////////
	// /Singleton-pattern.
	// /Is used to access the different properties of the player.
	////////////////////////////////////////////////////////////////////////////
	static PortalSpecs& get();

	std::list<AnimationSpecs> mAnimSpecs;

	std::string mSoundFilename;

	int mWaitingFrames;

private:
	PortalSpecs();						//Singleton-pattern
	PortalSpecs(const PortalSpecs& p);		//No copies
	PortalSpecs& operator=(PortalSpecs& p);	//No copies
};
////////////////////////////////////////////////////////////////////////////////
PortalSpecs::PortalSpecs() {
	auto& obj = PropertyManager::get().getObjectSettings();
	auto& item = obj.get_child( "objects.levelportal" );
	
	
	mSoundFilename = item.get<std::string>("soundfilename");
	mWaitingFrames = item.get<int>("waitingframes");
	AnimationSpecs::parse( item, mAnimSpecs);
}
////////////////////////////////////////////////////////////////////////////////
PortalSpecs& PortalSpecs::get() { 
	static PortalSpecs p;
	return p;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

LevelPortal::LevelPortal(sf::FloatRect position, SubLevel* subLevel, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce, sf::Vector2i direction) :
	GraphicalEntity( startEnabled ),
	mOnce( enabledOnce ),
	mHitBox( position ),
	mSubLevel_p(subLevel),
	mTargetLevel( targetLevel ),
	mTargetPortal( targetPortal ),
	mIsWaiting(false),
	mWaitingFrames(0),
	mDirection(direction),
	mCurrentAnimation_p( nullptr ),
	mSound(),
	mSoundBuffer_sp(ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + PortalSpecs::get().mSoundFilename))
{	
	mSound.setBuffer(*mSoundBuffer_sp);

	auto& animSpecs = PortalSpecs::get().mAnimSpecs;
	auto& ps = PortalSpecs::get();

		//Since there is only one animation in PortalSpec's list,
		//this loop will only be performed once. 
		//But for consistency, we use the loop-syntax.
	for( auto iter = animSpecs.begin(), end = animSpecs.end(); iter != end; ++iter) {
		if(iter->mAnimName != "portal" )
			continue;

		auto w =	iter->mWidth;
		auto h =	iter->mHeight;
		auto yO =	iter->mYOrigin;
		auto xO =	iter->mXOrigin;
		auto nos =	iter->mNrOfSprites;
		auto fps =	iter->mFramesPerSprite;
		auto file = iter->mFileName;
		auto name = iter->mAnimName;

		mCurrentAnimation_p = new Animation(FS_DIR_OBJECTANIMATIONS +"levelportal/"+ file , w, h, nos, fps, xO, yO);
	}
}

LevelPortal::~LevelPortal() {
	delete mCurrentAnimation_p;
}

void LevelPortal::update(SubLevel* subLevel_p){
	
	auto& ps = PortalSpecs::get();

	if (mIsWaiting){
		++mWaitingFrames; 
	}
	if (mWaitingFrames == PortalSpecs::get().mWaitingFrames){

		auto& player = *mSubLevel_p->getLevel()->getPlayer();
		auto& camera = *mSubLevel_p->getLevel()->getCamera();

		auto nextSubLevel_p = std::shared_ptr<SubLevel>();
		sf::Vector2f newPos;
		{
			nextSubLevel_p = mSubLevel_p->getLevel()->getSubLevel(mTargetLevel);
			auto temp_sp = nextSubLevel_p->getEntity( mTargetPortal ).lock();
			auto nextPortal_p = dynamic_cast<LevelPortal*>(temp_sp.get() );
				//////////////////////////////////////////////////////////////
				// /Get the position of the targetPortal
				// /
				// /Adjust the position so that the player does not spawn
				// /directly into a new portal and is teleported back
				//////////////////////////////////////////////////////////////
			auto& rect = nextPortal_p->getHitBox();
			auto tilesWide = static_cast<int>(rect.width / X_STEP) -1;

			nextPortal_p->setEnabled(false);

			newPos = sf::Vector2f( rect.left + X_STEP * tilesWide, rect.top);
		}

		//////////////////////////////////////////////////////////////
		// /Switch sublevel to the target level.
		// /Move the player to the position previously calculated
		// /Snap the camera to the players position
		// /Lock the camera again (since snapToPosition unlocks it automaticly)
		//////////////////////////////////////////////////////////////
		mSubLevel_p->getLevel()->switchSubLevel( mTargetLevel );
		player.setPosition( newPos );
		player.setFacingDirection(mDirection);
		camera.snapToPosition( GAME_TO_SCREEN * newPos );
		camera.lockCamera();

		auto fade_sp = std::shared_ptr<SubLevelFade>(new SubLevelFade(PortalSpecs::get().mWaitingFrames, SubLevelFade::FADE_IN));
		nextSubLevel_p->addScript(fade_sp);
		mIsWaiting = false;
		mWaitingFrames = 0;
	}	

	mCurrentAnimation_p->updateAnimation();		
}

void LevelPortal::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection) {
		//////////////////////////////////////////////////////////////
		// /If targetPortal is "", the portal is a one-way portal. Hence
		// /it shouldn't affect anything on collision
		//////////////////////////////////////////////////////////////
	if(mTargetPortal == "" || getEnabled() == false )
		return;
		//////////////////////////////////////////////////////////////
		// /Checks if the entity collided with it of type player.
		//////////////////////////////////////////////////////////////
	if( dynamic_cast<Player*>(pe) && !mIsWaiting){
		mIsWaiting = true;
		auto fade_sp = std::shared_ptr<SubLevelFade>(new SubLevelFade(PortalSpecs::get().mWaitingFrames, SubLevelFade::FADE_OUT));
		mSubLevel_p->addScript(fade_sp);
		//play sound
		mSound.play();
	}

	//if (mOnce == true)
	//	setEnabled(false);
}

void LevelPortal::drawSelf() {	
	sf::Vector2f pos = GAME_TO_SCREEN * sf::Vector2f( mHitBox.left, mHitBox.top);
	auto& s = mCurrentAnimation_p->getCurrentSprite();
	s.setPosition( pos );

	auto& window = *WindowManager::get().getWindow();
	auto& state  = *WindowManager::get().getStates();
	window.draw( s, state);
}