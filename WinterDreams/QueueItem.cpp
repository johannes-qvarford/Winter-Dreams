#include "QueueItem.h"
#include "Queue.h"

#include "SubLevel.h"

QueueItem::QueueItem(const std::string& queueName, const std::list<std::string>& entityNames, bool startsEnabled):
	Entity(startsEnabled),
	mQueueName(queueName),
	mEntities(entityNames)
{
}

QueueItem::~QueueItem() {
}

void QueueItem::update(SubLevel* subLevel_p) {
	//add itself to the queue if enabled, disable self afterwards
	if(getEnabled()) {
		setEnabled(false);

		auto entity_wp = subLevel_p->getEntity(mQueueName);
		auto queue_p = static_cast<Queue*>(entity_wp.lock().get());

		queue_p->queueItem(subLevel_p, this);
	}
}

const std::list<std::string>& QueueItem::getEntities() {
	return mEntities;
}