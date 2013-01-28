#include "PrecompiledHeader.h"
#include "Camera.h"
#include <cmath>

#include "GameToScreen.h"
#include "WindowManager.h"
#include "PhysicalEntity.h"

static float MAX_MOVESPEED = 5.0f;

Camera::Camera(sf::Vector2f position) :
	mCurrentPanSpeed( MAX_MOVESPEED ),
	mCameraPosition( GAME_TO_SCREEN * position),
	mDesiredPosition( mCameraPosition ),
	mLockedEntity( NULL )
{
	snapToPosition( mCameraPosition );
	draw();
}

Camera::Camera( PhysicalEntity* entity ) :
	mCurrentPanSpeed( MAX_MOVESPEED ),
	mCameraPosition (0,0),
	mDesiredPosition( mCameraPosition ),
	mLockedEntity( entity )
{
	auto entityPos = sf::Vector2f(entity->getHitBox().left, entity->getHitBox().top);
	mCameraPosition = GAME_TO_SCREEN * entityPos;

	snapToPosition( mCameraPosition );
	draw();
}

Camera::~Camera(void) {}

void Camera::draw() {
		//Get a reference to the render window
	auto& window = *WindowManager::get().getWindow();
		//If the camera is locked on an entity, update the cameras 
		//desired position to the entitys current position
	if( mLockedEntity != NULL ) {
		auto entityPos = sf::Vector2f(mLockedEntity->getHitBox().left, mLockedEntity->getHitBox().top);
		mDesiredPosition = GAME_TO_SCREEN * entityPos;
	}
		//If the difference between the desired position and the 
		//current position is greater then 0.1, move the camera
		//'currentPanSpeed' in the direction pointing to 
		//the desired position
	if(	abs(mCameraPosition.x - mDesiredPosition.x) > 0.1 ||
		abs(mCameraPosition.y - mDesiredPosition.y) > 0.1) {
			//Take out the direction between current pos and
			//desired pos
		auto dirVect = mDesiredPosition - mCameraPosition;
			//Calculate the lenght of the vector between the positions
		auto dirLenght = std::sqrt( dirVect.x * dirVect.x + dirVect.y * dirVect.y );
			//If lenght is longer then 0
		if(dirLenght > 0 ){
				//Normalize the direction vector
			dirVect.x = dirVect.x / dirLenght;
			dirVect.y = dirVect.y / dirLenght;
				//Then extend the normalized vector by the
				//current pan speed.
			dirVect.x *= dirLenght;
			dirVect.y *= dirLenght;
				//And update the cameras position by
				//the direction vector.
			mCameraPosition += dirVect;
		}
	}
		//Convert the window size to a float vector
	auto winSizeU = window.getSize();
	auto winSizeF = sf::Vector2f( static_cast<float>(winSizeU.x), static_cast<float>(winSizeU.y) );
		//Calculate the view position by substracting half the screen width
		//and screen height from the cameras current position
	auto camPos = mCameraPosition - sf::Vector2f(winSizeF.x * 0.5f , winSizeF.y * 0.5f);
		//Set the view
	sf::View view( sf::View( camPos, winSizeF) );
}

void Camera::followEntity(std::shared_ptr<PhysicalEntity> entity){
	mLockedEntity = entity;
}

void Camera::panToPosition(sf::Vector2f position){
		//Locked entity must be assigned NULL or the camera
		//will attempt to pan back to the locked entity
	mLockedEntity = NULL;	
	mDesiredPosition = position;
}

void Camera::snapToPosition(sf::Vector2f position){
		//Locked entity must be assigned NULL or the camera
		//will attempt to pan back to the locked entity
	mLockedEntity = NULL;
	mCameraPosition = position;
}

void Camera::setPanSpeed(float moveSpeedPercentage){
	mCurrentPanSpeed = MAX_MOVESPEED * moveSpeedPercentage;
}