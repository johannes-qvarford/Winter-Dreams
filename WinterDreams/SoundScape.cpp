#include "SoundScape.h"
#include "SubLevel.h"
#include "LevelState.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "WindowManager.h"
#include "GameToScreen.h"
#include "PropertyManager.h"
#include "TextDisplay.h"

#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

struct SoundScapeSpecs {
public:

	static SoundScapeSpecs& get(){ static SoundScapeSpecs s; return s; }

	std::map<std::string, std::string> mNarratorSoundToText;

private:

	SoundScapeSpecs() {
		auto& narrator = PropertyManager::get().getObjectSettings().get_child("objects.soundscape.narrator");
		for(auto it = narrator.begin(), end = narrator.end(); it != end; ++it) {
			auto name = it->first;
			auto text = it->second.get_value<std::string>();
			mNarratorSoundToText[name] = text;
		}
	}
};

SoundScape::SoundScape(sf::Rect<float> collisionBox, float innerRadius, int rangeDecay, float volume, bool loop, std::string soundName, bool startsEnabled, std::string soundType, int fadeInTime, bool threeD, SubLevel* subLevel_p):
CollisionZone(startsEnabled, collisionBox, false),
mBoolEntity(false),
mInnerRadius(innerRadius),
mRangeDecay(rangeDecay),
mVolume(volume),
mLoop(loop),
mSoundType(soundType),
mFadeInTime(fadeInTime),
mSoundName(soundName),
mEnabledLastFrame(startsEnabled),
mInitMusic(false),
mTotalVolume(0),
mClock(),
mThreeD(threeD),
mSpot(0),
//mICanHasNarratorSpot(false),
mHasNarratorPlayed(true),
mSound(new sf::Sound())
{
////////////////////////////////////////////////////////////////////////
// /FS_DIR_SOUNDS betyder: vart ligger filen, mSoundName betyder: vad heter filen
////////////////////////////////////////////////////////////////////////
	if (mSoundType == "sound")
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + mSoundName);
	else if (mSoundType == "narrator") {
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_NARRATORS + mSoundName);
		//mIsWaitingForSpot = true;
	}
	else
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_MUSIC + mSoundName);
	
	mSound->setBuffer(*mBuffer);
	mSound->setLoop(loop);
	mSound->setVolume(mVolume);
}

SoundScape::~SoundScape(){
	mSound->stop();
}





float SoundScape::getVolume(SubLevel* subLevel_p){

	std::shared_ptr<Player> player_sp(mPlayer_wp);

	sf::Rect<float> soundScapeHitBox_r = getHitBox();
	sf::Rect<float> playerHitBox_r = player_sp->getHitBox();

	sf::Vector2f soundScapeHitBox(soundScapeHitBox_r.left, soundScapeHitBox_r.top);
	sf::Vector2f playerHitBox(playerHitBox_r.left, playerHitBox_r.top);
	sf::Vector2f playerToSoundVector(playerHitBox - soundScapeHitBox);
	
//////////////////////////////////////////////////////////////////////
// /fullVolumeRadius är den radie som volymen ska vara satt till max i
// /distance är avståndet mellan ljudkällan och spelaren
// /maxRange är den längsta längden som det kommer höras ljud från
// /+ 0.0000001 är där för att mRangeDecay ska kunna vara 0 (bakgrundsmusik som är på hela banan)
//////////////////////////////////////////////////////////////////////

	float fullVolumeRadius = X_STEP * mInnerRadius;
	float distance = sqrt(playerToSoundVector.x * playerToSoundVector.x + playerToSoundVector.y * playerToSoundVector.y);
	float maxRange = (X_STEP * 100/(mRangeDecay + 0.0000001f)) + fullVolumeRadius;
	float volumeModifier;

//////////////////////////////////////////////////////////////////////
// /beroende på vad soundTypen är så ska volymen vara olika från vad användaren 
//  har satt för värden.
// /Är soundTypen en narrator så hämtas volymen för narrator via propertyManagersfunktion getUserSettings.
// /OSV. volumeModifier delas med 100 för att få ett decimaltal som man kan gångra med mVolume senare
//////////////////////////////////////////////////////////////////////
	if (mSoundType == "narrator"){
		volumeModifier = float(PropertyManager::get().getUserSettings()->get<int>("narratorVolume") );
		volumeModifier = float(volumeModifier/100 );
	}
	else if (mSoundType == "sound"){
		volumeModifier = float(PropertyManager::get().getUserSettings()->get<int>("soundVolume") );
		volumeModifier = float(volumeModifier/100 );
	}
	else {
		volumeModifier = float(PropertyManager::get().getUserSettings()->get<int>("musicVolume") );
		volumeModifier = float(volumeModifier/100 );
	}

//////////////////////////////////////////////////////////////////////
// /om spelaren är innanför radien som volymen ska vara satt till max så ska ljudet vara max.
// /Annar om spelaren är utanför radien där volymen ska vara satt till max men innanför
//  längden som det ska komma någon volym ifrån så är volymen beroende av 
//  maxVolymen gånger ett tal som ska bli 0,nått beroende på vart man står 
//  maxRange - distance är för att få hur stort talet är
//  och / maxRange - fullVolumeRadius är för att få det till 0,nått
//////////////////////////////////////////////////////////////////////

	sf::Time time = sf::milliseconds(0);
	sf::Time volumeUpdateTime = sf::milliseconds(mFadeInTime/100);

//////////////////////////////////////////////////////////////////////
// /Är det första gången en låt eller ett ljud startas så kan det vilja fade:as in
//////////////////////////////////////////////////////////////////////
	bool enabled = getEnabled();
	if (mSoundType == "music" && mInitMusic == false && getEnabled()  && mFadeInTime != 0){
		time = mClock.getElapsedTime();
		mClock.restart();

		if (mTotalVolume + 0.1 < mVolume * volumeModifier){
			float vol = float(time.asMicroseconds()) / (mFadeInTime * 1000);
			mTotalVolume +=  vol * (mVolume * volumeModifier);
			if(mTotalVolume > mVolume * volumeModifier)
				mTotalVolume = mVolume * volumeModifier;
			
		}

		if (mTotalVolume >= mVolume * volumeModifier){
			mInitMusic = true;
		}
	}

	if (mSoundType == "sound" && mThreeD){
		sf::Listener::setPosition(playerHitBox_r.left, playerHitBox_r.top, 0);
		//sf::Listener::setGlobalVolume(mVolume * volumeModifier);

		mSound->setPosition(soundScapeHitBox_r.left, soundScapeHitBox_r.top, -5);
		mSound->setMinDistance(mInnerRadius);
		mSound->setAttenuation(static_cast<float>(mRangeDecay) );
		mInitMusic = true;
	}

//////////////////////////////////////////////////////////////////////
// /om ljudet/musiken inte ska fade:as skall det gå igenom till nästa ifsats
//////////////////////////////////////////////////////////////////////
	if (mFadeInTime == 0)
		mInitMusic = true;

//////////////////////////////////////////////////////////////////////
// /om spelaren är innanför radien som volymen ska vara satt till max så ska ljudet vara max.
// /Annar om spelaren är utanför radien där volymen ska vara satt till max men innanför
//  längden som det ska komma någon volym ifrån så är volymen beroende av 
//  maxVolymen gånger ett tal som ska bli 0,nått beroende på vart man står 
//  maxRange - distance är för att få hur stort talet är
//  och / maxRange - fullVolumeRadius är för att få det till 0,nått
//////////////////////////////////////////////////////////////////////

	if (mSoundType == "narrator" || mInitMusic == true || !mThreeD){
		if (distance < fullVolumeRadius){
			mTotalVolume = mVolume * volumeModifier;
		}
		else if (distance <= maxRange && distance >= fullVolumeRadius){
			mTotalVolume = mVolume * volumeModifier * ((maxRange - distance) / (maxRange - fullVolumeRadius));
		}
		else
			mTotalVolume = 0;
	}
	float volume = mTotalVolume;

	return volume;
}




void SoundScape::update(SubLevel* subLevel_p){
	
//////////////////////////////////////////////////////////
// /player ska bara hämtas en gång och får inte vara en weak pointer så därför görs den om till en shared pointer
//////////////////////////////////////////////////////////
	if (mBoolEntity == false){
		auto player_sp = subLevel_p->getLevel()->getPlayer();
		mPlayer_wp = player_sp;
		if (mSoundType == "narrator")
			mHasNarratorPlayed = false;
		else
			mHasNarratorPlayed = true;

		if(mSoundName == "1002.ogg") {
			int a = 4;
		}

		subLevel_p->getLevel()->registerSound(mSound, (mSoundType == "narrator" ? LevelState::NARRATOR : mSoundType == "sound" ? LevelState::SOUND : LevelState::MUSIC));

		if (getEnabled() == true && mSoundType != "narrator"){
			mSound->play();
		}
		mBoolEntity = true;
	}
//////////////////////////////////////////////////////////////////
// /när ljudet/låten stoppas så ska det inte längre vara aktiverat så att man kan aktivera de igen
//////////////////////////////////////////////////////////////////
	if (mEnabledLastFrame && mSound->getStatus() == sf::Sound::Stopped && mHasNarratorPlayed){
		setEnabled(false);
	}
	auto enabledThisFrame = getEnabled();
	
//////////////////////////////////////////////////////////////////////
// /för att ett ljud/låt ska börja att spela så behöver det gå från att inte vara aktiverad till att vara det
//////////////////////////////////////////////////////////////////////
	if (enabledThisFrame == true && mEnabledLastFrame == false){
		if (mSoundType == "narrator"){
			//mSpot = subLevel_p->getLevel()->requestNarratorSpot();

						//try to find subtitles to narrators
			
			auto sss = SoundScapeSpecs::get();
			auto it = sss.mNarratorSoundToText.find(mSoundName);

			//empty string
			auto subs = std::string();
			if(it != sss.mNarratorSoundToText.end()) {
				//found match
				subs = it->second;
			}
			subLevel_p->getLevel()->queueNarrator(this, mSound, subs);
		}
		else {
			mClock.restart();
			mSound->play();
		}

	}

	/*if (mIsWaitingForSpot && subLevel_p->getLevel()->isSpotAvailable(mSpot) == true){
			mClock.restart();
			mSound->play();
			mIsWaitingForSpot = false;

	}*/

//////////////////////////////////////////////////////////////////////
// /Samma sak fast tvärtom
//////////////////////////////////////////////////////////////////////
	else if (enabledThisFrame == false && mEnabledLastFrame == true){
		mSound->stop();
		
		//kill text if it's still alive.
		if(auto text_sp = mText_wp.lock()) {
			text_sp->setAlive(false);
		}
	}
	

	float volume = getVolume(subLevel_p);


	

	mSound->setVolume(volume);


	mEnabledLastFrame = enabledThisFrame;
}


void SoundScape::setHasNarratorPlayed(bool played){
	mHasNarratorPlayed = played;
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
		//window.draw(vertex, 1, sf::Points, states);

	}
}