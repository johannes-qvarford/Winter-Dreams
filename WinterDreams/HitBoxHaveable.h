#ifndef INCLUDED_HITBOXHAVEABLE
#define INCLUDED_HITBOXHAVEABLE

////////////////////////////////////////////////////////////
// HitBoxHaveablles has hitboxes, this is becasue collidables and graphicals
// need hitboxes, to check for collisions, and check for positions.
////////////////////////////////////////////////////////////
class HitBoxHaveable {
public:

	virtual ~HitBoxHaveable(){}

	virtual sf::FloatRect& getHitBox() = 0;

	virtual const sf::FloatRect& getHitBox() const = 0;

	virtual sf::Vector2f getPosition() const = 0;
};

#endif