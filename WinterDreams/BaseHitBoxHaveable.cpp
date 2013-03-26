#include "BaseHitBoxHaveable.h"

#include <assert.h>

BaseHitBoxHaveable::BaseHitBoxHaveable(const sf::FloatRect& initialHitBox):
	mHitBox(initialHitBox)
{
}

BaseHitBoxHaveable::BaseHitBoxHaveable():
	mHitBox(-1, -1, -1, -1)//invalid hitbox
{
}

sf::FloatRect& BaseHitBoxHaveable::getHitBox() {
	assert(mHitBox.width > 0);
	return mHitBox;
}

const sf::FloatRect& BaseHitBoxHaveable::getHitBox() const {
	assert(mHitBox.width > 0);
	return mHitBox;
}

sf::Vector2f BaseHitBoxHaveable::getPosition() const {
	return sf::Vector2f(mHitBox.left, mHitBox.top);
}
