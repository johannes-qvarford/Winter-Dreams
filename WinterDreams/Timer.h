#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include "Script.h"

////////////////////////////////////////////////////////////
// /Timer is a script that counts 

////////////////////////////////////////////////////////////
class Timer : public Script {
public:

	Timer(int startFrames, bool startsEnabled);

};

#endif