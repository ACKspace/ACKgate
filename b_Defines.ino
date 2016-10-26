/**
 * This file contains only defines
 * and acts as a configuration-file.
 */
 
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Defines
/*
 * The serial-device which will
 * be used for debugging purposes.
 */
#define DEBUG_SERIAL Serial
/*
 * The baudrate which will be used
 * for the serial port.
 */
#define DEBUG_SERIAL_BAUD_RATE 9600
/*
 * The serial-device to which
 * the telephone will be connected.
 */
#define TELEPHONE_SERIAL Serial1
/*
 * The baudrate which will be used
 * for the telephone.
 */
#define TELEPHONE_SERIAL_BAUD_RATE 9600
/*
 * The name of the function
 * which will be called on an incoming call.
 * It has to have the following header:
 * function(callerid_t cid)
 */
#define TELEPHONE_INCOMING_CALL_FUNCTION incomingCall
/*
 * The name of the function
 * which will be called on an incoming call.
 * It has to have the following header:
 * function(callerid_t cid, char* message)
 */
#define TELEPHONE_INCOMMING_MESSAGE_FUNCTION incommingMessage
/*
 * The country calling code. When a call is made, 
 * some telephone-providers provide the callerId
 * without the country calling code,
 * so it needs to be added.
 */
#define COUNTRY_CALLING_CODE 31
/*
 * If the callerId has this length
 * the country calling code needs to be added.
 */
#define CALLER_ID_WITHOUT_COUNTRY_CALLING_CODE_LENGTH 9
/*
 * The pin which will be used
 * for opening the gate.
 */
#define GATE_OUTPUT_PIN 7
/*
 * The delay between the HIGH-
 * and LOW-state when opening the gate.
 */
#define GATE_OPEN_DELAY 500
/*
 * Test the callerId-functions.
 */
#define TEST_CALLER_ID 1
/*
 * Test the eeprom-functions.
 */
#define TEST_EEPROM 2
/*
 * Test the pdu-functions.
 */
#define TEST_PDU 4
/*
 * Test the telephone-functions.
 */
#define TEST_TELEPHONE 8
/*
 * Test the queue-functions.
 */
#define TEST_QUEUE 16
/*
 * This is the test (or main-program)
 * which will be run.
 */
#define TEST TEST_NONE
/*
 * Enable or disable debugging.
 * If this is set to 0, nothing will be printed to serial.
 * If this isn't set to 0, debugging-information
 * will be printed to serial.
 */
#define DEBUG 1
/*
 * Enable or disable administration-interface.
 * If this is set to 0, the administration-interface
 * will be disabled.
 * If this isn't set to 0, the administration-interface
 * will be enabled.
 */
#define ADMIN 0