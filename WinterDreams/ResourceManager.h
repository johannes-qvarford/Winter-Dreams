#ifndef INCLUDED_RESOURCEMANAGER
#define INCLUDED_RESOURCEMANAGER

#include <memory>
#include <map>
#include <string>

class ResourceManager{
public:
 	static ResourceManager& get();									//returns or creates the Resource Manager.
	
	std::shared_ptr<sf::Texture> getTexture(const std::string& key);			//if requested Texture exists, returns a shared pointer; else tries loading the request into memory and returns one.
	
	std::shared_ptr<sf::SoundBuffer> getSoundBuffer(const std::string& key);	//as getTexture, but does the same with a sound buffer.
	
	std::shared_ptr<sf::Font> getFont(const std::string& key);					//if you don't understand this by yourself by now you should go do something else.

private:
	
	ResourceManager();												
	
	std::map<std::string, std::weak_ptr<sf::Texture>> mTextureMap;
	
	std::shared_ptr<sf::Texture> loadTexture(const std::string& fn);
	
	std::map<std::string, std::weak_ptr<sf::SoundBuffer>> mSoundBufferMap;
	
	std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& key);
	
	std::map<std::string, std::weak_ptr<sf::Font>> mFontMap;
	
	std::shared_ptr<sf::Font> loadFont(const std::string& key);
};

#endif