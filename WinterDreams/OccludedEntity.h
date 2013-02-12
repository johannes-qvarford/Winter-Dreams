#ifndef INCLUDED_OCCLUDEDENTITY
#define INCLUDED_OCCLUDEDENTITY
#include "GraphicalEntity.h"
#include "WindowManager.h"
#include "ResourceManager.h"

class OccludedEntity : public GraphicalEntity {
	OccludedEntity( bool startEnabled=true );
	~OccludedEntity();
	void setAlpha(float alpha);
	void drawSelf();
private:
	float mAlpha;
	std::shared_ptr<sf::Shader> mShader;
	sf::Texture mTexture;
};

#endif