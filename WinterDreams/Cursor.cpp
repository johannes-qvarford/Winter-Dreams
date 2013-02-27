#include "Cursor.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "PropertyManager.h"
#include "FileStructure.h"
#include "ResourceManager.h"

#include <SFML/Graphics/Vertex.hpp>

static int COOLDOWN = 10;

class CursorSpecs {
public:

	static CursorSpecs& get(){ static CursorSpecs sSpecs; return sSpecs; }

	float mXOffsetFromButton;

	float mYOffsetFromButton;

	std::string mFilename;

private:

	CursorSpecs() {
		auto& cursor = PropertyManager::get().getGeneralSettings().get_child("ui.general.cursor");
		mXOffsetFromButton = cursor.get<float>("xoffsetfrombutton");
		mYOffsetFromButton = cursor.get<float>("yoffsetfrombutton");
		mFilename = cursor.get<std::string>("filename");
	}
};

Cursor::Cursor(const std::vector<std::shared_ptr<Widget> >& widgets):
mTexture(ResourceManager::get().getTexture(FS_DIR_UI +  CursorSpecs::get().mFilename)),
	mBounds(0, 0, 0, 0),
	mWidgets(widgets),
	mCurrentWidget(0),
	mCooldownFrames(0)
{
	resizeFrame();
	mWidgets[mCurrentWidget]->onHover(true);
}

void Cursor::activate() {
}

void Cursor::onHover(bool doHover) {
}

void Cursor::update(MenuState* state_p) {
	
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
			resizeFrame();
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
			resizeFrame();
		}
	}

	mWidgets[mCurrentWidget]->activate();
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	
#ifdef NEVER
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

	//draw cursor
	sf::Sprite spr;
	spr.setTexture(*mTexture);
	spr.setPosition(mBounds.left * target.getSize().x, mBounds.top * target.getSize().y);
	spr.setScale(
		float(target.getSize().x) / 1920, 
		float(target.getSize().y) / 1080);
	target.draw(spr);
}

const sf::FloatRect& Cursor::getBounds() const {
	return mBounds;
}

sf::FloatRect& Cursor::getBounds() {
	return mBounds;
}

void Cursor::setBounds(const sf::FloatRect& bounds) {
	mBounds = bounds;
}

void Cursor::resizeFrame() {
	
	auto& specs = CursorSpecs::get();
	auto& box = mWidgets[mCurrentWidget]->getBounds();

	//cursor is at the side of the button
	mBounds.left = box.left + specs.mXOffsetFromButton;
	mBounds.top = box.top + specs.mYOffsetFromButton;
	mBounds.width = float(mTexture->getSize().x) / 1920;
	mBounds.height = float(mTexture->getSize().y) / 1080;
}