#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX
#include "PlayerRelated.h"

class DamageHitBox:public PlayerRelated{
public:
	DamageHitBox(unsigned short damage, unsigned short x, unsigned short y, unsigned short w, unsigned short h, DamageTypes type);
private:
	enum DamageTypes{NORMAL, ANNAT};
	unsigned short damage, x, y, w, h;
};
#endif