#ifndef INCLUDED_RESOURCEMANAGER
#define INCLUDED_RESOURCEMANAGER

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
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

private:
	
	ResourceManager();												
	
	std::map<std::string, std::weak_ptr<sf::Texture>> mTextureMap;
	
	std::shared_ptr<sf::Texture> loadTexture(const std::string& fn);
	
	std::map<std::string, std::weak_ptr<sf::SoundBuffer>> mSoundBufferMap;
	
	std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& key);
	
	std::map<std::string, std::weak_ptr<sf::Font>> f
	
	std::shared_ptr<sf::Font> loadFont(const std::string& key);

	std::string mFilePath;
};

#endif