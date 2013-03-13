#include "TextDisplay.h"
#include "WindowManager.h"
#include "SubLevel.h"
#include "FileStructure.h"
#include "ResourceManager.h"

TextDisplay::TextDisplay(const std::vector<TimedText>& timedText, const sf::Vector2f& position, bool startEnabled):
	Script(startEnabled),
	mNumFrames(0),
	mFont_sp(ResourceManager::get().getFont(FS_DIR_FONTS + "arial.ttf")),
	mTimedText(timedText),
	mPosition(position),
	mShaderX_sp(ResourceManager::get().getShader(FS_DIR_SHADERS + "GaussBlurX.frag")),
	mShaderY_sp(ResourceManager::get().getShader(FS_DIR_SHADERS + "GaussBlurY.frag")),
	mLastIndex(-1)
{
}

void TextDisplay::update(SubLevel* subLevel_p) {
	++mNumFrames;
	
	auto curIndex = int(0);
	for(int i = 0; i < mTimedText.size(); ++i) {
		if(mNumFrames >= mTimedText[i].mTimestamp) {
			curIndex = i;
		}
	}

	mLastIndex = curIndex;
}

void TextDisplay::draw() const {
	auto& window = *WindowManager::get().getRenderWindow();
	auto states = *WindowManager::get().getStates();

	static int csize = 24;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
		csize += 1;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
		csize -= 1;

	auto text = sf::Text();

	text.setColor(sf::Color::White);
	text.setFont(*mFont_sp);
	text.setString(mTimedText[mLastIndex].mText);
	text.setCharacterSize(csize);

	auto winSize = window.getSize();

	auto bounds = text.getGlobalBounds();
	text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	text.setPosition(mPosition.x * winSize.x, mPosition.y * winSize.y);

	window.draw(text, states);
}