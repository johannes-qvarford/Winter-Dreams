#include "ListFrame.h"
#include "WindowManager.h"
#include "InputManager.h"

#include <SFML/Graphics/Vertex.hpp>

ListFrame::ListFrame(const std::vector<std::shared_ptr<Widget> >& widgets):
	mBounds(0, 0, 0, 0),
	mWidgets(widgets),
	mCurrentWidget(0)
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
	auto& mgr = InputManager::get();

	//up
	if(mgr.getStick() == sf::Vector2f(-1, -1)) {
		mWidgets[mCurrentWidget]->onHover(false);
		mCurrentWidget = ++mCurrentWidget % mWidgets.size();

		mBounds = *mWidgets[mCurrentWidget]->getBounds();
		mBounds.left -= 2;
		mBounds.top -= 2;
		mBounds.width += 4;
		mBounds.height += 4;
		mWidgets[mCurrentWidget]->onHover(true);
	}
	//down
	else if(mgr.getStick() == sf::Vector2f(1, 1)) {
		mWidgets[mCurrentWidget]->onHover(false);
		//the result of x mod y is implementation defined if x is negative.
		mCurrentWidget = --mCurrentWidget;
		if(mCurrentWidget < 0) 
			mCurrentWidget += mWidgets.size();

		mBounds = *mWidgets[mCurrentWidget]->getBounds();
		mBounds.left -= 2;
		mBounds.top -= 2;
		mBounds.width += 4;
		mBounds.height += 4;

		mWidgets[mCurrentWidget]->onHover(true);
	}

	if(mgr.isADown())
		mWidgets[mCurrentWidget]->activate();
}

void ListFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	
	//draw a debug frame
	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top), sf::Color(0, 0, 1)),
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top + mBounds.height), sf::Color(0, 0, 1)),
		sf::Vertex(sf::Vector2f(mBounds.left + mBounds.width, mBounds.top + mBounds.height), sf::Color(0, 0, 1)),
		sf::Vertex(sf::Vector2f(mBounds.left + mBounds.width, mBounds.top), sf::Color(0, 0, 1)),
		sf::Vertex(sf::Vector2f(mBounds.left, mBounds.top), sf::Color(0, 0, 1))
	};
//	target.draw(vertices, 5, sf::LinesStrip, states);
}

const sf::FloatRect& ListFrame::getBounds() const {
	return mBounds;
}

sf::FloatRect* ListFrame::getBounds() {
	return &mBounds;
}

