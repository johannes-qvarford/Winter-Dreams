#include "ObjectTypeRegistration.h"

ObjectTypeRegistration::ObjectTypeRegistration(const std::string& objectTypename, ObjectFactory::Callback cb) {
	ObjectFactory::get().registerCallback(objectTypename, cb);
}