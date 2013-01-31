#include "PrecompiledHeader.h"
#include "Camera.h"
#include <cmath>
#include <memory>

#include "GameToScreen.h"
#include "WindowManager.h"
#include "PhysicalEntity.h"

static float		CAM_PAN_PERCENTAGE = 0.10f;

Camera::Camera(sf::Vector2f position) :
	mCameraPosition(  GAME_TO_SCREEN * position),
	mDesiredPosition( mCameraPosition ),
	mLockedEntity(),
	mLockedCamera( false )
	{}

Camera::Camera( std::shared_ptr<PhysicalEntity> entity ) :
	mLockedEntity( entity ),
	mLockedCamera( true )
{
		//Get the entitys position
	auto entityPos = sf::Vector2f( entity->getHitBox().left, entity->getHitBox().top ); 
		//Assign the cameras position (and desired position) to the entitys position
	mCameraPosition = GAME_TO_SCREEN * entityPos;
	mDesiredPosition = mCameraPosition;
}

Camera::~Camera(void) {}

void Camera::update(GameState* gameState_p, int milliseconds){
		//Get a reference to the render window
	auto& window = *WindowManager::get().getWindow();
		//If the camera is locked on an entity, update the cameras 
		//desired position to the entitys current position

	if( mLockedCamera && !mLockedEntity.expired() ) {
		auto physical = std::shared_ptr<PhysicalEntity>( mLockedEntity );
		auto entityPos = sf::Vector2f(physical->getHitBox().left, physical->getHitBox().top);
		mDesiredPosition = GAME_TO_SCREEN * entityPos;
	}
		//If the difference between the desired position and the 
		//current position is greater then 0.1, move the camera
		//'currentPanSpeed' in the direction pointing to 
		//the desired position
	if(	abs(mCameraPosition.x - mDesiredPosition.x) > 0.5 ||
		abs(mCameraPosition.y - mDesiredPosition.y) > 0.5) {
			//Take out the direction between current pos and
			//desired pos
		auto dirVect = mDesiredPosition - mCameraPosition;
			//Extend the x-component by 2 to remove the eliptic "camera leash zone"
		dirVect.x *= 2;
			//Calculate the lenght of the vector between the positions
		auto dirLenght = std::sqrt( dirVect.x * dirVect.x + dirVect.y * dirVect.y );
			//If lenght is longer then the currentPanSpeed, it should
			//move currentPanSpeed in dirVect's direction
		if(dirLenght > 0.1 ){
				//To make the camera slow down the closer it
				//gets to the player, calculate a panMultiplier
			auto panMultiplier = dirLenght * CAM_PAN_PERCENTAGE;
				//Normalize the direction vector
			dirVect.x = dirVect.x / dirLenght;
			dirVect.y = dirVect.y / dirLenght;
				//Then extend the normalized vector by the
				//current pan speed.
			dirVect.x *= panMultiplier;
			dirVect.y *= panMultiplier;
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
	auto camPos = mCameraPosition/* - sf::Vector2f(winSizeF.x * 0.5 , winSizeF.y * 0.5)*/;
		//Set the view
	window.setView( sf::View( camPos, winSizeF) );
}

void Camera::followEntity(std::shared_ptr<PhysicalEntity> entity){
	mLockedEntity = entity;
}

void Camera::panToPosition(sf::Vector2f position){
	unlockCamera();
	mDesiredPosition = position;
}

void Camera::snapToPosition(sf::Vector2f position){
	unlockCamera();
	mCameraPosition = position;
	mDesiredPosition = position;
}

void Camera::setPanSpeed(float moveSpeedPercentage){
	CAM_PAN_PERCENTAGE = moveSpeedPercentage;
}

void Camera::draw() const {}
