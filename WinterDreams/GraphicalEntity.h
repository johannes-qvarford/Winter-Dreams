#ifndef INCLUDED_GRAPHICALENTITY
#define INCLUDED_GRAPHICALENTITY

#include "PhysicalEntity.h"
////////////////////////////////////////////////////////////
// /Abstract base class for objects and points in the system
// /that require graphical representation or may require 
// /graphics in the future.
////////////////////////////////////////////////////////////
class GraphicalEntity : public PhysicalEntity {
public:
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of GraphicalEntity has to define a
	// /function for drawing itself.
	////////////////////////////////////////////////////////////
	virtual void drawSelf() = 0;
protected:
	GraphicalEntity( bool startEnabled=true ) : PhysicalEntity( startEnabled ) { }
};

#endif