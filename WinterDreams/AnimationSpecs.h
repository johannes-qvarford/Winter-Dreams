#ifndef INCLUDED_ANIMATIONSPECS
#define INCLUDED_ANIMATIONSPECS

////////////////////////////////////////////////////////////////////////////////
struct AnimationSpecs{
	AnimationSpecs(	const std::string animName,
					const std::string fileName, 
					int spriteWidth, 
					int spriteHeight, 
					int numberOfSprites, 
					int framesPerSprite,
					int xOrigin,
					int yOrigin	);

	int mWidth, mHeight, mNrOfSprites;
	int mFramesPerSprite, mXOrigin, mYOrigin;
	std::string mFileName, mAnimName;
	////////////////////////////////////////////////////////////////////////////////
	// /Parses all animationspecifications from an objects property tree into
	// /instances of AnimSpecs into a list.
	// /The argument ptree should the objects root tree, not the objects animation
	// /tree.
	// /The argument list should be the list that will hold the information.
	////////////////////////////////////////////////////////////////////////////////
	static void parse(const boost::property_tree::ptree& animationTree, std::list<AnimationSpecs>& animList);
};
////////////////////////////////////////////////////////////////////////////////

#endif