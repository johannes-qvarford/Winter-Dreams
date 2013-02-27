#include "Button.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "FileStructure.h"

#include <SFML/Graphics/Sprite.hpp>

Button::Button(const sf::Vector2f& initialPosition, const std::string& buttonFilename):
mTexture_sp(ResourceManager::get().getTexture(FS_DIR_UI  + buttonFilename)),
	mBounds(initialPosition.x, initialPosition.y, 0, 0)
{
	mBounds.width = float(mTexture_sp->getSize().x) / 1920;
	mBounds.height = float(mTexture_sp->getSize().y) / 1080;
}

const sf::FloatRect& Button::getBounds() const {
	return mBounds;
}

sf::FloatRect& Button::getBounds() {
	return mBounds;
}

void Button::setBounds(const sf::FloatRect& bounds) {
	mBounds = bounds;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto size = target.getSize();
	auto sprite = sf::Sprite(*mTexture_sp);

	//draw in window coordinates
	sprite.setPosition(sf::Vector2f(mBounds.left * size.x, mBounds.top * size.y));
	sprite.setScale(
		float(target.getSize().x) / 1920, 
		float(target.getSize().y) / 1080);
	target.draw(sprite, states);
}
