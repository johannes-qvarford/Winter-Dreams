#ifndef INCLUDED_OBJECTTYPEREGISTRATION
#define INCLUDED_OBJECTTYPEREGISTRATION

#include "ObjectFactory.h"

#include <string>
class GameState;

////////////////////////////////////////////////////////////
// /Class whose constructor registers an object typename with
// /a callback function.
// /It purpose is to be constructed as global data(before main is called),
// /and the ability to register callback functions from any file.
////////////////////////////////////////////////////////////
class ObjectTypeRegistration {
public:

	////////////////////////////////////////////////////////////
	// /Register an object type with a callback function in ObjectFactory.
	////////////////////////////////////////////////////////////
	ObjectTypeRegistration(const std::string& objectTypename, ObjectFactory::Callback cb);

private:
	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	ObjectTypeRegistration(const ObjectTypeRegistration&);

	////////////////////////////////////////////////////////////
	// /No copying.
	////////////////////////////////////////////////////////////
	ObjectTypeRegistration& operator=(const ObjectTypeRegistration&);
};

#endif