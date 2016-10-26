/**
 * This file contains testing-functions.
 * It contains functions to compare
 * certain datatypes and then
 * display the result of the comparison.
 */

/*
 * Only compile this file
 * if a test is being run.
 */
#if (TEST)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Testing
/*
 * The text which will be printed 
 * when a test succeeds.
 */
#define TEXT_OK "V"
/*
 * The text which will be printed 
 * when a test fails.
 */
#define TEXT_NOT_OK "X"
/**
 * Compare the given boolean to false.
 * @param actual the boolean to be compared to false.
 */
void compareFalse(boolean actual)
{
	compareUnsignedNumbers(false, actual);
}
/**
 * Compare the given boolean to true.
 * @param actual the boolean to be compared to true.
 */
void compareTrue(boolean actual)
{
	compareUnsignedNumbers(true, actual);
}
/**
 * Compare 2 numbers.
 * @param expected the expected number.
 * @param actual the actual number. 
 */
void compareUnsignedNumbers(const uint64_t expected, const uint64_t actual)
{
	if (expected == actual)
	{
		ok();
	}
	else
	{
		notOk();
	}
}
/**
 * Compare 2 strings.
 * @param expected the expected string.
 * @param actual the actual string.
 */
void compareStrings(const char* expected, const char* actual)
{
	if ((strlen(expected) == strlen(actual)) && (!strcmp(expected, actual)))
	{
		ok();
	}
	else
	{
		notOk();
	}
}
/**
 * Called when a test succeeds.
 */
void ok()
{
	printString(TEXT_OK);
}
/**
 * Called when a test fails.
 */
void notOk()
{
	printString(TEXT_NOT_OK);
}
/**
 * Each test-file will only use the setup-function.
 * So an empty loop-function is just here
 * so that it doesn't have to be placed in every test-file. 
 */
void loop()
{ 
}
#endif
