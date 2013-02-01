#include "CollisionZone.h"

CollisionZone::CollisionZone(bool startsEnabled, sf::Rect<float> HitBox, bool onceCollisionCheck):
	PhysicalEntity(startsEnabled),
	mHitBox(HitBox),
	mOnce(onceCollisionCheck)
{
}

void CollisionZone::onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection){
	if (mOnce == true){
		setAlive(false);
	}
}

void CollisionZone::update(GameState* gameState){

}

sf::Rect<float>& CollisionZone::getHitBox(){
	return mHitBox;
}

sf::Vector2i CollisionZone::getDirection(){
	return sf::Vector2i(1, 0);
}
