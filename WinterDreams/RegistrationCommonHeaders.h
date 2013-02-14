#ifndef INCLUDED_REGISTRATIONCOMMONHEADERS
#define INCLUDED_REGISTRATIONCOMMONHEADERS

#include "ObjectTypeRegistration.h"
#include "SubLevel.h"
#include "LevelState.h"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include "GameToScreen.h"

#include <sstream>

inline void splitString(const std::string& str, std::list<std::string>* strList_p) {
	std::stringstream ss(str);
	std::string temp;
	while(std::getline(ss, temp, ',')) {
		strList_p->push_back(temp);
	}
}


#endif