/**
 * Test-file for Eeprom.
 */

/*
 * Only compile this file
 * if the eeprom-functions are being tested.
 */
#if (TEST == TEST_EEPROM)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning EepromTest
/**
 * Setup the serial-connection
 * and run all the eeprom-tests.
 */
void setup()
{
	setupSerial();
	testIsValidCallerId();
	testSIRG();
}
/**
 * Test-function for the function "isValidCallerId".
 */
void testIsValidCallerId()
{
	compareTrue(isValidCallerId(31457112345ULL));
	compareFalse(isValidCallerId(~0));
	compareFalse(isValidCallerId(0));
	compareTrue(isValidCallerId(0x123000321ULL));
	compareTrue(isValidCallerId(123000321ULL));
}
/**
 * Test-function for the functions:
 * - storeCallerIdInEeprom
 * - isCallerIdInEeprom
 * - removeCallerIdFromEeprom
 * - getNumberOfCallerIdsInEeprom
 */
void testSIRG()
{
	/*
	 * Clear the eeprom.
	 * Check that there are no callerIds
	 * stored in eeprom after it has been cleared.
	 */
	clearEeprom();
	compareUnsignedNumbers(0, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to store an invalid callerId.
	 * Check that there are
	 * still no callerIds in eeprom.
	 * The invalid callerId shouldn't
	 * get stored in eeprom.
	 */
	compareFalse(storeCallerIdInEeprom(~0));
	compareUnsignedNumbers(0, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to store 3 valid callerIds in eeprom.
	 * Check if they have been successfully stored.
	 */
	compareTrue(storeCallerIdInEeprom(31457112345ULL));
	compareTrue(storeCallerIdInEeprom(1));
	compareTrue(storeCallerIdInEeprom(0x1234567890ABCDEFULL));
	compareUnsignedNumbers(3, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to store 2 invalid callerIds
	 * when there are already some
	 * valid callerIds stored in eeprom.
	 * Check if the invalid callerIds
	 * didn't get stored.
	 */
	compareFalse(storeCallerIdInEeprom(0));
	compareFalse(storeCallerIdInEeprom(~0));
	compareUnsignedNumbers(3, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to check that the valid callerIds
	 * are stored in eeprom.
	 * Check that there are 3 callerIds in eeprom.
	 */
	compareTrue(isCallerIdInEeprom(0x1234567890ABCDEFULL));
	compareTrue(isCallerIdInEeprom(31457112345ULL));
	compareTrue(isCallerIdInEeprom(1));
	compareUnsignedNumbers(3, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to check if invalid callerIds
	 * are stored in eeprom.
	 * Check that there are still 3 callerIds in eeprom.
	 */
	compareFalse(isCallerIdInEeprom(0));
	compareFalse(isCallerIdInEeprom(~0));
	compareFalse(isCallerIdInEeprom(0x0900ACULL)); 
	compareUnsignedNumbers(3, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to remove 6 invalid callerIds.
	 * Check that there are still 3 callerIds in eeprom.
	 */
	compareFalse(removeCallerIdFromEeprom(~0));
	compareFalse(removeCallerIdFromEeprom(0));
	compareFalse(removeCallerIdFromEeprom(11));
	compareFalse(removeCallerIdFromEeprom(0x1234567890ABCDEFULL + 1));
	compareFalse(removeCallerIdFromEeprom(0x1234567890ABCDEFULL - 1));
	compareFalse(removeCallerIdFromEeprom(~0x1234567890ABCDEFULL));
	compareUnsignedNumbers(3, getNumberOfCallerIdsInEeprom());
	/*
	 * Remove the stored callerIds one by one
	 * and check each time that the number of callerIds
	 * stored in eeprom is correct.
	 */
	compareTrue(removeCallerIdFromEeprom(1));
	compareUnsignedNumbers(2, getNumberOfCallerIdsInEeprom());
	compareTrue(removeCallerIdFromEeprom(0x1234567890ABCDEFULL));
	compareUnsignedNumbers(1, getNumberOfCallerIdsInEeprom());
	compareTrue(removeCallerIdFromEeprom(31457112345ULL));
	compareUnsignedNumbers(0, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to store the maximum number of callerIds in eeprom.
	 * Check that the maximum number of callerIds are in eeprom.
	 */
	uint16_t maxNumberOfCallerIds = (E2END + 1) / sizeof(callerid_t);
	for(uint16_t n = 1; n <= maxNumberOfCallerIds; n++)
	{
		compareTrue(storeCallerIdInEeprom(n));
		compareUnsignedNumbers(n, getNumberOfCallerIdsInEeprom());
	}
	compareUnsignedNumbers(maxNumberOfCallerIds, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to check that all the callerIds are in eeprom.
	 * Check that the maximum number of callerIds are in eeprom.
	 */
	for(callerid_t n = 1; n <= maxNumberOfCallerIds; n++)
	{
		compareTrue(isCallerIdInEeprom(n));
	}
	compareUnsignedNumbers(maxNumberOfCallerIds, getNumberOfCallerIdsInEeprom());
	/*
	 * Try to add valid callerIds after the eeprom is full.
	 * Check that only the maximum number of callerIds are in eeprom.
	 */
	compareFalse(storeCallerIdInEeprom(maxNumberOfCallerIds + 1));
	compareUnsignedNumbers(maxNumberOfCallerIds, getNumberOfCallerIdsInEeprom());
	compareFalse(storeCallerIdInEeprom(maxNumberOfCallerIds + 317112345));
	compareUnsignedNumbers(maxNumberOfCallerIds, getNumberOfCallerIdsInEeprom());
	/*
	 * Remove a stored callerId from eeprom.
	 * Check that the number of callerIds in eeprom are correct.
	 */
	compareTrue(removeCallerIdFromEeprom(maxNumberOfCallerIds));
	compareUnsignedNumbers(maxNumberOfCallerIds - 1, getNumberOfCallerIdsInEeprom());
	/*
	 * Store another callerId in eeprom.
	 * Check that the number of callerIds in eeprom are correct.
	 */
	compareTrue(storeCallerIdInEeprom(maxNumberOfCallerIds + 1));
	compareUnsignedNumbers(maxNumberOfCallerIds, getNumberOfCallerIdsInEeprom());
	/*
	 * Remove another stored callerId from eeprom.
	 * Check that the number of callerIds in eeprom are correct.
	 */
	compareTrue(removeCallerIdFromEeprom(maxNumberOfCallerIds - 1));
	compareUnsignedNumbers(maxNumberOfCallerIds - 1, getNumberOfCallerIdsInEeprom());
	/*
	 * Remove another stored callerId from eeprom.
	 * Check that the number of callerIds in eeprom are correct.
	 */
	compareTrue(removeCallerIdFromEeprom(maxNumberOfCallerIds + 1));
	compareUnsignedNumbers(maxNumberOfCallerIds - 2, getNumberOfCallerIdsInEeprom());
	/*
	 * Remove all other stored callerIds from eeprom.
	 * Check that there are no more callerIds in eeprom.
	 */
	for(callerid_t n = maxNumberOfCallerIds - 2; n > 0; n--)
	{
		compareUnsignedNumbers(n, getNumberOfCallerIdsInEeprom());
		compareTrue(removeCallerIdFromEeprom(n));
	}
	compareUnsignedNumbers(0, getNumberOfCallerIdsInEeprom());
}
#endif
