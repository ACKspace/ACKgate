/*
#if (TEST == TEST_QUEUE)
void setup()
{
	setupSerial();
	testQueue1();
	testQueue2();
	testQueue3();
}
void testQueue1()
{
	queue_t queue;
	initQueue(&queue);
	for(uint8_t i = 0; i < 5; i++)
	{
		compareUnsignedNumbers(0, popFromQueue(&queue));
		compareTrue(isQueueEmpty(&queue));
	}
}
void testQueue2()
{
	queue_t queue;
	initQueue(&queue);
	pushToQueue(&queue, 45);
	compareUnsignedNumbers(45, popFromQueue(&queue));
	compareUnsignedNumbers(0, popFromQueue(&queue));
}
void testQueue3()
{
	queue_t queue;
	initQueue(&queue);
	compareTrue(isQueueEmpty(&queue));
	pushToQueue(&queue, 45);
	compareFalse(isQueueEmpty(&queue));
	pushToQueue(&queue, 67);
	compareFalse(isQueueEmpty(&queue));
	pushToQueue(&queue, 89);
	compareFalse(isQueueEmpty(&queue));
	pushToQueue(&queue, 10);
	compareFalse(isQueueEmpty(&queue));
	compareUnsignedNumbers(45, popFromQueue(&queue));
	compareFalse(isQueueEmpty(&queue));
	compareUnsignedNumbers(67, popFromQueue(&queue));
	compareFalse(isQueueEmpty(&queue));
	compareUnsignedNumbers(89, popFromQueue(&queue));
	compareFalse(isQueueEmpty(&queue));
	compareUnsignedNumbers(10, popFromQueue(&queue));
	compareTrue(isQueueEmpty(&queue));
	compareUnsignedNumbers(0, popFromQueue(&queue));
	compareTrue(isQueueEmpty(&queue));
	
}
#endif
*/