#include "LevelPortal.h"
#include "Player.h"
#include "Camera.h"
#include "LevelState.h"
#include "GameToScreen.h"
#include "SubLevel.h"
#ifdef _DEBUG
#include "WindowManager.h"
#endif
#include "InputManager.h"
#include "SubLevelFade.h"

LevelPortal::LevelPortal(sf::FloatRect position, SubLevel* subLevel, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce, sf::Vector2i direction) :
	CollisionZone( startEnabled, position, enabledOnce ),
	mSubLevel_p(subLevel),
	mTargetLevel( targetLevel ),
	mTargetPortal( targetPortal ),
	mIsWaiting(false),
	mWaitingFrames(0),
	mDirection(direction)
{	}

LevelPortal::~LevelPortal() { }


const int WAITINGFRAMES = 60;

void LevelPortal::update(SubLevel* subLevel_p){


		
			
		
	
	if (mIsWaiting){
		++mWaitingFrames; 
	}
	if (mWaitingFrames == WAITINGFRAMES){

		auto& player = *mSubLevel_p->getLevel()->getPlayer();
		auto& camera = *mSubLevel_p->getLevel()->getCamera();

		auto nextSubLevel_p = std::shared_ptr<SubLevel>();
		sf::Vector2f newPos;
		{
			nextSubLevel_p = mSubLevel_p->getLevel()->getSubLevel(mTargetLevel);
			auto temp_sp = nextSubLevel_p->getEntity( mTargetPortal ).lock();
			auto nextPortal_p = static_cast<LevelPortal*>(temp_sp.get() );
				//////////////////////////////////////////////////////////////
				// /Get the position of the targetPortal
				// /
				// /Adjust the position so that the player does not spawn
				// /directly into a new portal and is teleported back
				//////////////////////////////////////////////////////////////
			auto& rect = nextPortal_p->getHitBox();
			auto tilesWide = static_cast<int>(rect.width / X_STEP) -1;

			nextPortal_p->swapEnabled();

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
		InputManager::get().unlockInput();
		auto fade_sp = std::shared_ptr<SubLevelFade>(new SubLevelFade(WAITINGFRAMES, SubLevelFade::FADE_IN));
		nextSubLevel_p->addScript(fade_sp);
		mIsWaiting = false;
		mWaitingFrames = 0;
	}
}

void LevelPortal::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection) {
		//////////////////////////////////////////////////////////////
		// /If targetPortal is "", the portal is a one-way portal. Hence
		// /it shouldn't affect anything on collision
		//////////////////////////////////////////////////////////////
	if(mTargetPortal == "" || getEnabled() == false)
		return;
		//////////////////////////////////////////////////////////////
		// /Checks if the entity collided with it of type player.
		//////////////////////////////////////////////////////////////
	if( dynamic_cast<Player*>(pe) && !mIsWaiting){
		mIsWaiting = true;
		auto fade_sp = std::shared_ptr<SubLevelFade>(new SubLevelFade(WAITINGFRAMES, SubLevelFade::FADE_OUT));
		mSubLevel_p->addScript(fade_sp);
		InputManager::get().lockInput();
	}
}

void LevelPortal::drawSelf() {
#ifdef _DEBUG

	sf::Vertex vertices[] =
	{
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top), sf::Color::Red, sf::Vector2f( 0,  0)),
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f( 0, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top), sf::Color::Red, sf::Vector2f(10,  0))
	};

	auto& window = *WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	//translate to screen coordinates
	states.transform *= GAME_TO_SCREEN;

	window.draw(vertices, 4, sf::LinesStrip, states);
#endif
}