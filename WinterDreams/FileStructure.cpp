#include "FileStructure.h"

#ifdef SHIPPING
const std::string FS_DIR_BACKGROUNDS = "Resources/Images/Backgrounds/";
const std::string FS_DIR_MAPS = "Resources/Images/Maps/";
const std::string FS_DIR_OBJECTANIMATIONS = "Resources/Images/Objects/";
const std::string FS_DIR_LEVELS = "Resources/Levels/";
const std::string FS_DIR_SOUNDS = "Resources/Sounds/";
const std::string FS_DIR_SHADERS = "Resources/Shaders/";
const std::string FS_DIR_UI = "Resources/Images/UI/";
const std::string FS_DIR_LOADINGSCREEN = "Resources/Images/LoadingScreen/";

#else
const std::string FS_DIR_BACKGROUNDS = "../Winter-Dreams/Resources/Images/Backgrounds/";
const std::string FS_DIR_MAPS = "../Winter-Dreams/Resources/Images/Maps/";
const std::string FS_DIR_OBJECTANIMATIONS = "../Winter-Dreams/Resources/Images/Objects/";
const std::string FS_DIR_LEVELS = "../Winter-Dreams/Resources/Levels/";
const std::string FS_DIR_SOUNDS = "../Winter-Dreams/Resources/Sounds/";
const std::string FS_DIR_SHADERS = "../Winter-Dreams/Resources/Shaders/";
const std::string FS_DIR_UI = "../Winter-Dreams/Resources/Images/UI/";
const std::string FS_DIR_LOADINGSCREEN = "../Winter-Dreams/Resources/Images/LoadingScreen/";
#endif

#ifdef _MSC_VER
extern const std::string FS_DIR_FONTS = "C:/Windows/Fonts/";
#elif defined __GNUG__
extern const std::string FS_DIR_FONTS = "/usr/share/fonts/truetype/msttcorefonts/";
#else
#error "No font directory set for this platform."
#endif
