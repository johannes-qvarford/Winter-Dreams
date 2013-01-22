#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX

#include <SFML\Graphics\Rect.hpp>
#include "PlayerRelated.h"

class DamageHitBox:public PlayerRelated{
public:
	DamageHitBox(unsigned int damage, const sf::Rect<float>& hitBox, DamageTypes type);
	~DamageHitBox();
	const sf::Rect<float>& getHitBox() const;
	void update(int milliseconds);
	void drawSelf();
private:
	enum DamageTypes{NORMAL, ANNAT};
	unsigned int mDamage;
	sf::Rect<float> mHitBox;
	int mMilliseconds;
};
#endif