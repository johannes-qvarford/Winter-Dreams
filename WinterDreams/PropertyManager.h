#ifndef INCLUDED_PROPERTYMANAGER
#define INCLUDED_PROPERTYMANAGER

#include <boost/property_tree/ptree.hpp>
////////////////////////////////////////////////////////////
// /PropertyManager holds the settings for the game
// /Use it when there is a constant used, that will likely
// /change while playtestning.
////////////////////////////////////////////////////////////
class PropertyManager {
public:

	////////////////////////////////////////////////////////////
	// /Get the singleton.
	////////////////////////////////////////////////////////////
	static PropertyManager& get();

	////////////////////////////////////////////////////////////
	// /Get the general settings.
	////////////////////////////////////////////////////////////
	const boost::property_tree::ptree& getGeneralSettings() const;

	////////////////////////////////////////////////////////////
	// /Get the settings for objects.
	////////////////////////////////////////////////////////////
	const boost::property_tree::ptree& getObjectSettings() const;

private:
	
	////////////////////////////////////////////////////////////
	// /Default constructor.
	////////////////////////////////////////////////////////////
	PropertyManager();
	
	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	PropertyManager(const PropertyManager&);
	
	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	PropertyManager& operator=(const PropertyManager&);

	boost::property_tree::ptree mGeneralSettings;

	boost::property_tree::ptree mObjectSettings;
};

#endif