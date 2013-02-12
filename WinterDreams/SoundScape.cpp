#include "SoundScape.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "PropertyManager.h"
#include <cmath>



SoundScape::SoundScape(sf::Rect<float> collisionBox, float innerRadius, int rangeDecay, float volume, bool loop, std::string soundName, bool startsEnabled, std::string soundType):
CollisionZone(startsEnabled, collisionBox, false),
mBoolEntity(false),
mInnerRadius(innerRadius),
mRangeDecay(rangeDecay),
mVolume(volume),
mLoop(loop),
mSoundType(soundType),
mSoundName(soundName),
mEnabledLastFrame(startsEnabled)


{
////////////////////////////////////////////////////////////////////////
// /FS_DIR_SOUNDS betyder: vart ligger filen, mSoundName betyder: vad heter filen
////////////////////////////////////////////////////////////////////////
	mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + mSoundName);
	mSound.setBuffer(*mBuffer);
	mSound.setLoop(loop);
	mSound.setVolume(mVolume);
}

SoundScape::~SoundScape(){
	mSound.stop();
}

//////////////////////////////////////////////////////////
// /player ska bara h�mtas en g�ng och f�r inte vara en weak pointer s� d�rf�r g�rs den om till en shared pointer
//////////////////////////////////////////////////////////
void SoundScape::update(GameState* gameState){
	if (mBoolEntity == false){
		auto entity_wp = gameState->getEntity("player");
		auto entity_sp = std::shared_ptr<Entity>(entity_wp);
		auto player_sp = std::static_pointer_cast<Player>(entity_sp);
		mPlayer_wp = player_sp;
		if (getEnabled() == true){
			mSound.play();
		}
		mBoolEntity = true;
	}

//////////////////////////////////////////////////////////////////
// /n�r ljudet/l�ten stoppas s� ska det inte l�ngre vara aktiverat s� att man kan aktivera de igen
//////////////////////////////////////////////////////////////////
	if (mEnabledLastFrame && mSound.getStatus() == sf::Sound::Stopped){
		setEnabled(false);
	}
	auto enabledThisFrame = getEnabled();
	
//////////////////////////////////////////////////////////////////////
// /f�r att ett ljud/l�t ska b�rja att spela s� beh�ver det g� fr�n att inte vara aktiverad till att vara det
//////////////////////////////////////////////////////////////////////
	if (enabledThisFrame == true && mEnabledLastFrame == false){
		mSound.play();
	}
//////////////////////////////////////////////////////////////////////
// /Samma sak fast tv�rtom
//////////////////////////////////////////////////////////////////////
	else if (enabledThisFrame == false && mEnabledLastFrame == true){
		mSound.stop();
	}

	std::shared_ptr<Player> player_sp(mPlayer_wp);

	sf::Rect<float> soundScapeHitBox_r = getHitBox();
	sf::Rect<float> playerHitBox_r = player_sp->getHitBox();

	sf::Vector2f soundScapeHitBox(soundScapeHitBox_r.left, soundScapeHitBox_r.top);
	sf::Vector2f playerHitBox(playerHitBox_r.left, playerHitBox_r.top);
	sf::Vector2f playerToSoundVector(playerHitBox - soundScapeHitBox);


//////////////////////////////////////////////////////////////////////
// /fullVolumeRadius �r den radie som volymen ska vara satt till max i
// /distance �r avst�ndet mellan ljudk�llan och spelaren
// /maxRange �r den l�ngsta l�ngden som det kommer h�ras ljud fr�n
// /+ 0.0000001 �r d�r f�r att mRangeDecay ska kunna vara 0 (bakgrundsmusik som �r p� hela banan)
//////////////////////////////////////////////////////////////////////
	float volume;
	float fullVolumeRadius = X_STEP * mInnerRadius;
	float distance = sqrt(playerToSoundVector.x * playerToSoundVector.x + playerToSoundVector.y * playerToSoundVector.y);
	float maxRange = (X_STEP * 100/(mRangeDecay + 0.0000001)) + fullVolumeRadius;
	float volumeModifier;

	


//////////////////////////////////////////////////////////////////////
// /beroende p� vad soundTypen �r s� ska volymen vara olika fr�n vad anv�ndaren 
//  har satt f�r v�rden.
// /�r soundTypen en narrator s� h�mtas volymen f�r narrator via propertyManagersfunktion getUserSettings.
// /OSV. volumeModifier delas med 100 f�r att f� ett decimaltal som man kan g�ngra med mVolume senare
//////////////////////////////////////////////////////////////////////
	if (mSoundType == "narrator"){
		volumeModifier = PropertyManager::get().getUserSettings()->get<int>("narratorVolume");
		volumeModifier = volumeModifier/100;
	}
	else if (mSoundType == "sound"){
		volumeModifier = PropertyManager::get().getUserSettings()->get<int>("soundVolume");
		volumeModifier = volumeModifier/100;
	}
	else {
		volumeModifier = PropertyManager::get().getUserSettings()->get<int>("musicVolume");
		volumeModifier = volumeModifier/100;
	}


//////////////////////////////////////////////////////////////////////
// /om spelaren �r innanf�r radien som volymen ska vara satt till max s� ska ljudet vara max.
// /Annar om spelaren �r utanf�r radien d�r volymen ska vara satt till max men innanf�r
//  l�ngden som det ska komma n�gon volym ifr�n s� �r volymen beroende av 
//  maxVolymen g�nger ett tal som ska bli 0,n�tt beroende p� vart man st�r 
//  maxRange - distance �r f�r att f� hur stort talet �r
//  och / maxRange - fullVolumeRadius �r f�r att f� det till 0,n�tt
//////////////////////////////////////////////////////////////////////
	if (distance < fullVolumeRadius){
		volume = mVolume * volumeModifier;
	}
	else if (distance <= maxRange && distance >= fullVolumeRadius){
		volume = mVolume * volumeModifier * ((maxRange - distance) / (maxRange - fullVolumeRadius));
	}
	else
		volume = 0;

	mSound.setVolume(volume);

	mEnabledLastFrame = enabledThisFrame;
}

void SoundScape::drawSelf(){
	auto& manager = WindowManager::get();
	auto& window = *manager.getWindow();
	auto& states = *manager.getStates(); 

	auto& hitBox = getHitBox();
	auto position = sf::Vector2f(hitBox.left, hitBox.top);
	position = GAME_TO_SCREEN * position;


	for (int i = 0; i < 7; i++){
		position.x += i;

		sf::Vertex vertex[] = {sf::Vertex(position, sf::Color::Yellow)};
		window.draw(vertex, 1, sf::PrimitiveType::Points, states);

	}
}