#include "OccludedEntity.h"
#include "FileStructure.h"

OccludedEntity::OccludedEntity(bool startEnabled) :
	mAlpha(1.0f),
	GraphicalEntity ( startEnabled ){
		mShader = ResourceManager::get().getShader(FS_DIR_SHADERS + "Blend.frag");
}

OccludedEntity::~OccludedEntity(){
}

void OccludedEntity::setAlpha(float alpha){
	mAlpha=alpha;
}

void OccludedEntity::drawSelf(){
	auto renTex = WindowManager.get().getWindow();
	mShader->setParameter("Alpha",mAlpha);
	sf::Sprite spr(mTexture);
	if (getEnabled()){
		renTex->draw(spr);
	}else{
		renTex->draw(spr, mShader.get());
	}
}