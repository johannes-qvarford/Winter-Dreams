#ifndef INCLUDED_RESOURCEMANAGER
#define INCLUDED_RESOURCEMANAGER

#include "FileStructure.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <sfTheora\Video.h>
#include <memory>
#include <map>
#include <string>
#include <cassert>

class ResourceManager{
public:
	//////////////////////////////////////////////////////////
	// /Returns or creates the Resource Manager
	//////////////////////////////////////////////////////////
 	static ResourceManager& get();
	//////////////////////////////////////////////////////////
	// /If requested Texture exists, returns a shared pointer
	// /Else, tries loading the texture into memory and
	// /Returns one
	//////////////////////////////////////////////////////////
	std::shared_ptr<sf::Texture> getTexture(const std::string& key);
	//////////////////////////////////////////////////////////
	// /As getTexture, but with a Sound Buffer
	//////////////////////////////////////////////////////////
	std::shared_ptr<sf::SoundBuffer> getSoundBuffer(const std::string& key);
	//////////////////////////////////////////////////////////
	// /If you can't figure this out by yourself you should go
	//////////////////////////////////////////////////////////
	std::shared_ptr<sf::Font> getFont(const std::string& key);
	//////////////////////////////////////////////////////////
	// /REALLY
	//////////////////////////////////////////////////////////
	std::shared_ptr<sf::Shader> getShader(const std::string& key);
	//////////////////////////////////////////////////////////
	// /Returns a shared pointer to a dynamicly loaded video.
	// /The video is handled via shared pointers so the function
	// /that recieves the video does not need to take
	// /responsability over the video.
	//////////////////////////////////////////////////////////
	std::shared_ptr<sftheora::Video> getVideo(const std::string& key);

private:
	
	ResourceManager();			//Singleton pattern, private constructor
	ResourceManager(const ResourceManager& rm);				//No copies
	ResourceManager& operator=(const ResourceManager& rm);	//No copies
	
	std::map<std::string, std::weak_ptr<sf::Texture>> mTextureMap;
	
	std::shared_ptr<sf::Texture> loadTexture(const std::string& fn);
	
	std::map<std::string, std::weak_ptr<sf::SoundBuffer>> mSoundBufferMap;
	
	std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& key);
	
	std::map<std::string, std::weak_ptr<sf::Font>> mFontMap;
	
	std::shared_ptr<sf::Font> loadFont(const std::string& key);

	std::map<std::string, std::weak_ptr<sf::Shader>> mShaderMap;

	std::shared_ptr<sf::Shader> loadShader(const std::string& key);

	std::map<std::string, std::weak_ptr<sftheora::Video>> mVideoMap;

	std::shared_ptr<sftheora::Video> loadVideo(const std::string& key);

	std::string mFilePath;
};

#endif
