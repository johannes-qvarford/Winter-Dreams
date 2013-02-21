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
	QRDisplay(const sf::Vector2f& initialPosition);

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /fadeout, pop state, push loading state, fadein.
	////////////////////////////////////////////////////////////
	void activate();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

private:
	std::shared_ptr<sf::Texture> mQrCodeTexture_sp;
	sf::Sprite mQrSprite;
	sf::TcpSocket* mSocket_p;
	bool mUpdated;
};

#endif