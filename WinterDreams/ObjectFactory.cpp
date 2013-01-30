#include "ObjectFactory.h"
#include <boost/property_tree/ptree.hpp>

ObjectFactory& ObjectFactory::get() {
	static ObjectFactory sFac;
	return sFac;
}

ObjectFactory::ObjectFactory():
	mCallbacks()
{
}

void ObjectFactory::registerCallback(const std::string& objectTypename, ObjectFactory::Callback cb) {
	mCallbacks.insert(std::pair<std::string, Callback>(objectTypename, cb));
	return;
}

void ObjectFactory::callCallback(const std::string& objectTypename, GameState* state, const sf::Vector2f& position, const boost::property_tree::ptree& pt) {
	auto it = mCallbacks.find(objectTypename);
	
	if(it == mCallbacks.end())
		return;

	auto callback = it->second;
	callback(state, position, pt);
	return;
}