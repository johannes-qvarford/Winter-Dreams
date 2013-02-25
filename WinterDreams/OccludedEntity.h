#ifndef INCLUDED_OCCLUDEDENTITY
#define INCLUDED_OCCLUDEDENTITY
#include "GraphicalEntity.h"
#include "Animation.h"

class OccludedEntity : public GraphicalEntity {
public:
	OccludedEntity(const sf::FloatRect& initialPosition, const Animation& animation, float alpha, int layer, bool startEnabled);
	
	~OccludedEntity();
	
	void update(SubLevel* subLevel_p){};

	void onCollision(PhysicalEntity * pe_p,const sf::FloatRect& intersection){}

	void setAlpha(float alpha);
	
	void drawSelf();

	sf::FloatRect& getHitBox();

	//not checking this in SubLevel yet.
	int getLayer();
private:
	
	int mLayer;

	float mAlpha;

	std::shared_ptr<sf::Shader> mShader;

	Animation mAnimation;

	sf::FloatRect mHitBox;
};

#endif