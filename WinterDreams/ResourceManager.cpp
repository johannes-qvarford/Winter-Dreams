#include "ResourceManager.h"

#include <fstream>

ResourceManager::ResourceManager()
{}

//Returns the singular instance of ResouceManager
ResourceManager& ResourceManager::get(){
	static ResourceManager manager;
	return manager;
}
/*	Checks if the texture corresponding to "key" is loaded or not.
	
	If it is loaded, a shared_ptr to the texture is returned.
	If it isn't loaded, the resource is loaded and a shared pointer to the 
	texture is returned. A weak_ptr is then stored in a map, mapping key to the
	texture.																	*/
std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string &key){
		//Creates an itarator to the element corresponding to key.
	std::map<std::string, std::weak_ptr<sf::Texture> >::iterator iter;
	iter = mTextureMap.find(key);
		//If the element the iterator points to is in the map, and it element
		//it points to is not expiered, return a share_ptr to the element.
	if( iter != mTextureMap.end() && !iter->second.expired() ){
		return iter->second.lock();
	}
		//Else, load the element described by the key. 
	else{
		std::shared_ptr<sf::Texture> tex (new sf::Texture);

		//std::ifstream file(key.c_str());

		//if(!file) {
		//	assert(false);
		//}

		//std::vector<char> buffer;

		//file.seekg(0, std::ios::end);
		//auto size = file.tellg();
		//file.seekg(0, std::ios::beg);
		//buffer.resize(size);
		//file.read(&buffer[0], size);

		auto img = sf::Image();
		if(!img.loadFromFile(key)) {
			assert(0);
		}

		if(!tex->loadFromImage(img)) {
			assert(false);
		}

		//if (!tex->loadFromFile(mFilePath + key)){
		//		//If load fails, error.
		//	assert(false);
		//}

		//Then return a shared_ptr to the element just loaded.
		mTextureMap.insert(std::pair<std::string, std::weak_ptr<sf::Texture> >(key, tex) );
		return tex;
	}
}
/*	Checks if the soundBuffer corresponding to "key" is loaded or not.
	
	If it is loaded, a shared_ptr to the soundBuffer is returned.
	If it isn't loaded, the resource is loaded and a shared pointer to the 
	soundBuffer is returned. A weak_ptr is then stored in a map, mapping key to the
	soundBuffer.
	Se function getTexture for a descirption of each function.					*/
std::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const std::string &key){
	std::map<std::string, std::weak_ptr<sf::SoundBuffer> >::iterator iter;
	iter = mSoundBufferMap.find(key);

	if( iter != mSoundBufferMap.end() && !iter->second.expired() ){
		return iter->second.lock();
	}
	else{
		std::shared_ptr<sf::SoundBuffer> sndbuf (new sf::SoundBuffer);
		if (!sndbuf->loadFromFile(mFilePath + key)){
			assert(false);
		}
		mSoundBufferMap.insert(std::pair<std::string, std::weak_ptr<sf::SoundBuffer> >(key, sndbuf) );
		return sndbuf;
	}
}
/*	Checks if the font corresponding to "key" is loaded or not.
	
	If it is loaded, a shared_ptr to the font is returned.
	If it isn't loaded, the resource is loaded and a shared pointer to the 
	font is returned. A weak_ptr is then stored in a map, mapping key to the
	font.*/

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string &key){
	std::map<std::string, std::weak_ptr<sf::Font> >::iterator iter;
	iter = mFontMap.find(key);

	if( iter != mFontMap.end() && !iter->second.expired() ){
		return iter->second.lock();
	}
	else{
		std::shared_ptr<sf::Font> fnt (new sf::Font);
		if (!fnt->loadFromFile(mFilePath + key)){
			assert(false);
		}
		mFontMap.insert(std::pair<std::string, std::weak_ptr<sf::Font> >(key, fnt) );
		return fnt;
	}
}

std::shared_ptr<sf::Shader> ResourceManager::getShader(const std::string &key){
	std::map<std::string, std::weak_ptr<sf::Shader> >::iterator iter;
	iter = mShaderMap.find(key);

	if( iter != mShaderMap.end() && !iter->second.expired() ){
		return iter->second.lock();
	}
	else{
		std::shared_ptr<sf::Shader> shdr (new sf::Shader);
		if (!shdr->loadFromFile(mFilePath + key, sf::Shader::Fragment)){
			assert(false);
		}
		mShaderMap.insert(std::pair<std::string, std::weak_ptr<sf::Shader> >(key, shdr) );
		return shdr;
	}
}

std::shared_ptr<sftheora::Video> ResourceManager::getVideo(const std::string &key){
	std::map<std::string, std::weak_ptr<sftheora::Video> >::iterator iter;
	iter = mVideoMap.find(key);

	if( iter != mVideoMap.end() && !iter->second.expired() ){
		return iter->second.lock();
	}
	else{
		std::shared_ptr<sftheora::Video> video (new sftheora::Video);
		if (!video->load(mFilePath + key)){
			assert(false);
		}
		mVideoMap.insert(std::pair<std::string, std::weak_ptr<sftheora::Video> >(key, video) );
		return video;
	}
}