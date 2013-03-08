#include "OccludedEntity.h"
#include "FileStructure.h"

#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"

#include <algorithm>

OccludedEntity::OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float enabledOpacity, float disabledOpacity, int fadeTime, int layer, bool startEnabled) :
	GraphicalEntity ( startEnabled ),
	mEnabledAlpha(enabledOpacity),
	mDisabledAlpha(disabledOpacity),
	mLayer(layer),
	mShader(ResourceManager::get().getShader(FS_DIR_SHADERS + "Blend.frag")),
	mAnimation(animation),
	mHitBox(initialPosition)
{
	if (getEnabled()){
		mCurrentAlpha=enabledOpacity;
	} else {
		mCurrentAlpha=disabledOpacity;
	}

	mFadeTime=static_cast<float>(fadeTime / (1000.f / 60.f));
}

OccludedEntity::~OccludedEntity(){
}

void OccludedEntity::drawSelf(){
	auto renTex = WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

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

	if (getEnabled()){
		if (mCurrentAlpha < mEnabledAlpha){
			mCurrentAlpha += 1.f / (mFadeTime + 0.001f);
			mCurrentAlpha = std::min(mCurrentAlpha, mEnabledAlpha);
		} else if (mCurrentAlpha > mEnabledAlpha){
			mCurrentAlpha -= 1.f / (mFadeTime + 0.001f);
			mCurrentAlpha = std::max(mCurrentAlpha, mEnabledAlpha);
		}
	} else {
		if (mCurrentAlpha > mDisabledAlpha){
			mCurrentAlpha -= 1.f / (mFadeTime + 0.001f);
			mCurrentAlpha = std::max(mCurrentAlpha, mDisabledAlpha);
		} else if (mCurrentAlpha < mDisabledAlpha){
			mCurrentAlpha += 1.f / (mFadeTime + 0.001f);
			mCurrentAlpha = std::min(mCurrentAlpha, mDisabledAlpha);
		}
	}

	mShader->setParameter("alpha",mCurrentAlpha);
	sf::Sprite spr = mAnimation.getCurrentSprite();

	auto pos = GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top);
	pos.x = float(int(pos.x));//floor
	pos.y = float(int(pos.y));//floor

	spr.setPosition(pos);
	
	states.blendMode = sf::BlendAlpha;

//	if (getEnabled() == false){
		states.shader = mShader.get();
//	}

	renTex->draw(spr, states);

	auto vertex = sf::Vertex(pos, sf::Color(255, 0, 0));
	renTex->draw(&vertex, 1, sf::Points);

}

sf::FloatRect& OccludedEntity::getHitBox() {
	return mHitBox;
}

int OccludedEntity::getLayer() {
	return mLayer;
}