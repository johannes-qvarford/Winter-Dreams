#ifndef INCLUDED_QRDISPLAY
#define INCLUDED_QRDISPLAY

#include "Button.h"

namespace sf{
	class TcpSocket;
	class Texture;
};

class QRDisplay : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a start game button.
	////////////////////////////////////////////////////////////
	QRDisplay();

	////////////////////////////////////////////////////////////
	// /
	////////////////////////////////////////////////////////////
	void activate();

	void update(MenuState* menuState_p);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

private:
	enum DisplayStatus{ 
		TOP, BOTTOM, ASCEND, DESCEND
	};

	DisplayStatus mStatus;
	std::shared_ptr<sf::Texture> mUkontrollTexture_sp;
	sf::Sprite mUkontrollSprite;
	std::shared_ptr<sf::Texture> mQrCodeTexture_sp;
	sf::Vector2f mQrPos;
	sf::Vector2f mUkontrollPos;
	bool mUpdated;
	bool mActivated;
	
};

#endif