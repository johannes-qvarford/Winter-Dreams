#ifndef INCLUDED_ANIMATION
#define INCLUDED_ANIMATION

#include <string>

class Animation{
public:
	/*
	//	An animation needs the following:
	//	A filepath to it's intended texture. (filePath)
	//	Knowledge about how many spirtes the texture contains. (numberOfSprites)
	//	Knowledge about how many frames each sprite is displayed. (framesPerSprite)
	*/
	Animation(const std::string filePath, const unsigned int numberOfSprites, const unsigned int framesPerSprite);
	Animation(const Animation& animation);
	/*
	//	The destructor does not delete it's associated sf::Texture
	*/
	~Animation();
	/*
	//	getCurrentSpirte returns the current sprite from the sf::Texture.
	//	Each time getCurrentSprite is called it updates how many frames the
	//	current sprite's been displayed.
	*/
	sf::Sprite& getCurrentSprite();
	
private:
	sf::Texture* mTexture;			//The texture from which sprites are displayed.
	sf::Sprite&  mSprite;			//The current sprite from mTexture.
	unsigned int mNumberOfSprites;	//How many spirtes mTexture contains.
	unsigned int mFramesPerSprite;	//How many frames should each sprite be displayed.
	unsigned int mCurrentSprite;	//Which sprite (by index) is currently displayed.
	
};

#endif