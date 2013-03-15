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

	std::string mUkontrollFilename;

	sf::Vector2f mCursorPos;
private:

	QRSpecs() {
		//exit(int) is a c function. use some other name
		auto& qrs = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.ukontroll");
		mXOffset = qrs.get<float>("xoffset");
		mYOffsetBot = qrs.get<float>("yoffsetbot");
		mYOffsetTop = qrs.get<float>("yoffsettop");
		mUkontrollFilename = qrs.get<std::string>("filename");
		mCursorPos.x = qrs.get<float>("cursorposx");
		mCursorPos.y = qrs.get<float>("cursorposy");
	}
};

QRDisplay::QRDisplay():
	Button(QRSpecs::get().mCursorPos, "transp.png"),
	mUpdated(false),
	mActivated(false),
	mStatus( TOP ),
	mQrPos( 0,0 )
{
	mUkontrollTexture_sp = ResourceManager::get().getTexture(FS_DIR_UI + QRSpecs::get().mUkontrollFilename);
	auto& win = *WindowManager::get().getRenderWindow();
	auto& qrs = QRSpecs::get();

	mUkontrollSprite.setTexture( *mUkontrollTexture_sp);
	mUkontrollPos = sf::Vector2f( qrs.mXOffset, qrs.mYOffsetTop );
	mUkontrollSprite.setPosition( mUkontrollPos.x * win.getSize().x, mUkontrollPos.y * win.getSize().y );
}

void QRDisplay::activate() {
	bool updated = false;
	if( InputManager::get().isADown() || InputManager::get().isStartDown() )
		updated = true;

	if(mActivated == false && updated == true && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mActivated = true;

		auto response = getResponse();
		if( response.getStatus() == sf::Http::Response::Ok) {
			mQrCodeTexture_sp = std::make_shared<sf::Texture>( getQR( response ) );
		
			auto socket =  openSocket( response );
			InputManager::get().setSocket( socket );
		}
		else {
			mQrCodeTexture_sp = ResourceManager::get().getTexture(FS_DIR_UI + "ukontroll_connection_fail.png" );
		}

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
	if( mStatus == ASCEND ) {
		if( mUkontrollPos.y > QRSpecs::get().mYOffsetTop ) {
			mUkontrollPos.y -= 2.f / 1080.f;
			mBounds.top -= 2.f / 1080.f;
		}
		else
			mStatus = TOP;
	} else if( mStatus == DESCEND ) {
		if( mUkontrollPos.y < QRSpecs::get().mYOffsetBot ){
			mUkontrollPos.y += 2.f / 1080.f;
			mBounds.top += 2.f / 1080.f;
		}
		else
			mStatus = BOTTOM;
	}
	auto& win = *WindowManager::get().getRenderWindow();
	mUkontrollSprite.setPosition( mUkontrollPos.x * win.getSize().x, mUkontrollPos.y * win.getSize().y );
	

	mQrPos = sf::Vector2f( mUkontrollPos.x + (8.f / 1080.f), mUkontrollPos.y + (180.f / 1080.f) );
}

void QRDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);		
	
	target.draw(mUkontrollSprite, states);

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
