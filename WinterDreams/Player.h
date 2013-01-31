#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

#include "PlayerRelated.h"
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Window\Keyboard.hpp>
#include "Inventory.h"
#include "Animation.h"
#include <map>
#include "Animation.h"

class Inventory;
class GameState;

///////////////////////////////////////////////////////
// /This enum lists the different movement modes avalible to the
// /avatar.
///////////////////////////////////////////////////////
enum MovementMode {
	NORMAL, FLYING, ICEZONE
};

////////////////////////////////////////////////////////
// /Concrete class representing the players character.
////////////////////////////////////////////////////////
class Player : public PlayerRelated {
public:
	//////////////////////////////////////////////////////
	// /Players constructor needs a sf::Vector2f to assign
	// /the avatars initial position. 
	//////////////////////////////////////////////////////
	Player(sf::Vector2f initialPosition);
	//////////////////////////////////////////////////////
	// /Players destructor deletes its associated Inventory.
	//////////////////////////////////////////////////////
	~Player();
	//////////////////////////////////////////////////////
	// /Defines what actions should be performed on the avatar
	// /each time update is called. Used by GameState to perform a
	// /series of actions each update-frame.
	//////////////////////////////////////////////////////
	void update(GameState* gameState_p, int milliseconds);
	//////////////////////////////////////////////////////
	// /Defines how the avatar is drawn onto the window.
	//////////////////////////////////////////////////////
	void drawSelf();
	//////////////////////////////////////////////////////
	// /Returns the avatar's hitbox.
	// /Used primarily to check for collisions with other physical entities.
	//////////////////////////////////////////////////////
	sf::FloatRect& getHitBox();
	//////////////////////////////////////////////////////
	// /Defines what actions should be performed if the avatar collides
	// /with the argument PhysicalEntity. 
	//
	//					!!!!IMPORTANT NOTICE!!!
	//
	// /This function defines how the avatar should effect
	// /the entity collided with. NOT how the player should be effected.
	//////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection);
	//////////////////////////////////////////////////////
	// /Returns the avatars's current position as a 
	// /const sf::Vector2f-reference.
	// /This function cannot be used to affect the avatar's position.
	//////////////////////////////////////////////////////
	sf::Vector2f getPosition();
	//////////////////////////////////////////////////////
	// /Assigns the avatars's positonto the position
	// /described by the argument sf::Vector2f.
	// /Hence, this does not change the avatar's position by
	// /an amount, it overrides it's current position and sets the
	// /argument as it's new current position.
	//////////////////////////////////////////////////////
	void setPosition(const sf::Vector2f& position);
	//////////////////////////////////////////////////////
	// /Adjust the avatars current position by the argument vector.
	//////////////////////////////////////////////////////
	void adjustPosition(const sf::Vector2f& positionAdjustment);
	//////////////////////////////////////////////////////
	// /Returns the avatars current health.
	//////////////////////////////////////////////////////
	int getCurrentHealth() const;
	//////////////////////////////////////////////////////
	// /Sets the avatars current health to the argument amount.
	// /This function overrides the avatars current health and assigns 
	// /the avatars new current health to the argument amount.
	//////////////////////////////////////////////////////
	void setCurrentHealth(const int health);
	//////////////////////////////////////////////////////
	// /Adjusts the characters current health by the argument amount by
	// /adding the argument number to the avatars current health. If the
	// /result is greater then 9, the avatars current health will be assigned
	// /to 9.
	// /To reduce the avatars health, send a negative number as argument.
	//////////////////////////////////////////////////////
	void adjustCurrentHealth(const int healthAdjustment);
	//////////////////////////////////////////////////////
	// /Returns a const Inventory-reference to the players Inventory. The
	// /returned reference cannot be used to affect the players inventory.
	//////////////////////////////////////////////////////
	const Inventory& getInventory() const;
	//////////////////////////////////////////////////////
	// /Returns a pointer to the players Inventory. This pointer can be used
	// /to call functions and affect the players Inventory.
	// /If the player should be assigned a new item, or an item should be removed,
	// /call function "giveItem" or "takeItem" on the returned pointer.
	//////////////////////////////////////////////////////
	Inventory*	changeInventory();
	//////////////////////////////////////////////////////
	// /Returns the avatars current movement mode.
	// /This function cannot be used to affect the avatars current movement mode.
	//////////////////////////////////////////////////////
	MovementMode getCurrentMovementMode();
	//////////////////////////////////////////////////////
	// /Changes the avatars current movement mode to the argument mode.
	// /The enum listing the different movement modes are availible at the top of Player.h
	//////////////////////////////////////////////////////
	void setMovementMode(MovementMode movementMode);
	//////////////////////////////////////////////////////
	// /Returns the avatars direction
	// /
	//////////////////////////////////////////////////////
	sf::Vector2i getDirection();
private:
	Animation*						 mCurrentAnimation_p; //The avatar's current animation
	std::map<std::string, Animation> mAnimationMap;		//The avatar's animation map
	sf::FloatRect					 mHitBox;			//The avatar's current hitbox
	int								 mHealth;			//The avatar's current health
	MovementMode					 mMovementMode;		//The avatar's current movementMode
	Inventory						 mInventory;		//The avatar's inventory
	sf::Vector2i					 mDirection;		//The avatar's direction
};

#endif