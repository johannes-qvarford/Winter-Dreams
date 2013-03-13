#ifndef INCLUDED_SOUNDSCAPE
#define INCLUDED_SOUNDSCAPE

#include "SFML/Graphics/Rect.hpp"
#include "CollisionZone.h"
#include "Player.h"

class SoundScape : public CollisionZone {
public:



//////////////////////////////////////////////////////////////////////////////////////////////////
// /Konstruktorn behöver collisionBox för att veta vart soundscapen ska vara.
// /innerRadius behövs för att veta hur stor radien är för maxvolymen
// /rangeDecay behövs för att veta hur mycket i % som volymen ska minska per "tile"
// /volume behövs för att veta hur hög maxvolymen ska vara
// /loop behövs för att veta om ljudet/låten ska loopa
// /soundName behövs för att veta vilket ljud/låt det är som ska hämtas
// /startsEnabled behövs för att veta om ljudet ska köras direkt eller om det ska aktiveras först
// /soundType är för att bestämma om soundscapen är en narrator, sound eller music 
//////////////////////////////////////////////////////////////////////////////////////////////////
	SoundScape(sf::Rect<float> collisionBox, float innerRadius, int rangeDecay, float volume, bool loop, std::string soundName, bool startsEnabled, std::string soundType, int fadeInTime, bool threeD);

////////////////////////////////////////////////////////////////////////
// /stoppa ljudet/musiken
////////////////////////////////////////////////////////////////////////
	~SoundScape();


////////////////////////////////////////////////////////////////////////
// /Spela upp ljud/musik med full volym om man är inom en viss radie, spela det
// /lägre om man är utanför maxvoymradien men innanför maxlängdradien, loopa ljudet/låten
// /om den ska det.
////////////////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	void setHasNarratorPlayed(bool played);

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
	std::weak_ptr<Player> mPlayer_wp;
	bool mEnabledLastFrame;
	int mSpot;
	//bool mICanHasNarratorSpot;
	bool mHasNarratorPlayed;
	
	float getVolume(SubLevel* subLevel_p);

};

#endif
