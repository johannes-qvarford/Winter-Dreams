#ifndef INCLUDED_OCCLUDEDENTITY
#define INCLUDED_OCCLUDEDENTITY

#include "Entity.h"
#include "Drawable.h"
#include "BaseHitBoxHaveable.h"

#include "Animation.h"

class OccludedEntity : public Entity, public Drawable, public BaseHitBoxHaveable {
public:
	//NOTE: Expects high layer (100, not 1 for example, see Drawable)
	OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float enabledOpacity, float disabledOpacity, int fadeTime, int layer, bool startEnabled);
	
	void update(SubLevel* subLevel_p){};

	void draw();

	int getLayer();
private:
	
	int mLayer;

	float mEnabledAlpha, mDisabledAlpha, mCurrentAlpha;

	float mFadeTime;

	std::shared_ptr<sf::Shader> mShader;

	Animation mAnimation;


	sf::Vector2f mOffset;
};

#endif