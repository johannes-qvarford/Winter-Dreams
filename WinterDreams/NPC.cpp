#include "NPC.h"
#include "AnimationSpecs.h"
#include "SubLevel.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "GameToScreen.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "Player.h"

#include <limits>

class NPCSpecs {
public:
	static NPCSpecs& get();

	float getSpeed() { return mSpeed; }

	float getTolerance() { return mTolerance; }

	const std::string& getSoundFilename(){ return mSoundFilename; }

	const std::list<AnimationSpecs>& getAnimSpecList(){ return mAnimSpecList; }

private:

	float mSpeed;

	float mTolerance;

	std::string mSoundFilename;

	std::list<AnimationSpecs> mAnimSpecList;

	NPCSpecs();
	NPCSpecs(const NPCSpecs&);//no copy
	NPCSpecs& operator=(const NPCSpecs&);//no copy
};

NPCSpecs& NPCSpecs::get() {
	static NPCSpecs sSpecs;
	return sSpecs;
}

NPCSpecs::NPCSpecs()
{
	auto prop = PropertyManager::get().getObjectSettings();
	auto npcTree = prop.get_child("objects.npc");
	mSpeed = npcTree.get<float>("speed");
	mTolerance = npcTree.get<float>("tolerance");
	mSoundFilename = npcTree.get<std::string>("soundfilename");
	AnimationSpecs::parse(npcTree, mAnimSpecList);
}

NPC::NPC(const std::string& pathName, const sf::FloatRect& initialPosition, int damage, bool startsEnabled):
	GraphicalEntity(startsEnabled),
	mDamage(damage),
	mFoundPath(false),
	mPathName(pathName),
	mPath_p(NULL),
	mNextPoint(0),
	mHitBox(initialPosition),
	mFirstFrame(false),
	mSound(),
	mSoundBuffer(ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + NPCSpecs::get().getSoundFilename()))
{
	mSound.setBuffer(*mSoundBuffer);

	auto& npcSpecs = NPCSpecs::get();
	auto& animSpecs = npcSpecs.getAnimSpecList();

	Animation::fromListToMap(animSpecs, FS_DIR_OBJECTANIMATIONS + "npc/", &mAnimationMap);
	//use placeholder for now

	auto it = mAnimationMap.find("left");
	mCurrentAnimation_p = &it->second;
}

void NPC::update(SubLevel* subLevel_p) {

	//are we still looking for the path?
	if(mFoundPath == false) {
		auto& points = subLevel_p->getAiPath(mPathName);
		
		//does it exist (yet)?
		if(points.empty() == false)
		{
			mFoundPath = true;
			mPath_p = &points;

			auto position = sf::Vector2f(mHitBox.left, mHitBox.top);

			//find the nearest point
			//the distance to the nearest point will always be 
			//smaller than smallestDistance.
			int nearestPoint = -1;
			auto smallestDistance = std::numeric_limits<float>::max();

			for(size_t i = 0; i < mPath_p->size(); ++i) {
				//calculate distance to point
				auto& point = (*mPath_p)[i];
				auto vectorDistance = position - point;
				auto x = vectorDistance.x;
				auto y = vectorDistance.y;
				auto distance = sqrtf((x * x) + (y * y));

				//is it the smallest distance yet?
				if(distance < smallestDistance) {
					nearestPoint = i;
					smallestDistance = distance;
				}
			}

			mNextPoint = nearestPoint;
		}
	}
	//move 
	else {

		//convert part of hitbox to a vector for easier calculations
		auto position = sf::Vector2f(mHitBox.left, mHitBox.top);
		
		//get a vector to the point
		auto posToPoint = (*mPath_p)[mNextPoint] - position;

		float distance;
		{
			auto x = posToPoint.x;
			auto y = posToPoint.y;
			distance = sqrtf((x * x) + (y * y));
		}

		float speed = NPCSpecs::get().getSpeed();
		float tolerance = NPCSpecs::get().getTolerance();

		//are we really close to the point?
		if(fabs(distance - speed) < tolerance) {
			//go to next point, next frame
			mNextPoint = (mNextPoint + 1) % mPath_p->size();
		}
			
		//normalize
		auto normalPosToPoint = posToPoint / distance;

		mCurrentAnimation_p->updateAnimation();

		if(normalPosToPoint.x >= 0 && normalPosToPoint.y <= 0)
			mCurrentAnimation_p = &mAnimationMap.find("right")->second;
		else if(normalPosToPoint.x <= 0 && normalPosToPoint.y >= 0)
			mCurrentAnimation_p = &mAnimationMap.find("left")->second;
		else if(normalPosToPoint.x >= 0 && normalPosToPoint.y >= 0)
			mCurrentAnimation_p = &mAnimationMap.find("down")->second;
		else 
			mCurrentAnimation_p = &mAnimationMap.find("up")->second;

		//get closer to the point
		position += normalPosToPoint * speed; 

		//convert back to hitbox
		mHitBox.left = position.x;
		mHitBox.top = position.y;
	}
}

void NPC::drawSelf() {
	auto sprite = mCurrentAnimation_p->getCurrentSprite();
	
	//get screen position
	auto position = GAME_TO_SCREEN * sf::Vector2f(mHitBox.left, mHitBox.top);
	sprite.setPosition(position);
	
	auto& mgr = WindowManager::get();
	auto& window = *mgr.getWindow();
	auto& states = *mgr.getStates();

	window.draw(sprite, states);

}

sf::FloatRect& NPC::getHitBox() {
	return mHitBox;
}

void NPC::onCollision(PhysicalEntity * pe_p, const sf::FloatRect& intersection) {
	mSound.setVolume( PropertyManager::get().getUserSettings()->get<float>("volumes.soundVolume") * 0.3f );

	auto player_p = dynamic_cast<Player*>(pe_p);
	if(player_p == nullptr)
		return;

	//do this even if player has no life left?
	if(mSound.getStatus() != sf::Sound::Playing)
		mSound.play();

	auto curLevel = player_p->getCurrentLightLevel();
	if(curLevel > 1 && player_p->isVulnerable()) {

		//remove damage so that the player has at least 1 light level.
		player_p->setCurrentLightLevel(curLevel > mDamage ? curLevel - mDamage : 1);
		player_p->setInvulnerable();
	}
}
