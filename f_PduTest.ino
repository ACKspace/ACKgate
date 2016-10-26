/**
 * Test-file for Pdu.
 */

/*
 * Only compile this file
 * if the pdu-functions are being tested.
 */
#if (TEST == TEST_PDU)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning TestPdu
/**
 * Setup the serial-connection
 * and run all the pdu-tests.
 */
void setup()
{
	setupSerial();
	testIsHexDigit();
	testIsHexString();
	testGetValueFromHexChar();
	testGetValueFromHexPairFromString();
	testGetSmscLengthFromPdu();
	testGetSenderLengthFromPdu();
	testIsValidPdu();
	testGetCallerIdFromPdu();
	testConvertInvalidCharactersFromMessage();
	testGetMessageFromPdu();
}
/**
 * Test-function for the function "isHexDigit".
 */
void testIsHexDigit()
{
	compareTrue(isHexDigit('0'));
	compareTrue(isHexDigit('5'));
	compareTrue(isHexDigit('9'));
	compareTrue(isHexDigit('B'));
	compareTrue(isHexDigit('D'));
	compareTrue(isHexDigit('E'));
	compareFalse(isHexDigit('a'));
	compareFalse(isHexDigit('c'));
	compareFalse(isHexDigit('f'));
	compareFalse(isHexDigit('g'));
	compareFalse(isHexDigit('!'));
	compareFalse(isHexDigit(' '));
}
/**
 * Test-function for the function "isHexString".
 */
void testIsHexString()
{
	compareTrue(isHexString("0AB12FF9000"));
	compareTrue(isHexString(""));
	compareTrue(isHexString("A"));
	compareFalse(isHexString("ABCDEFG"));
	compareFalse(isHexString(" ABCDEF"));
	compareFalse(isHexString("ABC\nDEF"));
}
/**
 * Test-function for the function "getValueFromHexChar".
 */
void testGetValueFromHexChar()
{
	compareUnsignedNumbers(0, getValueFromHexChar('!'));
	compareUnsignedNumbers(0, getValueFromHexChar('0'));
	compareUnsignedNumbers(1, getValueFromHexChar('1'));
	compareUnsignedNumbers(6, getValueFromHexChar('6'));
	compareUnsignedNumbers(9, getValueFromHexChar('9'));
	compareUnsignedNumbers(10, getValueFromHexChar('A'));
	compareUnsignedNumbers(15, getValueFromHexChar('F'));
	compareUnsignedNumbers(0, getValueFromHexChar('G'));
}
/**
 * Test-function for the function "getValueFromHexPairFromString".
 */
void testGetValueFromHexPairFromString()
{
	compareUnsignedNumbers(0x12, getValueFromHexPairFromString("12", 0));
	compareUnsignedNumbers(0xEF, getValueFromHexPairFromString("DEF", 1));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("1", 0));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("FFF", 2));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("GH", 0));
	compareUnsignedNumbers(0x67, getValueFromHexPairFromString("1234567890", 5));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("12345 7890", 5));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("123456 890", 5));
	compareUnsignedNumbers(0x00, getValueFromHexPairFromString("", 2));
}
/**
 * Test-function for the function "getSmscLengthFromPdu".
 */
void testGetSmscLengthFromPdu()
{
	compareUnsignedNumbers(0x1B, getSmscLengthFromPdu("1B"));
	compareUnsignedNumbers(0x00, getSmscLengthFromPdu("1"));
	compareUnsignedNumbers(0x07, getSmscLengthFromPdu("07911326040000F0040B911346610089F60000208062917314080CC8F71D14969741F977FD07"));
	compareUnsignedNumbers(0x00, getSmscLengthFromPdu(""));
}
/**
 * Test-function for the function "getSenderLengthFromPdu".
 */
void testGetSenderLengthFromPdu()
{
	compareUnsignedNumbers(0x00, getSenderLengthFromPdu("1B"));
	compareUnsignedNumbers(0x00, getSenderLengthFromPdu("1"));
	compareUnsignedNumbers(0x0B, getSenderLengthFromPdu("07911326040000F0040B911346610089F60000208062917314080CC8F71D14969741F977FD07"));
	compareUnsignedNumbers(0x00, getSenderLengthFromPdu(""));
}
/**
 * Test-function for the function "isValidPdu".
 */
void testIsValidPdu()
{
	compareFalse(isValidPdu("ABC"));
	compareTrue(isValidPdu("ABCDEF1234567890"));
	compareFalse(isValidPdu(" ABCDEF1234567890!"));
	compareFalse(isValidPdu(""));
}
/**
 * Test-function for the function "getCallerIdFromPdu".
 */
void testGetCallerIdFromPdu()
{
	compareUnsignedNumbers(85294281388ULL, getCallerIdFromPdu("07915892208800F0040B915892241883F800009921810170002B0341E211"));
	compareUnsignedNumbers(27838890001ULL, getCallerIdFromPdu("07917283010010F5040BC87238880900F10000993092516195800AE8329BFD4697D9EC37"));
	compareUnsignedNumbers(31641600986ULL, getCallerIdFromPdu("07911326040000F0040B911346610089F60000208062917314080CC8F71D14969741F977FD07"));
	compareUnsignedNumbers(31628870634ULL, getCallerIdFromPdu("07911326040011F5240B911326880736F40000111081017362401654747A0E4ACF41F4329E0E6A97E7F3F0B90C8A01"));
}
/**
 * Test-function for the function "convertInvalidCharactersFromMessage".
 */
void testConvertInvalidCharactersFromMessage()
{
	char x = PDU_INVALID_CHARACTER;
	char expected[] = {
	x, x, x, x, x, x, x, x,
	x, x, '\n', x, x, '\r', x, x,
	x, x, x, x, x, x, x, x,
	x, x, x, x, x, x, x, x,
	' ', '!', '"', '#', x, '%', '&', '\'',
	'(', ')', '*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ':', ';', '<', '=', '>', '?',
	x, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', x, x, x, x, x,
	x, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	'x', 'y', 'z', x, x, x, x, x,
	'\0' };
	uint8_t length = 128;
	char actual[length + 1];
	for(uint8_t i  = 0; i < length; i++)
	{
		actual[i] = i;
	}
	actual[length] = '\0';
	convertInvalidCharactersFromMessage(actual, length);
	compareStrings(expected, actual);
}
/**
 * Test-function for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu()
{
	testGetMessageFromPdu1();
	testGetMessageFromPdu2();
	testGetMessageFromPdu3();
	testGetMessageFromPdu4();
	testGetMessageFromPdu5();
	testGetMessageFromPdu6();
}
/**
 * Test-function #1 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu1()
{
	char actual[] = "07915892208800F0040B915892241883F800009921810170002B0341E211";
	char expected[] = "ADG";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
/**
 * Test-function #2 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu2()
{
	char actual[] = "07917283010010F5040BC87238880900F10000993092516195800AE8329BFD4697D9EC37";
	char expected[] = "hellohello";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
/**
 * Test-function #3 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu3()
{
	char actual[] = "07911326040000F0040B911346610089F60000208062917314080CC8F71D14969741F977FD07";
	char expected[] = "How are you?";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
/**
 * Test-function #4 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu4()
{
	char actual[] = "07916407058099F911000A8170607896200000A71554747A0E4ACF416110945805B5CBF379F85C06";
	char expected[] = "This is a PDU message";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
/**
 * Test-function #5 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu5()
{
	char actual[] = "07911326040011F5240B911326880736F40000111081017362401654747A0E4ACF41F4329E0E6A97E7F3F0B90C8A01";
	char expected[] = "This is text message 1";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
/**
 * Test-function #6 for the function "getMessageFromPdu".
 */
void testGetMessageFromPdu6()
{
	char actual[] = "07911326040011F5040B911356956826F0000031202291413140A031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E560";
	char expected[] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
	getMessageFromPdu(actual);
	compareStrings(expected, actual);
}
#endif
