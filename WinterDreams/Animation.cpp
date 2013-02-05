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
					 unsigned int framesPerSprite,
					 unsigned int xOffset,
					 unsigned int yOffset) :
	mNumberOfSprites(numberOfSprites),
	mFramesPerSprite(framesPerSprite),
	mSpriteWidth( spriteWidth ),
	mSpriteHeight( spriteHeight ),
	mCurrentFrame(0),
	mCurrentSprite(0),
	mTexture_p(ResourceManager::get().getTexture(filePath) ),
	mSprite(),
	mEndOfAnimation( false )
{
		//Binds the sprite to the texture.
	mSprite.setTexture(*mTexture_p);
	mSprite.setTextureRect(sf::IntRect(0,0, spriteWidth, spriteHeight));
	mSprite.setOrigin( static_cast<float>(xOffset), static_cast<float>(yOffset) );
}
////////////////////////////////////////////////////////////
// /Copy constructor for animation. 
////////////////////////////////////////////////////////////
Animation::Animation(const Animation& animation) :
	mNumberOfSprites(animation.mNumberOfSprites),
	mFramesPerSprite(animation.mFramesPerSprite),
	mCurrentFrame(animation.mCurrentFrame),
	mCurrentSprite(animation.mCurrentSprite),
	mSpriteHeight( animation.mSpriteHeight ),
	mSpriteWidth( animation.mSpriteWidth ),
	mTexture_p(animation.mTexture_p),
	mSprite(animation.mSprite),
	mEndOfAnimation( animation.mEndOfAnimation )
{}
////////////////////////////////////////////////////////////
// /Destructor. No action.
////////////////////////////////////////////////////////////
Animation::~Animation() {}
////////////////////////////////////////////////////////////
// /Returns the current sprite from the sprite sheet.
////////////////////////////////////////////////////////////
sf::Sprite Animation::getCurrentSprite(){
	return mSprite;
}

void Animation::setPosition(const sf::Vector2f& position) {
	mSprite.setPosition( position );
}

void Animation::resetAnimation(){
		//Resets the frame count and sprite count.
	mCurrentSprite= 0;
	mCurrentFrame = 0;	
}

void Animation::updateAnimation(){
	unsigned int left = mSpriteWidth * mCurrentSprite;
	////////////////////////////////////////////////////////
	//Below is currently inactive since we use single
	//row sprite sheets
	////////////////////////////////////////////////////////
	unsigned int top = 0;
	//	//Calculates the left and top bounds of the rect.
	//while(left >= mTexture_p->getSize().x){
	//	left -= mTexture_p->getSize().x;
	//	top += mSpriteHeight;
	//}
	////////////////////////////////////////////////////////
		//Assigns the TextureRect
	mSprite.setTextureRect(sf::IntRect( left, top, mSpriteWidth, mSpriteHeight ) );
		//Advances frame count. If the sprite's been visible as long as it should,
		//the next sprite in the sheet is selected and the frame count i reset.
	++mCurrentFrame;
	if( mCurrentFrame >= mFramesPerSprite ){
		++mCurrentSprite;
			//If the current sprite number is higher then the 
			//number of sprites in the sprite sheet, it resets.
		if(mCurrentSprite == mNumberOfSprites ) {
			mCurrentSprite = 0;
			mEndOfAnimation = true;
		}
		else {
			mEndOfAnimation = false;
		}
		mCurrentFrame = 0;
	}
}

bool Animation::endOfAnimation() const{
	return mEndOfAnimation;
}
