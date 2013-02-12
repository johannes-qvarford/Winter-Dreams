//#include "LevelPortal.h"
//#include "Player.h"
//#include "Camera.h"
//#include "LevelState.h"
//
//LevelPortal::LevelPortal(sf::FloatRect position, const std::string& targetLevel, const std::string& targetPortal, bool startEnabled, bool enabledOnce) :
//	CollisionZone( startEnabled, position, enabledOnce ),
//	mTargetLevel( targetLevel ),
//	mTargetPortal( targetPortal )
//{	}
//
//LevelPortal::~LevelPortal() { }
//
//void LevelPortal::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection) {
//	if( static_cast<Player*>(pe) ){
//		auto& player = LevelState::getPlayer();
//		auto& camera = LevelState::getCamera();
//		auto& newPos = LevelState::getPortal( mTargetLevel, mTargetPortal );
//
//		player.setPosition( newPos );
//		camera.snapToPosition( newPos );
//		camera.lockCamera();
//
//		LevelState::setSubLevel( targetLevel );	
//	}
//}