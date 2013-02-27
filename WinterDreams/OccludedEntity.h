#ifndef INCLUDED_OCCLUDEDENTITY
#define INCLUDED_OCCLUDEDENTITY
#include "GraphicalEntity.h"
#include "Animation.h"

class OccludedEntity : public GraphicalEntity {
public:
	OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float enabledOpacity, float disabledOpacity, int fadeTime, int layer, bool startEnabled);
	
	~OccludedEntity();
	
	void update(SubLevel* subLevel_p){};

	void onCollision(PhysicalEntity * pe_p,const sf::FloatRect& intersection){}
	
	void drawSelf();

	sf::FloatRect& getHitBox();

	int getLayer();
private:
	
	int mLayer;

	float mEnabledAlpha, mDisabledAlpha, mCurrentAlpha;

	float mFadeTime;

	std::shared_ptr<sf::Shader> mShader;

	Animation mAnimation;

	sf::FloatRect mHitBox;

	sf::Vector2f mOffset;
};

#endif