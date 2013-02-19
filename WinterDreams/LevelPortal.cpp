#include "LevelPortal.h"
#include "Player.h"
#include "Camera.h"
#include "LevelState.h"
#include "GameToScreen.h"
#include "SubLevel.h"
#ifdef _DEBUG
#include "WindowManager.h"
#endif


LevelPortal::LevelPortal(sf::FloatRect position, SubLevel* level, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce) :
	CollisionZone( startEnabled, position, enabledOnce ),
	mLevel( level->getLevel() ),
	mTargetLevel( targetLevel ),
	mTargetPortal( targetPortal )
{	}

LevelPortal::~LevelPortal() { }

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
	if( dynamic_cast<Player*>(pe)){
		auto& player = *mLevel->getPlayer();
		auto& camera = *mLevel->getCamera();

		sf::Vector2f newPos;
		{
			auto nextLevel = mLevel->getSubLevel(mTargetLevel);
			auto temp_sp = nextLevel->getEntity( mTargetPortal ).lock();
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
		mLevel->switchSubLevel( mTargetLevel );
		player.setPosition( newPos );
		camera.snapToPosition( GAME_TO_SCREEN * newPos );
		camera.lockCamera();
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