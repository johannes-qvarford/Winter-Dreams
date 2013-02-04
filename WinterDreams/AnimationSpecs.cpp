#include "AnimationSpecs.h"

AnimationSpecs::AnimationSpecs(	const std::string animName,
								const std::string fileName, 
								int spriteWidth, 
								int spriteHeight, 
								int numberOfSprites, 
								int framesPerSprite,
								int xOrigin,
								int yOrigin	) :
		mWidth  ( spriteWidth ),
		mHeight ( spriteHeight ),
		mNrOfSprites( numberOfSprites ),
		mFramesPerSprite ( framesPerSprite ),
		mAnimName ( animName ),
		mFileName ( fileName ),
		mXOrigin (xOrigin ),
		mYOrigin (yOrigin )
		{ }
////////////////////////////////////////////////////////////////////////////////
void AnimationSpecs::parse(const boost::property_tree::ptree& animationTree, std::list<AnimationSpecs>& animList) {
	auto& ptree = animationTree;
	auto& animations = ptree.get_child( "animations" );

	for(auto iter = animations.begin(), end = animations.end(); iter != end; ++iter){
		if(iter->second.get<std::string>("filename") != "null"){
			auto w =	iter->second.get<int>	("spritewidth");
			auto h =	iter->second.get<int>	("spriteheight");
			auto yO =	iter->second.get<int>	("yorigin");
			auto xO =	iter->second.get<int>	("xorigin");
			auto nos =	iter->second.get<int>	("numberofsprites");
			auto fps =	iter->second.get<int>	("framespersprite");
			auto file = iter->second.get<std::string>	("filename");

			auto name = iter->first;
		
			AnimationSpecs as(name, file, w, h, nos, fps, xO, yO);
			animList.push_back( as );
		}
	}
}
////////////////////////////////////////////////////////////////////////////////