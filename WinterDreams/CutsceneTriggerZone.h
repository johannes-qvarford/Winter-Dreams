#ifndef INCLUDED_CUTSCENETRIGGERZONE
#define INCLUDED_CUTSCENETRIGGERZONE

#include "TriggerZone.h"
#include<SFML\Graphics\Rect.hpp>

class CutsceneTriggerZone : public TriggerZone {
public:
	/*
	//	Creates a TriggerZone for playing cutscenes at a
	//	position described by a sf:FloatRect
	*/
	CutsceneTriggerZone(sf::FloatRect zone);
	/*
	//	No dynamic members do delete.
	*/
	~CutsceneTriggerZone();
	/*
	//	Defines what action CutscenesTriggerZone should
	//	perform each game-update.
	*/
	virtual void update(GameState* gameState_p, int milliseconds);
	/*
	//	CutsceneTriggerZone will be invisible, hence will 
	//	drawSelf produce no graphical representation.
	*/
	virtual void drawSelf(sf::RenderWindow* window_p);
	/*
	//	Returns the hitbox of the CutsceneTriggerZone.
	*/
	virtual const sf::FloatRect& getHitBox() const;
private:

	sf::FloatRect mHitBox;
};

#endif