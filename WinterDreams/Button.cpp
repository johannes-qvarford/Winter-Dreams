#include "Button.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "StateManager.h"

#include <SFML/Graphics/Sprite.hpp>

Button::Button(const sf::Vector2f& initialPosition, std::shared_ptr<sf::Texture> texture_sp, const sf::Text& text, std::shared_ptr<sf::Font> font_sp):
	mTexture_sp(texture_sp),
	mBounds(initialPosition.x, initialPosition.y, float(texture_sp->getSize().x), float(texture_sp->getSize().y)),
	mText(text),
	mFont_sp(font_sp)
{
}

void Button::activate() {
}

void Button::update(MenuState* state_p) {
}

void Button::onHover(bool doHover) {
	//red or yellow
	auto color = (doHover ? sf::Color(1, 0, 0) : sf::Color(1, 1, 0));
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
	target.draw(sprite, states);
}