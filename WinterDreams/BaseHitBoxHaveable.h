#ifndef INCLUDED_BASEHITBOXHAVEABLE
#define INCLUDED_BASEHITBOXHAVEABLE

#include "HitBoxHaveable.h"

#include <SFML/Graphics/Rect.hpp>

////////////////////////////////////////////////////////////
// /Basic implementation of HitBoxHaveable.
// /Can be intialized without a HitBox, but then it cannot be read at runtime
// /(this is to prevent programming errors, 
// /like creating a world graphical entity without a hitbox.)
////////////////////////////////////////////////////////////
class BaseHitBoxHaveable : virtual public HitBoxHaveable {
public:

	BaseHitBoxHaveable(const sf::FloatRect& initialHitBox);

	BaseHitBoxHaveable();//creates invalid hitbox

	sf::FloatRect& getHitBox() override;//will assert for invalid hitbox

	const sf::FloatRect& getHitBox() const override;//will assert for invalid hitbox

	sf::Vector2f getPosition() const override;

protected:

	sf::FloatRect mHitBox;
};

#endif