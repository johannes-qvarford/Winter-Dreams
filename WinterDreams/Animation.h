#ifndef INCLUDED_ANIMATION
#define INCLUDED_ANIMATION

#include <string>
#include <memory>

namespace sf{
	class Sprite;
	class Texture;
};


class Animation{
public:
	////////////////////////////////////////////////////////////
	// / An animation needs the following:
	// / A filepath to it's intended texture. (filePath)
	// / Knowledge about how many pixels wide the sprite is. Minimum 1.
	// / Knowledge about how many pixels high the sprite is. Minimum 1.
	// / Knowledge about how many spirtes the texture contains. (numberOfSprites) Minimum 1.
	// / Knowledge about how many frames each sprite is displayed. (framesPerSprite) Minimum 1.
	////////////////////////////////////////////////////////////
	Animation(const std::string filePath, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int numberOfSprites, unsigned int framesPerSprite);
	Animation(const Animation& animation);
	////////////////////////////////////////////////////////////
	// /The texture's shared pointer leaves scope. 
	////////////////////////////////////////////////////////////
	~Animation();
	////////////////////////////////////////////////////////////
	// / getCurrentSpirte returns the current sprite from the sf::Texture
	// / Each time getCurrentSprite is called it updates how many frames the
	// / current sprite's been displayed
	////////////////////////////////////////////////////////////
	sf::Sprite getCurrentSprite();
	////////////////////////////////////////////////////////////
	// / Resets the animation to display the first sprite in the spritesheet
	// / Usually used once when another animation is assigned to an object.
	////////////////////////////////////////////////////////////
	void resetAnimation();
private:
	std::shared_ptr<sf::Texture> mTexture_p; //The texture from which sprites are displayed.
	sf::Sprite  mSprite;			//The current sprite from mTexture.
	unsigned int mSpriteWidth;		//The width, in pixels, of the sprite.
	unsigned int mSpriteHeight;		//The height, in pixels, of the sprite.
	unsigned int mNumberOfSprites;	//How many spirtes mTexture contains.
	unsigned int mFramesPerSprite;	//How many frames should each sprite be displayed.
	unsigned int mCurrentSprite;	//Which sprite (by index) is currently displayed.
	unsigned int mCurrentFrame;		//Which frame (not sprite) is currently active.

};

#endif