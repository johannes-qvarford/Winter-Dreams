#ifndef INCLUDED_QUEUE
#define INCLUDED_QUEUE

#include "Script.h"

#include <list>
#include <string>
#include <queue>

class QueueItem;

////////////////////////////////////////////////////////////
// /Queue is a class used for tracking and changing the state of
// /a number of queued items. all entities in a queued item have their state swapped
// /when they get put into the queue, and pushed out of the queue.
////////////////////////////////////////////////////////////
class Queue : public Script {
public:
	////////////////////////////////////////////////////////////
	// /Create a queue that can hold a maximun number of items.
	// /Initialize it with an existing list.
	////////////////////////////////////////////////////////////
	Queue(int numItems, const std::list<std::string>& initialQueueItems, bool startsEnabled);

	////////////////////////////////////////////////////////////
	// /Destructor.
	////////////////////////////////////////////////////////////
	~Queue();

	////////////////////////////////////////////////////////////
	// /Draw nothing.
	////////////////////////////////////////////////////////////
	void draw() const;

	////////////////////////////////////////////////////////////
	// /Try to add the initial items to the queue if it hasn't already.
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

	////////////////////////////////////////////////////////////
	// /Queue an item, and swap its state.
	// /If the queue was full, push out the first item pushed in (FIFO),
	// /and swap its state.
	////////////////////////////////////////////////////////////
	void queueItem(SubLevel* subLevel_p, const std::string& queueItemName);

	////////////////////////////////////////////////////////////
	// /Queue an item, and swap its state.
	// /If the queue was full, push out the first item pushed in (FIFO),
	// /and swap its state.
	////////////////////////////////////////////////////////////
	void queueItem(SubLevel* subLevel_p, QueueItem* queueItem_p);

private:

	void queueItem_impl(SubLevel* subLevel_p, QueueItem* queueItem_p, bool doIt);

	int mNumItems;
	std::queue<QueueItem*> mQueuedItems;
	std::list<std::string> mInitialQueueItems;
	bool mHasQueuedInitial;
};

#endif