#include "CollisionZone.h"

CollisionZone::CollisionZone(bool startsEnabled, sf::Rect<float> HitBox, bool onceCollisionCheck):
	PhysicalEntity(startsEnabled),
	mHitBox(HitBox),
	mOnce(onceCollisionCheck)
{
}

void CollisionZone::onCollision(PhysicalEntity* pe_p, const sf::Rect<float>& intersection){
	if (mOnce == true){
		setEnabled(false);
	}
}

void CollisionZone::update(SubLevel* subLevel){

}

sf::Rect<float>& CollisionZone::getHitBox(){
	return mHitBox;
}