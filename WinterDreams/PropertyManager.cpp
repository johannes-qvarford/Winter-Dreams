#include "PropertyManager.h"
#include <boost/property_tree/json_parser.hpp>

#include <string>

#ifdef SHIPPING
static const std::string GENERAL_SETTINGS_FILENAME = "Settings.json";
static const std::string OBJECTS_FILENAME = "Objects.json";
static const std::string USER_SETTINGS_FILENAME = "UserSettings.json";
#else
static const std::string GENERAL_SETTINGS_FILENAME = "../Winter-Dreams/Settings.json";
static const std::string OBJECTS_FILENAME = "../Winter-Dreams/Objects.json";
static const std::string USER_SETTINGS_FILENAME = "../Winter-Dreams/UserSettings.json";
#endif

PropertyManager& PropertyManager::get() {
	static PropertyManager sMgr;
	return sMgr;
}

PropertyManager::PropertyManager():
	mGeneralSettings(),
	mObjectSettings(),
	mUserSettings()
{
	using namespace boost::property_tree;
	json_parser::read_json(GENERAL_SETTINGS_FILENAME, mGeneralSettings);
	json_parser::read_json( OBJECTS_FILENAME , mObjectSettings );
	json_parser::read_json(USER_SETTINGS_FILENAME, mUserSettings);
}

const boost::property_tree::ptree& PropertyManager::getGeneralSettings() const {
	return mGeneralSettings;
}

const boost::property_tree::ptree& PropertyManager::getObjectSettings() const {
	return mObjectSettings;
}

boost::property_tree::ptree* PropertyManager::getUserSettings() {
	return &mUserSettings;
}