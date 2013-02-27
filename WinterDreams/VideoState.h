#ifndef INCLUDED_VIDEOSTATE
#define INCLUDED_VIDEOSTATE

#include "State.h"
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <memory>

namespace sftheora{
	class Video;
};
namespace sf{
	class Sound;
	class SoundBuffer;
};

////////////////////////////////////////////////////////////
// /State represent a state that the game can be in. 
// /It can be updated by a higher order class.
////////////////////////////////////////////////////////////
class VideoState : public State{
public:
	////////////////////////////////////////////////////////////
	// /The video state has to be told which file it should load.
	// /
	// /It will automaticly search in the FS_DIR_VIDEO for the
	// /argument file name.
	////////////////////////////////////////////////////////////
	VideoState(const std::string& videoFileName);
	////////////////////////////////////////////////////////////
	// /No dynamic member variables.
	////////////////////////////////////////////////////////////
	~VideoState();

	////////////////////////////////////////////////////////////
	// /Update the state of the video. Makes the video advance a
	// /number of frames, depending on time passed since last update.
	////////////////////////////////////////////////////////////
	void update();

	////////////////////////////////////////////////////////////
	// /Render the current frame of the video.
	////////////////////////////////////////////////////////////
	void render();

	////////////////////////////////////////////////////////////
	// /Starts the video states internal timers. Requered to be 
	// /called in order to make the video run for a correct 
	// /lenght of time.
	////////////////////////////////////////////////////////////
	void onUnfreeze();

protected:
	////////////////////////////////////////////////////////////
	// /Will queue a request to perform the onVideoEnd-function
	////////////////////////////////////////////////////////////
	void reqestVideoEnd();

private:
	////////////////////////////////////////////////////////////
	// /Each subclass of video state may define their own 
	// /"onVideoEnd" function.
	// /A subclass cannot call onVideoEnd. The only way to reach
	// /onVideoEnd is via the function requestVideoEnd.
	////////////////////////////////////////////////////////////
	virtual void onVideoEnd();

	std::shared_ptr<sftheora::Video> mVideo;	//The states video
	bool mRequestPerformed;	//True if a VideoEnd request has been performed

	sf::Time mVidLenght;	//Keeps track of the video's lenght
	sf::Clock mRunTime;		//Kepes track of how long time the video's been running	
	sf::Clock mDeltaTime;	//Used to calculate how many frames the video should advance
};

#endif