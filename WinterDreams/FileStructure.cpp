#include "FileStructure.h"

#ifdef SHIPPING
const std::string FS_DIR_MAIN = "Resources/";
#else
const std::string FS_DIR_MAIN = "../Winter-Dreams/Resources/";
#endif

const std::string FS_DIR_BACKGROUNDS		= FS_DIR_MAIN + "Images/Backgrounds/";
const std::string FS_DIR_MAPS				= FS_DIR_MAIN + "Images/Maps/";
const std::string FS_DIR_OBJECTANIMATIONS	= FS_DIR_MAIN + "Images/Objects/";
const std::string FS_DIR_LEVELS				= FS_DIR_MAIN + "Levels/";
const std::string FS_DIR_SOUNDS				= FS_DIR_MAIN + "Audio/Sounds/";
const std::string FS_DIR_NARRATORS			= FS_DIR_MAIN + "Audio/Narrative/";
const std::string FS_DIR_MUSIC				= FS_DIR_MAIN + "Audio/Music/";
const std::string FS_DIR_SHADERS			= FS_DIR_MAIN + "Shaders/";
const std::string FS_DIR_UI					= FS_DIR_MAIN + "Images/UI/";
const std::string FS_DIR_LOADINGSCREEN		= FS_DIR_MAIN + "Images/LoadingScreen/";
const std::string FS_DIR_VIDEO				= FS_DIR_MAIN + "Video/";

#ifdef _MSC_VER
extern const std::string FS_DIR_FONTS = "C:/Windows/Fonts/";
#elif defined __GNUG__
extern const std::string FS_DIR_FONTS = "/usr/share/fonts/truetype/msttcorefonts/";
#else
#error "No font directory set for this platform."
#endif
