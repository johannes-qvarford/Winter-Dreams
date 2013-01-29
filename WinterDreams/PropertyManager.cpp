#include "PropertyManager.h"
#include <boost/property_tree/json_parser.hpp>

#include <string>

static const std::string GENERAL_SETTINGS_FILENAME = "Settings.json";
static const std::string OBJECT_SETTINGS_FILENAME = "Objects.json";

PropertyManager& PropertyManager::get() {
	static PropertyManager sMgr;
	return sMgr;
}

PropertyManager::PropertyManager():
	mGeneralSettings(),
	mObjectSettings()
{
	using namespace boost::property_tree;
	boost::property_tree::
	json_parser::read_json(GENERAL_SETTINGS_FILENAME, mGeneralSettings);
	json_parser::read_json(OBJECT_SETTINGS_FILENAME, mObjectSettings);
}

const boost::property_tree::ptree& PropertyManager::getGeneralSettings() const {
	return mGeneralSettings;
}

const boost::property_tree::ptree& PropertyManager::getObjectSettings() const {
	return mObjectSettings;
}