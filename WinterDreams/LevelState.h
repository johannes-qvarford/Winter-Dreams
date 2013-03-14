#ifndef INCLUDED_LEVELSTATE
#define INCLUDED_LEVELSTATE

#include <memory>
#include <string>
#include "SFML\Audio\Sound.hpp"
#include <queue>
#include "State.h"
#include "SoundScape.h"

class SubLevel;
class Player;
class Camera;
class InventoryDisplay;


////////////////////////////////////////////////////////////
// /LevelState controls a collection of sublevels.
// /It owns a Player and a Camera, which exist in all sublevels.
// /Only one sublevel can be active(updated) at one time.
////////////////////////////////////////////////////////////
class LevelState : public State {
public:

	////////////////////////////////////////////////////////////
	// /construct a level with a name.
	////////////////////////////////////////////////////////////
	LevelState(const std::string& levelName);

	////////////////////////////////////////////////////////////
	// /Destructor.
	////////////////////////////////////////////////////////////
	~LevelState();

	////////////////////////////////////////////////////////////
	// /Update the state.
	////////////////////////////////////////////////////////////
	virtual void update();

	////////////////////////////////////////////////////////////
	// /Render the current frame.
	////////////////////////////////////////////////////////////
	virtual void render();

	////////////////////////////////////////////////////////////
	// /Add a sublevel to the level.
	////////////////////////////////////////////////////////////
	void addSubLevel(const std::string& name, std::shared_ptr<SubLevel> sublevel_sp);

	////////////////////////////////////////////////////////////
	// /Switch to a sublevel with a certain name.
	////////////////////////////////////////////////////////////
	void switchSubLevel(const std::string& name);

	////////////////////////////////////////////////////////////
	// /Get a sublevel with a certain name.
	////////////////////////////////////////////////////////////
	std::shared_ptr<SubLevel> getSubLevel(const std::string& name);

	////////////////////////////////////////////////////////////
	// /Set the level's player.
	////////////////////////////////////////////////////////////
	void setPlayer(std::shared_ptr<Player> player_sp);

	////////////////////////////////////////////////////////////
	// /Set the level's camera.
	////////////////////////////////////////////////////////////
	void setCamera(std::shared_ptr<Camera> camera_sp);

	////////////////////////////////////////////////////////////
	// /Set the level's inventory display.
	////////////////////////////////////////////////////////////
	void setInventoryDisplay(std::shared_ptr<InventoryDisplay> display_sp);

	////////////////////////////////////////////////////////////
	// /Get the level's player.
	////////////////////////////////////////////////////////////
	std::shared_ptr<Player> getPlayer();

	////////////////////////////////////////////////////////////
	// /Get the level's camera.
	////////////////////////////////////////////////////////////
	std::shared_ptr<Camera> getCamera();
	
	////////////////////////////////////////////////////////////
	// /Get the inventory display.
	////////////////////////////////////////////////////////////
	std::shared_ptr<InventoryDisplay> getInventoryDisplay();

	////////////////////////////////////////////////////////////
	// /Get the level's name.
	////////////////////////////////////////////////////////////
	const std::string& getLevelName() const;

	struct Narrator{
		SoundScape* mSoundScape_p;
		std::shared_ptr<sf::Sound> mSound_sp;
		std::shared_ptr<TextDisplay> mText_sp;
	}; 

	enum SoundType{
		SOUND,
		NARRATOR,
		MUSIC
	};

	void registerSound(std::shared_ptr<sf::Sound>, SoundType type);

	void onFreeze();

	void onUnfreeze();

	void queueNarrator(SoundScape* soundScape_p, std::shared_ptr<sf::Sound> sound_sp, std::string subs);


private:

	int mNextSpot;

	int mFinishedSpot;

	typedef std::map<std::string, std::shared_ptr<SubLevel> > SubLevels;

	bool mIngameMenu;

	SubLevels mSubLevels;

	std::queue<Narrator> mQueue;

	std::vector<std::weak_ptr<sf::Sound>> mRegSoundVecSound;

	std::vector<std::weak_ptr<sf::Sound>> mRegSoundVecMusic;

	SubLevels::iterator mCurrentSubLevel;

	std::shared_ptr<Player> mPlayer_sp;

	std::shared_ptr<Camera> mCamera_sp;

	std::shared_ptr<InventoryDisplay> mInventoryDisplay_sp;

	std::string mLevelName;

	////////////////////////////////////////////////////////////
	// /No copy.
	////////////////////////////////////////////////////////////
	LevelState(const LevelState&);

	////////////////////////////////////////////////////////////
	// /No copy.
	////////////////////////////////////////////////////////////
	LevelState& operator=(const LevelState&);
};


#endif