#ifndef INCLUDED_QUEUEITEM
#define INCLUDED_QUEUEITEM

#include "Entity.h"

#include <string>
#include <list>

////////////////////////////////////////////////////////////
// /QueueItem is a script that is not meant to be used on its own.
// /It gets the name of a queue, and a list of names of entities that
// /should be added to the queue when it gets enabled
// /(usually by a trigger, one of its entities is often the same trigger).
////////////////////////////////////////////////////////////
class QueueItem : public Entity {
public:
	////////////////////////////////////////////////////////////
	// /Create a QueueItem that knows of a queue to add itself to,
	// /when it gets enabled.
	////////////////////////////////////////////////////////////
	QueueItem(const std::string& queueName, const std::list<std::string>& entityNames, bool startsEnabled);

	////////////////////////////////////////////////////////////
	// /Destructor.
	////////////////////////////////////////////////////////////
	~QueueItem();

	////////////////////////////////////////////////////////////
	// /Update the queue by checking whether or not its enabled.
	// /it will add itself to the queue if so, and then disable itself.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	////////////////////////////////////////////////////////////
	// /Get the entities tied to the queue item.
	////////////////////////////////////////////////////////////
	const std::list<std::string>& getEntities();

private:
	std::string mQueueName;
	std::list<std::string> mEntities;

};

#endif