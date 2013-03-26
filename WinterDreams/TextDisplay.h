#ifndef INCLUDED_TEXTDISPLAY
#define INCLUDED_TEXTDISPLAY


#include "Entity.h"
#include "Drawable.h"
#include "BaseHitBoxHaveable.h"

#include <vector>

////////////////////////////////////////////////////////////
// /TextDisplay is used to draw text on the screen.
////////////////////////////////////////////////////////////
class TextDisplay : public Entity, public Drawable, public BaseHitBoxHaveable {
public:

	struct TimedText {
		int mTimestamp;
		std::string mText;
	};

	////////////////////////////////////////////////////////////
	// /Create a text display that displays text.
	////////////////////////////////////////////////////////////
	TextDisplay(const std::vector<TimedText>& timedText, const sf::Vector2f& position, bool startEnabled);

	////////////////////////////////////////////////////////////
	// /Update the displayed text.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	////////////////////////////////////////////////////////////
	// /Draw the text.
	////////////////////////////////////////////////////////////
	void draw();

private:

	int mNumFrames;

	int mLastIndex;

	std::shared_ptr<sf::Shader> mShaderX_sp;

	std::shared_ptr<sf::Shader> mShaderY_sp;

	std::shared_ptr<sf::Font> mFont_sp;

	std::vector<TimedText> mTimedText;

	sf::RenderTexture mTexture1;

	sf::RenderTexture mTexture2;

	sf::RenderTexture mTexture3;

	sf::Text mText;

	sf::Vector2f mPosition;
};

#endif