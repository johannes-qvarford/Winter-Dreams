#ifndef INCLUDED_GAMETOSCREEN
#define INCLUDED_GAMETOSCREEN

static const float SC_X_XOFFSET = 32.f;
static const float SC_X_YOFFSET = 16.f;
static const float X_STEP = 35.77708763999664f;

static const float SC_Y_XOFFSET = -32.f;
static const float SC_Y_YOFFSET = 16.f;
static const float Y_STEP = 35.77708763999664f;

static const sf::Transform GAME_TO_SCREEN(
	SC_X_XOFFSET / X_STEP,	SC_Y_XOFFSET / Y_STEP,	0,
	SC_X_YOFFSET / X_STEP,	SC_Y_YOFFSET / Y_STEP,	0,
	0,						0,						0
);

#endif