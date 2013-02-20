#include "ListFrame.h"
#include "WindowManager.h"
#include "InputManager.h"

#include <SFML/Graphics/Vertex.hpp>

static int COOLDOWN = 10;

ListFrame::ListFrame(const std::vector<std::shared_ptr<Widget> >& widgets):
	mBounds(0, 0, 0, 0),
	mWidgets(widgets),
	mCurrentWidget(0),
	mCooldownFrames(0)
{
	mBounds = *mWidgets[mCurrentWidget]->getBounds();
	mBounds.left -= 2;
	mBounds.top -= 2;
	mBounds.width += 4;
	mBounds.height += 4;
	mWidgets[mCurrentWidget]->onHover(true);
}

void ListFrame::activate() {
}

void ListFrame::onHover(bool doHover) {
}

void ListFrame::update(MenuState* state_p) {
	
	mCooldownFrames++;
	
	
	auto& mgr = InputManager::get();

	auto xstick = mgr.getStick().x;
	auto ystick = mgr.getStick().y;

	if(mCooldownFrames > COOLDOWN) {
		//up
		if(xstick > 0 && ystick > 0) {
			mWidgets[mCurrentWidget]->onHover(false);
			mCurrentWidget = ++mCurrentWidget % mWidgets.size();

			mWidgets[mCurrentWidget]->onHover(true);

			mCooldownFrames = 0;
		}
		//down
		else if(xstick < 0 && ystick < 0) {
			mWidgets[mCurrentWidget]->onHover(false);
			//the result of x mod y is implementation defined if x is negative.
			mCurrentWidget = --mCurrentWidget;
			if(mCurrentWidget < 0) 
				mCurrentWidget += mWidgets.size();

			mWidgets[mCurrentWidget]->onHover(true);

			mCooldownFrames = 0;
		}
	}

	mWidgets[mCurrentWidget]->activate();

	resizeFrame();
}

void ListFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	
#ifdef _DEBUG
	//draw a debug frame
	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top), sf::Color(0, 255, 255)),
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top + mBounds.height), sf::Color(0, 255, 255)),
		sf::Vertex(sf::Vector2f(mBounds.left + mBounds.width, mBounds.top + mBounds.height), sf::Color(0, 255, 255)),
		sf::Vertex(sf::Vector2f(mBounds.left + mBounds.width, mBounds.top), sf::Color(0, 255, 255)),
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top), sf::Color(0, 255, 255))
	};
	target.draw(vertices, 5, sf::LinesStrip, states);
#endif
}

const sf::FloatRect& ListFrame::getBounds() const {
	return mBounds;
}

sf::FloatRect* ListFrame::getBounds() {
	return &mBounds;
}

void ListFrame::resizeFrame() {
	mBounds = *mWidgets[mCurrentWidget]->getBounds();
	mBounds.left -= 2;
	mBounds.top -= 2;
	mBounds.width += 4;
	mBounds.height += 4;
}