/**
 * Test-file for Telephone.
 */

/*
 * Only compile this file
 * if the telephone-functions are being tested.
 */
#if (TEST == TEST_TELEPHONE)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning TestTelephone
/**
 * Setup the serial-connection
 * and run all the telephone-tests.
 */
void setup()
{
	setupSerial();
	testGetCallerIdFromCall();
}
/**
 * Test-function for the function "getCallerIdFromCall".
 * This test is split in sub-tests for memory allocation reasons.
 */
void testGetCallerIdFromCall()
{
	testGetCallerIdFromCall1();
	testGetCallerIdFromCall2();
	testGetCallerIdFromCall3();
	testGetCallerIdFromCall4();
	testGetCallerIdFromCall5();
	testGetCallerIdFromCall6();
}
/**
 * Test-function #1 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall1()
{
	char call[] = "*ECAV: 1,6,1,,,\"31457112345\",145;";
	compareUnsignedNumbers(31457112345ULL, getCallerIdFromCall(call));
}
/**
 * Test-function #2 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall2()
{
	char call[] = "*ECAV: 1,6,1,,,\"31457112345,145;";
	compareUnsignedNumbers(0, getCallerIdFromCall(call));
}
/**
 * Test-function #3 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall3()
{
	char call[] = "*ECAV: 1,6,1,,,31457112345,145\";";
	compareUnsignedNumbers(0, getCallerIdFromCall(call));
}
/**
 * Test-function #4 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall4()
{
	char call[] = "*ECAV: 1,6,1,,,\"314571123a5\",145;";
	compareUnsignedNumbers(0, getCallerIdFromCall(call));
}
/**
 * Test-function #5 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall5()
{
	char call[] = "*ECAV: 1,6,1,,,\"3145711234531457112345\",145;";
	compareUnsignedNumbers(0, getCallerIdFromCall(call));
}
/**
 * Test-function #6 for the function "getCallerIdFromCall".
 */
void testGetCallerIdFromCall6()
{
	char call[] = "*ECAV: 1,6,1,,,\"314571123453145711234531457112345\",145;";
	compareUnsignedNumbers(0, getCallerIdFromCall(call));
}
#endif
