/*
#if ((TEST == TEST_QUEUE) || (ADMIN))
#warning Queue
#define QUEUE_LENGTH 4
#define QUEUE_EMPTY -1

struct queue_t
{
	uint8_t array[QUEUE_LENGTH];
	uint8_t length;
	int8_t get;
	int8_t set;
};

void initQueue(struct queue_t* queue)
{
	queue->length = QUEUE_LENGTH;
	queue->get = QUEUE_EMPTY;
	queue->set = 0;
}

boolean pushToQueue(struct queue_t* queue, uint8_t value)
{
	boolean pushed;
	if ((queue->get == queue->set) || (value == 0))
	{
		pushed = false;
	}
	else
	{
		queue->array[queue->set] = value;
		if (queue->get < 0)
		{
			queue->get = queue->set;
		}
		queue->set = (queue->set + 1) % queue->length;
		pushed = true;
	}
	return pushed;
}

uint8_t popFromQueue(struct queue_t* queue)
{
	uint8_t popped;
	if (queue->get < 0)
	{
		popped = 0;
	}
	else
	{
		popped = queue->array[queue->get];
		queue->get = (queue->get + 1) % queue->length;
		if (queue->get == queue->set)
		{
			queue->get = QUEUE_EMPTY;
		}
	}
	return popped;
}

boolean isQueueEmpty(struct queue_t* queue)
{
	return (queue->get == QUEUE_EMPTY);
}
#endif
*/