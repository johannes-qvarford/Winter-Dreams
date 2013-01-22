#ifndef INCLUDED_ANIMATION
#define INCLUDED_ANIMATION

#include <SFML\Graphics.hpp>
#include <string>

class Animation{
public:
	/*
	//	An animation needs the following:
	//	A filepath to it's intended texture. (filePath)
	//	A sf::FloatRect that descries the size of the sprite.
	//	Knowledge about how many spirtes the texture contains. (numberOfSprites)
	//	Knowledge about how many frames each sprite is displayed. (framesPerSprite)
	*/
	Animation(const std::string filePath, const sf::FloatRect spriteSize, const unsigned int numberOfSprites, const unsigned int framesPerSprite);
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
	sf::Texture* mTexture_p;		//The texture from which sprites are displayed.
	sf::Sprite&  mSprite;			//The current sprite from mTexture.
	sf::FloatRect mSpirteSize		//The size, in pixels, of the sprite.
	unsigned int mNumberOfSprites;	//How many spirtes mTexture contains.
	unsigned int mFramesPerSprite;	//How many frames should each sprite be displayed.
	unsigned int mCurrentSprite;	//Which sprite (by index) is currently displayed.
	
};

#endif