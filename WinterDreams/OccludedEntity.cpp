#include "OccludedEntity.h"
#include "FileStructure.h"

#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"

OccludedEntity::OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float alpha, int layer, bool startEnabled) :
	GraphicalEntity ( startEnabled ),
	mAlpha(alpha),
	mTargetAlpha(alpha),
	mLayer(layer),
	mShader(ResourceManager::get().getShader(FS_DIR_SHADERS + "Blend.frag")),
	mAnimation(animation),
	mHitBox(initialPosition)
{
}

OccludedEntity::~OccludedEntity(){
}

void OccludedEntity::setAlpha(float alpha){
	mTargetAlpha=alpha;
}

void OccludedEntity::drawSelf(){
	auto renTex = WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		mTargetAlpha = 1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		mTargetAlpha = 0.5;
	}

	static float xoffset = 0;
	static float yoffset = 0;

	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
	//	xoffset += 0.20;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
	//	xoffset -= 0.20;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
	//	yoffset += 0.20;
	//}
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
	//	yoffset -= 0.20;
	//}

	//std::cout << xoffset << " "  << yoffset << std::endl; 

//	std::cout << ol << std::endl;
	if (mTargetAlpha < mAlpha) { mAlpha-=0.02f; } 
	if (mTargetAlpha > mAlpha) { mAlpha+=0.02f; }
	mShader->setParameter("alpha",mAlpha);
//	mShader->setParameter("alpha",mAlpha / 100.f);
	sf::Sprite spr = mAnimation.getCurrentSprite();

#ifdef NEVER
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

//	static auto texture_sp = ResourceManager::get().getTexture(FS_DIR_OBJECTANIMATIONS + "occluder/bridgeMiddleTop.png");
//	spr.setTexture(*texture_sp);

	auto pos = GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top);

	spr.setPosition(pos);
	
	states.blendMode = sf::BlendAlpha;

	mTargetAlpha=1.0f;
	if (getEnabled() == false){
		states.shader = mShader.get();
		mTargetAlpha = 0.5f;
	}

	renTex->draw(spr, states);

	auto vertex = sf::Vertex(pos, sf::Color(0, 255, 0));
	renTex->draw(&vertex, 1, sf::Points);

#endif
}

sf::FloatRect& OccludedEntity::getHitBox() {
	return mHitBox;
}

int OccludedEntity::getLayer() {
	return mLayer;
}