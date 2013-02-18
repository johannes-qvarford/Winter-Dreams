#include "OccludedEntity.h"
#include "FileStructure.h"

#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"

OccludedEntity::OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, bool startEnabled) :
	GraphicalEntity ( startEnabled ),
	mAlpha(1.0f),
	mShader(ResourceManager::get().getShader(FS_DIR_SHADERS + "Blend.frag")),
	mAnimation(animation),
	mHitBox(initialPosition)
{
}

OccludedEntity::~OccludedEntity(){
}

void OccludedEntity::setAlpha(float alpha){
	mAlpha=alpha;
}

void OccludedEntity::drawSelf(){
	auto renTex = WindowManager::get().getWindow();
	auto& states = *WindowManager::get().getStates();

	mShader->setParameter("Alpha",mAlpha / 100.f);
	sf::Sprite spr = mAnimation.getCurrentSprite();

#ifdef _DEBUG
	sf::Vertex vertices[] =
	{
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top), sf::Color::Green, sf::Vector2f( 0,  0)),
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top + mHitBox.height), sf::Color::Green, sf::Vector2f( 0, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top + mHitBox.height), sf::Color::Green, sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top), sf::Color::Green, sf::Vector2f(10,  0)),
	};

	auto& window = *WindowManager::get().getWindow();
//	auto& states = *WindowManager::get().getStates();

	//translate to screen coordinates
	states.transform *= GAME_TO_SCREEN;

	window.draw(vertices, 4, sf::Quads, states);
#else

	if (getEnabled()){
		renTex->draw(spr, states);
	}else{
		renTex->draw(spr, mShader.get());
	}
#endif
}

sf::FloatRect& OccludedEntity::getHitBox() {
	return mHitBox;
}