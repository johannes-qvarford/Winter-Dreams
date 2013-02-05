#include "PropertyManager.h"
#include <boost/property_tree/json_parser.hpp>

#include <string>

#ifdef SHIPPING
static const std::string GENERAL_SETTINGS_FILENAME = "Settings.json";
static const std::string OBJECTS_FILENAME = "Objects.json";
#else
static const std::string GENERAL_SETTINGS_FILENAME = "../Winter-Dreams/Settings.json";
static const std::string OBJECTS_FILENAME = "../Winter-Dreams/Objects.json";
#endif

PropertyManager& PropertyManager::get() {
	static PropertyManager sMgr;
	return sMgr;
}

PropertyManager::PropertyManager():
	mGeneralSettings(),
	mObjectSettings()
{
	using namespace boost::property_tree;
	json_parser::read_json(GENERAL_SETTINGS_FILENAME, mGeneralSettings);
#ifdef SHIPPING
	auto obj = mGeneralSettings.get<std::string>("objects");
	json_parser::read_json( obj , mObjectSettings );
#else
	auto obj = OBJECTS_FILENAME;
	json_parser::read_json( obj , mObjectSettings );
#endif

}

const boost::property_tree::ptree& PropertyManager::getGeneralSettings() const {
	return mGeneralSettings;
}

const boost::property_tree::ptree& PropertyManager::getObjectSettings() const {
	return mObjectSettings;
}