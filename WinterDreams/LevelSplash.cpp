#include "PrecompiledHeader.h"
#include "LevelSplash.h"

#include "ResourceManager.h"
#include "WindowManager.h"
#include "FileStructure.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "InputManager.h"
#include "Camera.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Window.hpp>



LevelSplash::LevelSplash(const std::string& splashFileName, const int lifeTime, const int fadeoutTime, const int fadeinTime, bool startEnabled) : 
	Script( startEnabled ),
	mLifeTime( lifeTime ),
	mAlpha( 0.f ),
	mFadeInTime( fadeinTime ),
	mFadeInTimeCurrent( 0 ),
	mFadeOutTime( fadeoutTime ),
	mFadeOutTimeCurrent( 0 ),
	mLevelSplashTexture( ResourceManager::get().getTexture( FS_DIR_LOADINGSCREEN + "LevelSplash/" + splashFileName ) ),
	mLevelSplash( *mLevelSplashTexture ),
	mBlendShader( ResourceManager::get().getShader( FS_DIR_SHADERS + "Blend.frag" ) )
{
	InputManager::get().lockInput();
}

LevelSplash::~LevelSplash()
{}

void LevelSplash::draw() const {}

void LevelSplash::update(SubLevel* subLevel_p) {
	auto& cam = subLevel_p->getLevel()->getCamera();
	
	mLevelSplash.setOrigin( sf::Vector2f( mLevelSplashTexture->getSize() / unsigned(2) ) );
	mLevelSplash.setPosition( cam->getPosition() );


	if( mFadeInTime != mFadeInTimeCurrent ){
		++mFadeInTimeCurrent;
		mAlpha = mFadeInTimeCurrent / mFadeInTime;
	}
	else if( mLifeTime > 0 ){
		--mLifeTime;
	}
	else if( mFadeOutTime != mFadeInTimeCurrent){
		++mFadeOutTime;
		mAlpha = mFadeOutTimeCurrent / mFadeOutTime;
	}
	else {
		setAlive( false );
		InputManager::get().unlockInput();
	}
}

void LevelSplash::draw() const{
	auto& win = *WindowManager::get().getWindow();


	mBlendShader->setParameter( "alpha", mAlpha );

	win.draw( mLevelSplash, mBlendShader.get() );
}