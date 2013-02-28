#include "QRDisplay.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "Network.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

static const char * const BOX_FILENAME = "placeholder_box.png"; 
static const char * const FONT_FILENAME = "arial.ttf";
static const char * const TEXT_STRING = "uKontroll";

QRDisplay::QRDisplay(const sf::Vector2f& initialPosition):
	Button(initialPosition, FS_DIR_UI + BOX_FILENAME),
	mUpdated(false)
{

}

void QRDisplay::activate() {
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;

		auto response = getResponse();

		mQrCodeTexture_sp = std::make_shared<sf::Texture>( getQR( response ) );
		
		mQrSprite.setTexture( *mQrCodeTexture_sp );
		mQrSprite.setPosition( 0,0 );
		mQrSprite.setScale( 0.5, 0.5 );

		mSocket_p = openSocket( response );
	}
}

void QRDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);
	//auto sprite = sf::Sprite(*mTexture_sp);
	//sprite.setPosition(mBounds.left, mBounds.top);
	//target.draw(sprite, states);
	//target.draw(mText);
	//target.draw( mQrSprite );	
}
