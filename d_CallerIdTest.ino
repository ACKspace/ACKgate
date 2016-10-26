/**
 * Test-file for CallerId.
 */

/*
 * Only compile this file
 * if the callerId-functions are being tested.
 */
#if (TEST == TEST_CALLER_ID)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning CallerIdTest
/**
 * Setup the serial-connection
 * and run all callerId-tests.
 */
void setup()
{
	setupSerial();
	testCallerIdLength();
	testGetStringFromCallerId();
	testGetCallerIdFromString();
	testAddCountryCallingCode();
}
/**
 * Test-function for the function "getCallerIdLength".
 */
void testCallerIdLength()
{
	compareUnsignedNumbers(1, getCallerIdLength(0ULL));
	compareUnsignedNumbers(1, getCallerIdLength(1ULL));
	compareUnsignedNumbers(1, getCallerIdLength(9ULL));
	compareUnsignedNumbers(2, getCallerIdLength(99ULL));
	compareUnsignedNumbers(3, getCallerIdLength(100ULL));
	compareUnsignedNumbers(9, getCallerIdLength(457112345ULL));
	compareUnsignedNumbers(11, getCallerIdLength(31457112345ULL));
}
/**
 * Test-function for the function "getStringFromCallerId".
 */
void testGetStringFromCallerId()
{
	char actual[MAX_CALLER_ID_LENGTH + 1];
	getStringFromCallerId(31457112345ULL, actual); 
	compareStrings("31457112345", actual);
	getStringFromCallerId(0, actual); 
	compareStrings("0", actual);
	getStringFromCallerId(123000321ULL, actual); 
	compareStrings("123000321", actual);
}
/**
 * Test-function for the function "getCallerIdFromString".
 */
void testGetCallerIdFromString()
{
	compareUnsignedNumbers(31457112345ULL, getCallerIdFromString("31457112345"));
	compareUnsignedNumbers(0, getCallerIdFromString(" 31457112345"));
	compareUnsignedNumbers(0, getCallerIdFromString("31 457112345"));
	compareUnsignedNumbers(314571123453145711ULL, getCallerIdFromString("314571123453145711"));
	compareUnsignedNumbers(0ULL, getCallerIdFromString("3145711234531457112345"));
	compareUnsignedNumbers(0ULL, getCallerIdFromString("314571123453145711234531457112345"));
}
/**
 * Test-function for the function "addCountryCallingCode".
 */
void testAddCountryCallingCode()
{
	compareUnsignedNumbers(1200, addCountryCallingCode(1200));
	compareUnsignedNumbers(7112345ULL, addCountryCallingCode(7112345ULL));
	compareUnsignedNumbers(31457112345ULL, addCountryCallingCode(457112345ULL));
	compareUnsignedNumbers(1234567890ULL, addCountryCallingCode(1234567890ULL));
}
#endif
