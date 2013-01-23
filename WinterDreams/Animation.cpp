#include "Animation.h"
#include "ResourceManager.h"
#include <SFML\Graphics.hpp>
////////////////////////////////////////////////////////////
// /Constructor.
// /Simply assigns the arguments to the correct member variable.
////////////////////////////////////////////////////////////
Animation::Animation(const std::string filePath, 
					 unsigned int spriteWidth,
					 unsigned int spriteHeight,
					 unsigned int numberOfSprites, 
					 unsigned int framesPerSprite) :
	mNumberOfSprites(numberOfSprites),
	mFramesPerSprite(framesPerSprite),
	mCurrentFrame(0),
	mCurrentSprite(0),
	mSpriteWidth(spriteWidth),
	mSpriteHeight(spriteHeight),
	mTexture_p(ResourceManager::get().getTexture(filePath) ),
	mSprite()
{
		//Binds the texture to the spirte.
	mSprite.setTexture(*mTexture_p);
	mSprite.setTextureRect(sf::IntRect(0,0, mSpriteWidth, mSpriteHeight));
}
////////////////////////////////////////////////////////////
// /Copy constructor for animation. 
////////////////////////////////////////////////////////////
Animation::Animation(const Animation& animation) :
	mNumberOfSprites(animation.mNumberOfSprites),
	mFramesPerSprite(animation.mFramesPerSprite),
	mCurrentFrame(animation.mCurrentFrame),
	mCurrentSprite(animation.mCurrentSprite),
	mSpriteWidth(animation.mSpriteWidth),
	mSpriteHeight(animation.mSpriteHeight),
	mTexture_p(animation.mTexture_p),
	mSprite(animation.mSprite)
{}
////////////////////////////////////////////////////////////
// /Destructor. No action.
////////////////////////////////////////////////////////////
Animation::~Animation() {}
////////////////////////////////////////////////////////////
// /Returns the current sprite from the sprite sheet.
////////////////////////////////////////////////////////////
sf::Sprite Animation::getCurrentSprite(){
	unsigned int left = mSpriteWidth * mCurrentSprite;
	unsigned int top = 0;
		//Calculates the left and top bounds of the rect.
	while(left >= mTexture_p->getSize().x){
		left -= mTexture_p->getSize().x;
		top += mSpriteHeight;
	}
		//Assigns the TextureRect
	mSprite.setTextureRect(sf::IntRect( left, top, mSpriteWidth, mSpriteHeight ) );
		//Advances frame count. If the sprite's been visible as long as it should,
		//the next sprite in the sheet is selected and the frame count i reset.
	++mCurrentFrame;
	if( mCurrentFrame >= mFramesPerSprite ){
		++mCurrentSprite;
		mCurrentFrame = 0;
	}

	return mSprite;
}

void Animation::resetAnimation(){
		//Resets the frame count and sprite count.
	mCurrentSprite= 0;
	mCurrentFrame = 0;	
}
