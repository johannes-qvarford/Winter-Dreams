#include "PrecompiledHeader.h"
#include "LevelSplash.h"


LevelSplash::LevelSplash(const int lifeTime, bool startEnabled ) : 
	Script( startEnabled ),
	mLifeTime( lifeTime )
{}


LevelSplash::~LevelSplash()
{}

void LevelSplash::draw() const {}

void LevelSplash::update(SubLevel* subLevel_p) {}