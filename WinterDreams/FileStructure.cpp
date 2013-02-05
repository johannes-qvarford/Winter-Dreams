#include "FileStructure.h"

#ifdef SHIPPING
const std::string FS_DIR_BACKGROUNDS = "Resources/Images/Backgrounds/";
const std::string FS_DIR_MAPS = "Resources/Images/Maps/";
const std::string FS_DIR_OBJECTANIMATIONS = "Resources/Images/Objects/";
const std::string FS_DIR_LEVELS = "Resources/Levels/";
#else
const std::string FS_DIR_BACKGROUNDS = "../Winter-Dreams/Resources/Images/Backgrounds/";
const std::string FS_DIR_MAPS = "../Winter-Dreams/Resources/Images/Maps/";
const std::string FS_DIR_OBJECTANIMATIONS = "../Winter-Dreams/Resources/Images/Objects/";
const std::string FS_DIR_LEVELS = "../Winter-Dreams/Resources/Levels/";
#endif