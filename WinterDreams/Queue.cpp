#include "Queue.h"
#include "SubLevel.h"
#include "QueueItem.h"

static void swapEntitiesStates(SubLevel* subLevel_p, QueueItem* queueItem_p);
static Entity* toEntity(SubLevel* subLevel_p, const std::string& name);

Queue::Queue(int numItems, const std::list<std::string>& initialQueueItems, bool startsEnabled):
	Entity(startsEnabled),
	mNumItems(numItems),
	mQueuedItems(),
	mInitialQueueItems(initialQueueItems),
	mHasQueuedInitial(false)
{
}

Queue::~Queue() {
}

void Queue::update(SubLevel* subLevel_p) {
	
	//all entities have to exist before being added to the queue.
	if(mHasQueuedInitial == false) {
		mHasQueuedInitial = true;

		int numItems = 0;
		//swap the state of each entity in the initial queue.
		for(auto it = mInitialQueueItems.begin(), end = mInitialQueueItems.end(); it != end; ++it) {
			auto& name = *it;
			//always add to queue
			queueItem_impl(subLevel_p, static_cast<QueueItem*>(toEntity(subLevel_p, name)), true);
			++numItems;
		}

		//list can't be to big
		assert(numItems <= mNumItems);
	}
}

void Queue::queueItem(SubLevel* subLevel_p, const std::string& queueItemName) {
	//only add to queue if enabled.
	queueItem_impl(subLevel_p, static_cast<QueueItem*>(toEntity(subLevel_p, queueItemName)), getEnabled());
}

void Queue::queueItem(SubLevel* subLevel_p, QueueItem* queueItem_p) {
	//only add to queue if enabled.
	queueItem_impl(subLevel_p, queueItem_p, getEnabled());
}

void Queue::queueItem_impl(SubLevel* subLevel_p, QueueItem* queueItem_p, bool doIt) {
	if(doIt == false)
		return;
	
	//swap incoming entitys state
	swapEntitiesStates(subLevel_p, queueItem_p);
	mQueuedItems.push(queueItem_p);

	if(mQueuedItems.size() > static_cast<unsigned>(mNumItems)) {
		//swap outgoing entitys state
		auto& queueItem_p = mQueuedItems.front();
		swapEntitiesStates(subLevel_p, queueItem_p);
		mQueuedItems.pop();
	}
}

static void swapEntitiesStates(SubLevel* subLevel_p, QueueItem* queueItem_p) {
	auto& nameList = queueItem_p->getEntities();

	//swap the state of all the entities in the queue item.
	for(auto it = nameList.begin(), end = nameList.end(); it != end; ++it) {
		auto& name = *it;
		auto entity_p = toEntity(subLevel_p, name);
		entity_p->swapEnabled();
	}
}

static Entity* toEntity(SubLevel* subLevel_p, const std::string& name) {
	auto entity_wp = subLevel_p->getEntity(name);
	return entity_wp.lock().get();
}
