/**
 * This file contains functions
 * to communicate with the serial-connection.
 * These functions act as a proxy
 * for the real Serial.print and Serial.println,
 * so that the functions in this file can be called
 * without worrying about if
 * testing or debugging is turned on or off.
 */

/*
 * Only compile this file
 * if testing is being done
 * or debugging is enabled.
 */
#if ((TEST) || (DEBUG))
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Serial
/**
 * Setup the serial-connection for use.
 */
void setupSerial()
{
	DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
}
/**
 * Print a single character to serial.
 * @param c the character to be printed.
 */
void printChar(const char c)
{
	DEBUG_SERIAL.print(c);
}
/**
 * Print a string followed by a newline to serial.
 * @param str the string to be printed.
 */
void printString(const char* str)
{
	if (strlen(str) > 0)
	{
		DEBUG_SERIAL.println(str);
	}
}
/**
 * Print a callerId followed by a newline to serial.
 * @param cid the callerId to be printed.
 */
void printCallerId(const callerid_t cid)
{
	char cidStr[getCallerIdLength(cid) + 1];
	getStringFromCallerId(cid, cidStr);
	printString(cidStr);
}
/*
 * If no testing is being done
 * and debugging is disabled,
 * create dummy-functions.
 */
#else
void setupSerial() {}
void printChar(const char c) {}
void printString(const char* str) {}
void printCallerId(const callerid_t cid) {}
#endif
