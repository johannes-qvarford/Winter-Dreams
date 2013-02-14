#include "LevelPortal.h"
#include "Player.h"
#include "Camera.h"
#include "LevelState.h"
#include "GameToScreen.h"
#include "SubLevel.h"

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
	if(mTargetPortal == "")
		return;
		//////////////////////////////////////////////////////////////
		// /Checks if the entity collided with it of type player.
		//////////////////////////////////////////////////////////////
	if( static_cast<Player*>(pe)){
		auto& player = *mLevel->getPlayer();
		auto& camera = *mLevel->getCamera();

		sf::Vector2f newPos;
		{
			auto nextLevel = mLevel->getSubLevel(mTargetLevel);
			auto temp_sp = nextLevel->getEntity( mTargetPortal ).lock();
			auto nextPortal_p = static_cast<LevelPortal*>(temp_sp.get() );
				//////////////////////////////////////////////////////////////
				// /Get the position of the targetPortal
				//////////////////////////////////////////////////////////////
			newPos = sf::Vector2f(nextPortal_p->getHitBox().left, nextPortal_p->getHitBox().top );
				//////////////////////////////////////////////////////////////
				// /Adjust the position so that the player does not spawn
				// /directly into a new portal and is teleported back
				//////////////////////////////////////////////////////////////
			newPos.y += Y_STEP;
		}
			//////////////////////////////////////////////////////////////
			// /Switch sublevel to the target level.
			// /Move the player to the position previously calculated
			// /Snap the camera to the players position
			// /Lock the camera again (since snapToPosition unlocks it automaticly)
			//////////////////////////////////////////////////////////////
		mLevel->switchSubLevel( mTargetLevel );
		player.setPosition( newPos );
		camera.snapToPosition( newPos );
		camera.lockCamera();
	}
}