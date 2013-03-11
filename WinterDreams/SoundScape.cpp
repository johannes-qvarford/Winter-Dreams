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
mSound(new sf::Sound())
{
////////////////////////////////////////////////////////////////////////
// /FS_DIR_SOUNDS betyder: vart ligger filen, mSoundName betyder: vad heter filen
////////////////////////////////////////////////////////////////////////
	if (mSoundType == "sound")
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_SOUNDS + mSoundName);
	else if (mSoundType == "narrator")
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_NARRATORS + mSoundName);
	else
		mBuffer = ResourceManager::get().getSoundBuffer(FS_DIR_MUSIC + mSoundName);
	
	mSound->setBuffer(*mBuffer);
	mSound->setLoop(loop);
	mSound->setVolume(mVolume);

	//try to find subtitles to narrators
	auto sss = SoundScapeSpecs::get();
	auto it = sss.mNarratorSoundToText.find(mSoundName);

	if(it != sss.mNarratorSoundToText.end()) {

		//found match

		auto subs = it->second;

		std::vector<TextDisplay::TimedText> timedTexts;

		//std::string str1("[t0]and stuff[t2000]hello mom!");
    
		std::vector<std::string> splitVec;
		boost::split( splitVec, subs, boost::is_any_of("[]"), boost::token_compress_on );

		for(size_t i = 1; i < splitVec.size(); i+=2) {
			TextDisplay::TimedText tt;
			tt.mText = splitVec[i+1];
			tt.mTimestamp = (std::atoi(splitVec[i].c_str()+1)) / 1000.f * 60;
			timedTexts.push_back(tt);
		}

		auto text_sp = std::make_shared<TextDisplay>(timedTexts, sf::Vector2f(0.5, 0.5), true);
		subLevel_p->addScript(text_sp);
	}
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
// /fullVolumeRadius �r den radie som volymen ska vara satt till max i
// /distance �r avst�ndet mellan ljudk�llan och spelaren
// /maxRange �r den l�ngsta l�ngden som det kommer h�ras ljud fr�n
// /+ 0.0000001 �r d�r f�r att mRangeDecay ska kunna vara 0 (bakgrundsmusik som �r p� hela banan)
//////////////////////////////////////////////////////////////////////

	float fullVolumeRadius = X_STEP * mInnerRadius;
	float distance = sqrt(playerToSoundVector.x * playerToSoundVector.x + playerToSoundVector.y * playerToSoundVector.y);
	float maxRange = (X_STEP * 100/(mRangeDecay + 0.0000001f)) + fullVolumeRadius;
	float volumeModifier;


//////////////////////////////////////////////////////////////////////
// /beroende p� vad soundTypen �r s� ska volymen vara olika fr�n vad anv�ndaren 
//  har satt f�r v�rden.
// /�r soundTypen en narrator s� h�mtas volymen f�r narrator via propertyManagersfunktion getUserSettings.
// /OSV. volumeModifier delas med 100 f�r att f� ett decimaltal som man kan g�ngra med mVolume senare
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
// /om spelaren �r innanf�r radien som volymen ska vara satt till max s� ska ljudet vara max.
// /Annar om spelaren �r utanf�r radien d�r volymen ska vara satt till max men innanf�r
//  l�ngden som det ska komma n�gon volym ifr�n s� �r volymen beroende av 
//  maxVolymen g�nger ett tal som ska bli 0,n�tt beroende p� vart man st�r 
//  maxRange - distance �r f�r att f� hur stort talet �r
//  och / maxRange - fullVolumeRadius �r f�r att f� det till 0,n�tt
//////////////////////////////////////////////////////////////////////




	sf::Time time = sf::milliseconds(0);
	sf::Time volumeUpdateTime = sf::milliseconds(mFadeInTime/100);

	
//////////////////////////////////////////////////////////////////////
// /�r det f�rsta g�ngen en l�t eller ett ljud startas s� kan det vilja fade:as in
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
// /om ljudet/musiken inte ska fade:as skall det g� igenom till n�sta ifsats
//////////////////////////////////////////////////////////////////////
	if (mFadeInTime == 0)
		mInitMusic = true;


//////////////////////////////////////////////////////////////////////
// /om spelaren �r innanf�r radien som volymen ska vara satt till max s� ska ljudet vara max.
// /Annar om spelaren �r utanf�r radien d�r volymen ska vara satt till max men innanf�r
//  l�ngden som det ska komma n�gon volym ifr�n s� �r volymen beroende av 
//  maxVolymen g�nger ett tal som ska bli 0,n�tt beroende p� vart man st�r 
//  maxRange - distance �r f�r att f� hur stort talet �r
//  och / maxRange - fullVolumeRadius �r f�r att f� det till 0,n�tt
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
// /player ska bara h�mtas en g�ng och f�r inte vara en weak pointer s� d�rf�r g�rs den om till en shared pointer
//////////////////////////////////////////////////////////
	if (mBoolEntity == false){
		auto player_sp = subLevel_p->getLevel()->getPlayer();
		mPlayer_wp = player_sp;
		if (getEnabled() == true){
			mSound->play();
		}
		mBoolEntity = true;
	}
//////////////////////////////////////////////////////////////////
// /n�r ljudet/l�ten stoppas s� ska det inte l�ngre vara aktiverat s� att man kan aktivera de igen
//////////////////////////////////////////////////////////////////
	if (mEnabledLastFrame && mSound->getStatus() == sf::Sound::Stopped){
		setEnabled(false);
	}
	auto enabledThisFrame = getEnabled();
	
//////////////////////////////////////////////////////////////////////
// /f�r att ett ljud/l�t ska b�rja att spela s� beh�ver det g� fr�n att inte vara aktiverad till att vara det
//////////////////////////////////////////////////////////////////////
	if (enabledThisFrame == true && mEnabledLastFrame == false){
		mClock.restart();
		mSound->play();
	}
//////////////////////////////////////////////////////////////////////
// /Samma sak fast tv�rtom
//////////////////////////////////////////////////////////////////////
	else if (enabledThisFrame == false && mEnabledLastFrame == true){
		mSound->stop();
	}

	float volume = getVolume(subLevel_p);


	

	mSound->setVolume(volume);


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
		window.draw(vertex, 1, sf::Points, states);

	}
}