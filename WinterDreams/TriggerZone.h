#ifndef INCLUDED_TRIGGERZONE
#define INCLUDED_TRIGGERZONE

#include "PhysicalEntity.h"
#include <SFML/Graphics/Rect.hpp>

/*
//	Trigger zones are entities with a set collision box.
*/
class TriggerZone : public PhysicalEntity {
public:

	/*
	//	Create a TriggerZone at (x,y) with a width and a height.
	*/
	TriggerZone(float x, float y, float width, float height); 

	/*
	//	Get the TriggerZones hitbox.
	*/
	const sf::Rect<float>& getHitBox() const;

private:

	sf::Rect<float> mHitBox;
};

#endif