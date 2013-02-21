#include "Button.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "StateManager.h"

#include <SFML/Graphics/Sprite.hpp>

Button::Button(const sf::Vector2f& initialPosition, const std::string& text, const std::string& buttonFilename, const std::string& fontFilename, int characterSize):
	mTexture_sp(ResourceManager::get().getTexture(buttonFilename)),
	mBounds(initialPosition.x, initialPosition.y, 0, 0),
	//mText(text, *mFont_sp, 20),
	mText(),
	mFont_sp(ResourceManager::get().getFont(fontFilename))
{
	mText.setString(text);
	mText.setFont(*mFont_sp);
	mText.setCharacterSize(20);
	mText.setColor( sf::Color::Blue );

	mBounds.width = float(mTexture_sp->getSize().x);
	mBounds.height = float(mTexture_sp->getSize().y);

	//place text in the middle of the button
	mText.setPosition(initialPosition + sf::Vector2f(mBounds.width / 2, mBounds.height / 2));
	mText.setOrigin(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2);
}

void Button::activate() {
}

void Button::update(MenuState* state_p) {
	auto& mgr = WindowManager::get();
	auto& window = *mgr.getRenderWindow();
	auto& winSize = window.getSize();
	auto& texSize = mTexture_sp->getSize();

	auto tempBounds = mBounds;

	//always in center
	mBounds.left = (winSize.x / 2) - texSize.x / 2;
	mText.setPosition(mBounds.left + (mBounds.width / 2), mText.getPosition().y);
}

void Button::onHover(bool doHover) {
	//red or yellow
	auto color = (doHover ? sf::Color(255, 0, 0) : sf::Color::Blue);
	mText.setColor(color);
}

const sf::FloatRect& Button::getBounds() const {
	return mBounds;
}

sf::FloatRect* Button::getBounds() {
	return &mBounds;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto sprite = sf::Sprite(*mTexture_sp);
	sprite.setPosition(mBounds.left, mBounds.top);
	target.draw(sprite, states);
	target.draw(mText);
}