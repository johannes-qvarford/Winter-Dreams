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

class QRSpecs {
public:

	static QRSpecs& get(){ static QRSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffsetBot;
	float mYOffsetTop;

	std::string mFilename;

private:

	QRSpecs() {
		//exit(int) is a c function. use some other name
		auto& exit_ = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.ukontroll");
		mXOffset = exit_.get<float>("xoffset");
		mYOffsetBot = exit_.get<float>("yoffsetbot");
		mYOffsetTop = exit_.get<float>("yoffsettop");
		mFilename = exit_.get<std::string>("filename");
	}
};

QRDisplay::QRDisplay():
	Button(sf::Vector2f(QRSpecs::get().mXOffset, QRSpecs::get().mYOffsetBot), QRSpecs::get().mFilename),
	mUpdated(false),
	mActivated(false),
	mStatus( BOTTOM ),
	mQrPos( 0,0 )
{
}

void QRDisplay::activate() {
	bool updated = false;
	if( InputManager::get().isADown() )
		updated = true;

	if(mActivated == false && updated == true && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mActivated = true;

		auto response = getResponse();

		mQrCodeTexture_sp = std::make_shared<sf::Texture>( getQR( response ) );
		
		auto socket =  openSocket( response );
		InputManager::get().setSocket( socket );
	}

	if( mStatus == BOTTOM && updated == true ){
		mStatus = ASCEND;
		updated = false;
	}
	else if( mStatus == TOP && updated == true ){
		mStatus = DESCEND;
		updated = false;
	}
}

void QRDisplay::update(MenuState* menuState_p){
	if( mStatus == ASCEND ){
		if( mBounds.top > QRSpecs::get().mYOffsetTop )
			mBounds.top -= 2.f / 1080.f;
		else
			mStatus = TOP;
	} else 	if( mStatus == DESCEND ){
		if( mBounds.top < QRSpecs::get().mYOffsetBot )
			mBounds.top += 2.f / 1080.f;
		else
			mStatus = BOTTOM;
	}

	mQrPos = sf::Vector2f( mBounds.left + (8.f / 1080.f), mBounds.top + mBounds.height - (8.f / 1080.f) );
}

void QRDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);		
	
	if( mActivated ){
		auto qrSprite = sf::Sprite( *mQrCodeTexture_sp );
		qrSprite.setPosition( mQrPos.x * target.getSize().x, mQrPos.y * target.getSize().y );
		qrSprite.setScale(
			float(target.getSize().x) / 1920*0.45f, 
			float(target.getSize().y) / 1080*0.45f);
		qrSprite.setOrigin( 0.f, static_cast<float>(mQrCodeTexture_sp->getSize().y) );
		target.draw( qrSprite, states );	
	}
}
