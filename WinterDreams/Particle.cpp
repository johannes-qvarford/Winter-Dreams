#include "Particle.h"
#include "WindowManager.h"
#include "ResourceManager.h"

Particle::Particle(sf::Vector2f position, sf::Vector2f speed, int lifetime, std::string filePath):
mPosition(position),
mSpeed(speed),
mLife(lifetime)
{
	mTexture = ResourceManager::get().getTexture(filePath);
	mSprite.setTexture(*mTexture);
}

void Particle::update(){
	--mLife;
	mPosition+=mSpeed;
	mSprite.setPosition(mPosition.x,mPosition.y);
}

void Particle::drawSelf(){
	WindowManager::get().getWindow()->draw(mSprite);
}

void Particle::setSpeed(const sf::Vector2f& speedVector){
	mSpeed=speedVector;
}

void Particle::adjustSpeed(const sf::Vector2f& adjustVector){
	mSpeed+=adjustVector;
}

bool Particle::getDead(){
	return mLife;
}