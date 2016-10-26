/**
 * This file will handle all communication with the telephone.
 * It will setup a connection to the telephone and
 * send initial commands to the telephone.
 * After that a loop will be entered
 * which will process the input from the telephone
 * for incoming calls and messages.
 */

/*
 * Only compile this file
 * if the telephone-functions are being tested
 * or the main-program is being run.
 */
#if ((TEST == TEST_TELEPHONE) || (!TEST))
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Telephone
/*
 * Generate a warning to determine
 * if debugging is enabled or disabled.
 */
#if (DEBUG)
#warning with debug
#else
#warning without debug
#endif
/*
 * Generate a warning to determine
 * if admin is enabled or disabled.
 */
#if (ADMIN)
#warning with admin
#else
#warning without admin
#endif
/*
 * The length of the buffer which is used
 * to read a line from the telephone.
 */
#define TELEPHONE_READ_BUFFER_LENGTH 384
/*
 * The request send to the telephone
 * which will turn call monitoring on.
 * Without this the telephone won't send the callerId.
 */
//#define AT_CALL_MONITORING_ON "AT*ECAM=1"
#define AT_CALL_MONITORING_ON "AT+CLIP=1"
/*
 * The part of the response
 * from the telephone
 * before the callerId
 * when there is an incoming call.
 */
//#define AT_CALL_PRE "*ECAV: 1,6,1,,,\""
#define AT_CALL_PRE "+CLIP: \""
/*
 * The part of the response
 * from the telephone
 * after the callerId
 * when there is an incoming call.
 */
//#define AT_CALL_POST "\""
#define AT_CALL_POST "\""
/*
 * The amount of milliseconds
 * for which the program will be delayed
 * after a command has been
 * written to the telephone.
 */
#define TELEPHONE_WRITE_DELAY 1000
/*
 * Only enable the sms specific functionality
 * if the administration-interface is enabled.
 */
#if ADMIN
/*
 * The request send to the telephone
 * for storing messages
 * on the internal storage of the phone.
 */
#define AT_MESSAGE_STORAGE_INTERNAL "AT+CPMS=\"ME\""
/*
 * The request send to the telephone
 * which will sent unsollicited response
 * about newly received messages to the arduino.
 */
#define AT_MESSAGE_SEND_TO_TERMINAL "AT+CNMI=2,3,0,0,0"

//#define AT_INCOMING_MESSAGE "+CMTI"

//#define AT_GET_MESSAGE "AT+CMGR="

/*
 * The request send to the telephone
 * which will delete the sms.
 */
//#define AT_MESSAGE_DELETE "AT+CMGD="

//queue_t smsQueue;
//uint8_t smsToBeDeleted;
#endif
/**
 * Setup the telephone:
 * - Initialise the serial-connection
 *   to the telephone.
 * - Send certain AT-commands
 *   to the telephone.
 */ 
void setupTelephone()
{
	TELEPHONE_SERIAL.begin(TELEPHONE_SERIAL_BAUD_RATE); 
	writeAndReadTelephone("ATE1");
	writeAndReadTelephone(AT_CALL_MONITORING_ON); 
	#if ADMIN
	writeAndReadTelephone(AT_MESSAGE_STORAGE_INTERNAL); 
	writeAndReadTelephone(AT_MESSAGE_SEND_TO_TERMINAL);
	//initQueue(&smsQueue);
	//smsToBeDeleted = 0;
	#endif
}
/**
 * Write data to the telephone.
 * Read data from the telephone
 * and, if debugging is enabled,
 * display it on the serial-connection.
 * @param str the data to be written.
 */
void writeAndReadTelephone(const char* str)
{
	writeTelephone(str);
	readTelephone();
}
/**
 * Write data to the telephone.
 * @param str the data to be written.
 */
void writeTelephone(const char* str)
{
	TELEPHONE_SERIAL.println(str);
	delay(TELEPHONE_WRITE_DELAY);
}
/**
 * Read data from the telephone
 * and, if debugging is enabled,
 * display it on the serial-connection.
 */
void readTelephone()
{
	while(TELEPHONE_SERIAL.available())
	{
		printChar(TELEPHONE_SERIAL.read());
	}
}
/**
 * This function can be called
 * from the loop-function.
 * The function declares an character-buffer,
 * reads a line from the telephone into that buffer,
 * print the line to serial-connection
 * if debugging is enabled
 * and processes the line.
 */
void loopTelephone()
{
	char line[TELEPHONE_READ_BUFFER_LENGTH];
	readLineTelephone(line);
	printString(line);
	processLineTelephone(line);
	//#if ADMIN
	//processSmsQueue();
	//#endif
}
/**
 * Read a line from the telephone.
 * This function will block
 * until a line is succesfully read.
 * This function won't read
 * past the length of the given buffer.
 * @param line the buffer for the
 * incoming line from the telephone.
 */
void readLineTelephone(char* line)
{
	boolean done = false;
	uint16_t pos = 0;
	while(!done)
	{
		if (TELEPHONE_SERIAL.available())
		{
			char c = (char) TELEPHONE_SERIAL.read();
			if ((c == '\n') || (c == '\r') || (pos >= (TELEPHONE_READ_BUFFER_LENGTH - 1)))
			{
				pos = min(pos, TELEPHONE_READ_BUFFER_LENGTH - 1);
				line[pos] = '\0';
				done = true;
			}
			else
			{
				line[pos] = c;
				pos++;
			}
		}
	}
}
/**
 * Process a line received from the telephone
 * for incoming calls or incoming messages. 
 * @param line a line received from the telephone.
 */
void processLineTelephone(char* line)
{
	/*
	 * If there is a incoming call:
	 * - retrieve the callerId.
	 * - add the country calling code
	 *   if it isn't there.
	 * - print to callerId to serial-connection
	 *   if debugging is turned on.
	 * - invoke the defined function
	 *   to handle the incoming call.
	 */
	if ((strstr(line, AT_CALL_PRE)) && (strstr(line, AT_CALL_POST)))
	{
		callerid_t cid = getCallerIdFromCall(line);
		cid = addCountryCallingCode(cid);
		printCallerId(cid);
		TELEPHONE_INCOMING_CALL_FUNCTION(cid);
		hangupTelephone();
	}
	#if ADMIN
	//else if (strstr(line, AT_INCOMING_MESSAGE))
	//{
	//	pushToQueue(&smsQueue, getMessageNumberFromIncomingMessage(line));
	//}
	/*
	 * If there is an incoming message:
	 * - Get the callerId and message.
	 * - Print the callerId and message
	 *   if debugging is turned on.
	 * - invoke the defined function
	 *   to handle the incoming message.
	 * - delete the message on the telephone.
	 */
	else if (isValidPdu(line))
	{
		callerid_t cid = getCallerIdFromPdu(line);
		cid = addCountryCallingCode(cid);
		getMessageFromPdu(line);
		printCallerId(cid);
		printString(line);
		TELEPHONE_INCOMMING_MESSAGE_FUNCTION(cid, line);
		deleteMostRecentMessageFromTelephone();
	}
	#endif
}
/**
 * Get the callerId from the call-response
 * received from the telephone.
 * @param call the call-response
 * received from the telephone.
 * @return the callerId
 * or 0 if there is no valid Callerid.
 */
callerid_t getCallerIdFromCall(char* call)
{
	callerid_t cid = 0;
	char* cidBegin = strstr(call, AT_CALL_PRE);
	if (cidBegin)
	{
		cidBegin += strlen(AT_CALL_PRE);
		char* cidEnd = strstr(cidBegin, AT_CALL_POST);
		if (cidEnd)
		{
			cidEnd[0] = '\0';
			cid = getCallerIdFromString(cidBegin);
		}
	}
	return cid;
}

/**
 * Deletes the most recent message
 * from telephone-memory.
 */
void deleteMostRecentMessageFromTelephone()
{
	writeTelephone("AT+CMGD=1");
}
/**
 * Hangs up an incoming call.
 */
void hangupTelephone()
{
	writeAndReadTelephone("AT+CHUP");
	writeAndReadTelephone("ATH0");
}

//#if ADMIN
//uint8_t getMessageNumberFromIncomingMessage(const char* incomingMessage)
//{
//	uint8_t messageNumber = 0;
//	char* number = strstr(incomingMessage, ",");
//	if (number)
//	{
//		number++;
//		messageNumber = atoi(number);
//	}
//	return messageNumber;
//}

//void processSmsQueue()
//{
//	if (smsToBeDeleted > 0)
//	{		
//		smsToBeDeleted = 0;
//	}
//	else if (!isQueueEmpty(&smsQueue))
//	{
//		smsToBeDeleted = popFromQueue(&smsQueue);
//		Serial.println(smsToBeDeleted);
//	}
//}
//#endif

#endif