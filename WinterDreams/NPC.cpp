#include "NPC.h"
#include "GameState.h"

#include <limits>

static const float WALKING_SPEED = 1.0f;

NPC::NPC(const std::string& pathName, const sf::FloatRect& initialPosition, bool startsEnabled):
	GraphicalEntity(startsEnabled),
	mFoundPath(false),
	mPathName(pathName),
	mPath_p(NULL),
	mNextPoint(0),
	mHitBox(initialPosition)
{
}

void NPC::update(GameState* state) {
	//are we still looking for the path?
	if(mFoundPath == false) {
		auto& points = state->getAiPath(mPathName);
		
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

			for(int i = 0; i < mPath_p->size(); ++i) {
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

		//are we really close to the point?
		if(abs(distance - WALKING_SPEED) < 0.01) {
			//go to next point, next frame
			mNextPoint = (mNextPoint + 1) % mPath_p->size();
		}
			
		//normalize
		auto normalPosToPoint = posToPoint / distance;

		//get closer to the point
		position += normalPosToPoint * WALKING_SPEED; 

		//convert back to hitbox
		mHitBox.left = position.x;
		mHitBox.top = position.y;
	}
}

void NPC::drawSelf() {
	//TODO: Add code that draws the NPC.
}