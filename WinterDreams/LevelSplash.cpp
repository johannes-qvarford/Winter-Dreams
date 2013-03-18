#include "PrecompiledHeader.h"
#include "LevelSplash.h"

#include "ResourceManager.h"
#include "WindowManager.h"
#include "FileStructure.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "InputManager.h"
#include "Camera.h"
#include "GameToScreen.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>



LevelSplash::LevelSplash(const std::string& splashFileName, const int lifeTime, int fadeoutTime, bool startEnabled) : 
	Script( startEnabled ),
	mLifeTime( lifeTime ),
	mAlpha( 1.f ),
	mFadeOutTime( fadeoutTime ),
	mFadeOutTimeCurrent( 0 ),
	mFirstUpdate( true ),
	mLevelSplashTexture( ResourceManager::get().getTexture( FS_DIR_BACKGROUNDS + "LevelSplash/" + splashFileName ) ),
	mLevelSplash( *mLevelSplashTexture ),
	mBlendShader( ResourceManager::get().getShader( FS_DIR_SHADERS + "Blend.frag" ) )
{
}

LevelSplash::~LevelSplash()
{}


void LevelSplash::update(SubLevel* subLevel_p) {
	if(getEnabled() == false)
		return;

	if( mFirstUpdate == true ){
		InputManager::get().lockInput();
		mFirstUpdate = false;
	}

	auto& win = *WindowManager::get().getRenderWindow();

	auto pos = sf::Vector2f( 0,0 );
	mLevelSplash.setScale( static_cast<float>(win.getSize().x) / 1920.f, static_cast<float>(win.getSize().y) / 1080.f ) ; 

//	mLevelSplash.setOrigin( mLevelSplashTexture->getSize().x / 2, mLevelSplashTexture->getSize().y / 2 );
 	mLevelSplash.setPosition(  pos );
	
	if( mLifeTime > 0 ){
		InputManager::get().lockInput();
		--mLifeTime;
	}
	else if( mFadeOutTimeCurrent < mFadeOutTime){
		InputManager::get().unlockInput();
		++mFadeOutTimeCurrent;
		mAlpha = 1.f - static_cast<float>(mFadeOutTimeCurrent) / static_cast<float>(mFadeOutTime);
	}
	else {
		setAlive( false );
	}
}

void LevelSplash::draw() const{
	auto& win = *WindowManager::get().getRenderWindow();
	auto states = *WindowManager::get().getStates();

	states.blendMode = sf::BlendAlpha;
	states.shader = mBlendShader.get();
	mBlendShader->setParameter( "alpha", mAlpha );

	win.draw( mLevelSplash, states );
}
