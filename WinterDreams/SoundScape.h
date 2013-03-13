#ifndef INCLUDED_SOUNDSCAPE
#define INCLUDED_SOUNDSCAPE

#include "SFML/Graphics/Rect.hpp"
#include "CollisionZone.h"
#include "Player.h"
#include "TextDisplay.h"

class SoundScape : public CollisionZone {
public:

//////////////////////////////////////////////////////////////////////////////////////////////////
// /Konstruktorn beh�ver collisionBox f�r att veta vart soundscapen ska vara.
// /innerRadius beh�vs f�r att veta hur stor radien �r f�r maxvolymen
// /rangeDecay beh�vs f�r att veta hur mycket i % som volymen ska minska per "tile"
// /volume beh�vs f�r att veta hur h�g maxvolymen ska vara
// /loop beh�vs f�r att veta om ljudet/l�ten ska loopa
// /soundName beh�vs f�r att veta vilket ljud/l�t det �r som ska h�mtas
// /startsEnabled beh�vs f�r att veta om ljudet ska k�ras direkt eller om det ska aktiveras f�rst
// /soundType �r f�r att best�mma om soundscapen �r en narrator, sound eller music 
//////////////////////////////////////////////////////////////////////////////////////////////////
	SoundScape(sf::Rect<float> collisionBox, float innerRadius, int rangeDecay, float volume, bool loop, std::string soundName, bool startsEnabled, std::string soundType, int fadeInTime, bool threeD, SubLevel* subLevel_p);

////////////////////////////////////////////////////////////////////////
// /stoppa ljudet/musiken
////////////////////////////////////////////////////////////////////////
	~SoundScape();


////////////////////////////////////////////////////////////////////////
// /Spela upp ljud/musik med full volym om man �r inom en viss radie, spela det
// /l�gre om man �r utanf�r maxvoymradien men innanf�r maxl�ngdradien, loopa ljudet/l�ten
// /om den ska det.
////////////////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);


	void drawSelf();

private:

	bool mBoolEntity;
	float mInnerRadius;
	int mRangeDecay;
	float mVolume;
	bool mLoop;
	bool mInitMusic;
	float mTotalVolume;
	bool mThreeD;
	sf::Clock mClock;
	int mFadeInTime;
	std::string mSoundType;
	std::string mSoundName;
	std::shared_ptr<sf::SoundBuffer> mBuffer;
	std::shared_ptr<sf::Sound> mSound;
	std::weak_ptr<TextDisplay> mText_wp;
	std::weak_ptr<Player> mPlayer_wp;
	bool mEnabledLastFrame;
	int mSpot;
	bool mICanHasNarratorSpot;
	bool mIsWaitingForSpot;
	
	float getVolume(SubLevel* subLevel_p);

};

#endif
