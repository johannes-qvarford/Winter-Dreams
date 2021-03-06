#include "TriggerZone.h"
#include "SubLevel.h"
#include "Player.h"
#include "WindowManager.h"
#include "GameToScreen.h"

#include <boost/foreach.hpp>

#ifdef _DEBUG
#include <SFML/Graphics/VertexArray.hpp>
#endif

//number of times that a zones update method need
//be called without it touching a player
//to mean that the player has left the zone.
static const int EXIT_FRAMES = 2;

TriggerZone::TriggerZone(const sf::FloatRect& hitBox, TriggerZone::Action action, const std::list<std::string>& onEnterNames, const std::list<std::string>& onExitNames, std::list<std::string> requiredItems, std::list<std::string> absenceRequiredItems, int lightLevel, SubLevel* subLevel_p, bool triggerOnce, bool startEnabled):
	Entity(startEnabled),
	BaseHitBoxHaveable(hitBox),
	mOnce(triggerOnce),
	mAction(action),
	mUpdatesSinceLastTouch(EXIT_FRAMES + 1),
	mInZone(false),
	mLightLevel(lightLevel),
	mEnterNames(onEnterNames),
	mExitNames(onExitNames),
	mRequiredItems(requiredItems),
	mAbsenceRequiredItems(absenceRequiredItems),
	mSubLevel_p(subLevel_p)
{
}

void TriggerZone::update(SubLevel* subLevel_p) {
	mUpdatesSinceLastTouch++;
	mUpdatesSinceLastTouch %= (EXIT_FRAMES + 2);

	//if there have been a few frames since last touch,
	//and the player is in the zone,
	//then the player has exited the zone.
	if(mInZone && mUpdatesSinceLastTouch == EXIT_FRAMES) {

		mInZone = false;

		//do nothing if not enabled
		if(getEnabled() == false)
			return;
		
		for(auto it = mExitNames.begin(), end = mExitNames.end(); it != end; ++it) {
			auto& name = *it;
			auto entity_wp = mSubLevel_p->getEntity(name);
			
			//do nothing if entity is dead
			if(entity_wp.expired())
				return;
			std::shared_ptr<Entity> entity_sp(entity_wp);
			switch(mAction) {
			case ACTION_SWAP: entity_sp->swapEnabled(); break;
			case ACTION_ENABLE: entity_sp->setEnabled(true); break;
			case ACTION_DISABLE: entity_sp->setEnabled(false); break;
			}
		}
	}
}

void TriggerZone::drawSelf() {
#ifdef _DEBUG

	sf::Vertex vertices[] =
	{
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top), sf::Color::Red, sf::Vector2f( 0,  0)),
		sf::Vertex(sf::Vector2f(mHitBox.left, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f( 0, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top + mHitBox.height), sf::Color::Red, sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(mHitBox.left + mHitBox.width, mHitBox.top), sf::Color::Red, sf::Vector2f(10,  0)),
	};

	auto& window = *WindowManager::get().getWindow();
	auto states = *WindowManager::get().getStates();

	//translate to screen coordinates
	states.transform *= GAME_TO_SCREEN;

	window.draw(vertices, 4, sf::Quads, states);
#endif
}

void TriggerZone::onCollision(Collidable* col_p, const sf::Rect<float>& intersection) {
	
	//only collide with players with a high enough light level.
	
	auto player_p = dynamic_cast<Player*>(col_p);
	if(player_p == nullptr || player_p->getCurrentLightLevel() < mLightLevel)
		return;

	auto& inventory = player_p->getInventory();

	//only collide if the player has certain items.
	for(auto it = mRequiredItems.begin(), end = mRequiredItems.end(); it != end; ++it) {
		
		auto& itemName = *it;
		if(inventory.hasItem(itemName) == 0)
			return;
	}
	//only collider if the player doesn't have certain items
	BOOST_FOREACH(const auto& itemName, mAbsenceRequiredItems) {
		if(inventory.hasItem(itemName) != 0)
			return;
	}

	//if the player isn't in the zone yet,
	//then the player has entered the zone
	if(mInZone == false) {
		mInZone = true;

		//do nothing if not enabled
		if(getEnabled() == false)
			return;

		for(auto it = mEnterNames.begin(), end = mEnterNames.end(); it != end; ++it) {
			auto& name = *it;

			//do nothing if entity is dead
			auto entity_wp = mSubLevel_p->getEntity(name);
			if(entity_wp.expired())
				return;

			std::shared_ptr<Entity> entity_sp(entity_wp);
			switch(mAction) {
			case ACTION_SWAP: entity_sp->swapEnabled(); break;
			case ACTION_ENABLE: entity_sp->setEnabled(true); break;
			case ACTION_DISABLE: entity_sp->setEnabled(false); break;
			}
			//disable self if it's a one time trigger
			if(mOnce)
				setEnabled(false);
		}
	}

	//reset
	mUpdatesSinceLastTouch = 0;
}