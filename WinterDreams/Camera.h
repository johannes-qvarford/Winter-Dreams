#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include "Script.h"
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\View.hpp>
class PhysicalEntity;

////////////////////////////////////////////////////////////
// /The class for controlling the in-game view (i.e the camera).
// /When constructing the camera, either give it a 
// /PhysicalEntity-pointer (it will then follow the entity)
// /or construct it with a sf::Vector2f, in which case the
// /vector describes the cameras centerpoint.
////////////////////////////////////////////////////////////
class Camera : public Script{
public:
	////////////////////////////////////////////////////////////
	// /Assigns the camera to look at a certain position.
	// /The argument position will be the center of the view
	////////////////////////////////////////////////////////////
	Camera(sf::Vector2f centerPosition);
	////////////////////////////////////////////////////////////
	// /Assign the camera to follow an entity. This will lock
	// /the camera to the entity.
	////////////////////////////////////////////////////////////
	Camera(std::shared_ptr<PhysicalEntity>(entity) );
	////////////////////////////////////////////////////////////
	// /If the camera follows a physical entity, it does not
	// /have responsability over the entity.
	////////////////////////////////////////////////////////////
	~Camera();
	////////////////////////////////////////////////////////////
	// /Updates the widows view to match the camera.
	// /Should be called once per update. 
	////////////////////////////////////////////////////////////
	void update(GameState* gameState_p, int milliseconds);
	////////////////////////////////////////////////////////////
	// /Locks the camera on a physical entity. The center of
	// /the cameras view will match the entitys position.
	// /				
	// /					 IMPORTANT!
	// /This will NOT automaticlly lock the camera to the entity
	////////////////////////////////////////////////////////////
	void followEntity(std::shared_ptr<PhysicalEntity> entity);
	////////////////////////////////////////////////////////////
	// /The camera will pan to the argument position no faster
	// /then the cameras current pan speed. 
	// /The argument position will be the center of the view
	// /This function will automaticlly unlock the camera
	////////////////////////////////////////////////////////////
	void panToPosition(sf::Vector2f position);
	////////////////////////////////////////////////////////////
	// /The camera instantainiously moves to the argument
	// /position.
	// /The argument position will be the center of the view
	// /This function will automaticlly unlock the camera
	////////////////////////////////////////////////////////////
	void snapToPosition(sf::Vector2f position);
	////////////////////////////////////////////////////////////
	// /Sets the cameras maximum allowed pan speed.
	// /The camera moves % of the distance between the cam
	// /and the avatar each frame.
	////////////////////////////////////////////////////////////
	void setPanSpeed(float moveSpeedPercentage);
	////////////////////////////////////////////////////////////
	// /The camera will not follow the locked entity anymore
	////////////////////////////////////////////////////////////
	void unlockCamera() { mLockedCamera = false; }
	////////////////////////////////////////////////////////////
	// /The camera will follow the locked entity again if the
	// /is  locked entity
	////////////////////////////////////////////////////////////
	void lockCamera()	{ mLockedCamera = true;	 }

	void draw() const;
private:
	std::shared_ptr<PhysicalEntity>	mLockedEntity;  // A pointer to the locked entity. Will be NULL if no entity is locked.
	sf::Vector2f					mCameraPosition;  // The cameras position, in screen  
	sf::Vector2f					mDesiredPosition;// The position to which the came wish to move
	bool							mLockedCamera;
};

#endif