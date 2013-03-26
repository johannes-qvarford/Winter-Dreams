#include "OccludedEntity.h"
#include "FileStructure.h"

#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"

#include <algorithm>

OccludedEntity::OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float enabledOpacity, float disabledOpacity, int fadeTime, int layer, bool startEnabled) :
	Entity(startEnabled),
	BaseHitBoxHaveable(initialPosition),
	mEnabledAlpha(enabledOpacity),
	mDisabledAlpha(disabledOpacity),
	mLayer(layer),
	mShader(ResourceManager::get().getShader(FS_DIR_SHADERS + "Blend.frag")),
	mAnimation(animation)
{
	if (getEnabled()){
		mCurrentAlpha=enabledOpacity;
	} else {
		mCurrentAlpha=disabledOpacity;
	}

	if(layer != 150)
		int a = 3;

	mFadeTime=static_cast<float>(fadeTime / (1000.f / 60.f));
}


void OccludedEntity::draw(){
	auto renTex = WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	static float xoffset = 0;
	static float yoffset = 0;

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
	states.shader = mShader.get();
	renTex->draw(spr, states);
}

int OccludedEntity::getLayer() {
	return mLayer;
}