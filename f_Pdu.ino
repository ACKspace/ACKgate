/**
 * This file contains PDU-functions
 * to get the callerId and the message
 * from a PDU (protocol discription unit).
 * PDU is a SMS represented as a hexadecimal string. 
 */

/*
 * Only compile this file
 * if the eeprom-functions are being tested
 * or the main-program with admin-option is being run.
 */
#if ((TEST == TEST_PDU) || ((!TEST) && (ADMIN)))
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Pdu
/*
 * Some characters which will be decoded
 * from a sms-message can't be displayed
 * using "normal" characters.
 * These characters will be replaced
 * with the character below.
 */
#define PDU_INVALID_CHARACTER '~'
/**
 * Determines if a character is
 * a hexadecimal digit.
 * @param c the character to be determined
 * to be a hexadecimal digit.
 * @return true is the given character
 * is a hexadecimal digit,
 * false if the given character
 * is not a hexadecimal digit.
 */ 
boolean isHexDigit(const char c)
{
	return (((c >= '0') &&(c <= '9')) ||
		((c >= 'A') && (c <= 'F')));
}
/**
 * Determines if a string is
 * a hexadecimal string.
 * @param str the string to be determined
 * to be a hexadecimal string.
 * @return true is the given string
 * is a hexadecimal string,
 * false if the given string
 * is not a hexadecimal string.
 */
boolean isHexString(const char* str)
{
	boolean isHexString = true;
	for(int i = 0; (str[i] != '\0') && isHexString; i++)
	{
		if (!isHexDigit(str[i]))
		{
			isHexString = false;
		}
	}
	return isHexString;
}
/**
 * Returns the value of a hexadecimal character.
 * @param the hexadecimal character
 * whose value will be returned.
 * @return the value of the given hexadecimal character
 * or 0 if not a valid hexadecimal character is specified.
 */
uint8_t getValueFromHexChar(const char c)
{
	uint8_t value = 0;
	if (isHexDigit(c))
	{
		if ((c >= '0') &&(c <= '9'))
		{
			value = c - '0';
		}
		else
		{
			value = 10 + (c - 'A');
		}
	}
	return value;
}
/**
 * Get the value of a pair of hexadecimal digits in a given string.
 * @param str the given string which contains the hexadecimal digits.
 * @param start the starting position of the hexadecimal pair.
 * @return the value of the hexadecimal pair or
 * 0 if there is illegal input.
 */
uint8_t getValueFromHexPairFromString(const char* str, const uint16_t start)
{
	uint8_t hex = 0;
	/*
	 * A string which has a length
	 * lesser than 2 can't contain a pair.
	 */
	if (strlen(str) >= 2)
	{
		/*
		 * Check if the starting position
		 * is correct regarding the length
		 * of the given string.
		 */
		if (start < (strlen(str) - 1))
		{
			/*
			 * Check that both characters
			 * are hexadecimal digits.
			 */
			if (isHexDigit(str[start]) && isHexDigit(str[start + 1]))
			{
				hex += getValueFromHexChar(str[start + 1]);
				hex += getValueFromHexChar(str[start]) * 16;
			}
		}
	}
	return hex;
}
/**
 * Returns the short message service center callerId-length
 * from a given pdu.
 * @param pdu the pdu from which the smsc callerId-length
 * has to be determined.
 * @return the smsc callerId-length from the given pdu. 
 */
uint8_t getSmscLengthFromPdu(const char* pdu)
{
	return getValueFromHexPairFromString(pdu, 0);
}
/**
 * Returns the sender callerId-length
 * from a given pdu.
 * @param pdu the pdu from which the sender callerId-length
 * has to be determined.
 * @return the sender callerId-length from the given pdu. 
 */
uint8_t getSenderLengthFromPdu(const char* pdu)
{
	uint16_t startPosition = 4 + (getSmscLengthFromPdu(pdu) * 2);
	return getValueFromHexPairFromString(pdu, startPosition);
}
/**
 * Determines if a given pdu is a valid pdu.
 * @param pdu the pdu determined to be valid or not.
 * @return true if the pdu is valid,
 * false if the pdu is not valid.
 */
boolean isValidPdu(const char* pdu)
{
	boolean isValid = true;
	/*
	 * Check for:
	 * - non-empty string
	 * - an even number of characters in the string
	     (each character is represent as 2 hexadecimal numbers)
	 * - that the string contains only hexadecimal characters
	     (0-9, A-F)	 
	 */
	if ((strlen(pdu) <= 0) || (strlen(pdu) % 2 != 0) || !isHexString(pdu))
	{
		isValid = false;
	}
	return isValid;
}
/**
 * Return the callerId of the sender from a given pdu.
 * @param pdu the pdu from which the callerId will be read.
 * @return the callerId of the given pdu
 * or 0 if an invalid pdu is given.
 */
callerid_t getCallerIdFromPdu(const char* pdu)
{
	callerid_t cid = 0;	
	if (isValidPdu(pdu))
	{
		/*
		 * The position in the pdu at which
		 * the callerId of the sender starts.
		 */
		uint16_t startPosition = 8 + (getSmscLengthFromPdu(pdu) * 2);
		/*
		 * The length of the callerId
		 * based on the given pdu.
		 */
		uint16_t callerIdLength = getSenderLengthFromPdu(pdu);
		/*
		 * Read the callerId from the pdu.
		 * The callerId "31457112345"
		 * is stored as "1354172143F5",
		 * so each pair has to be reversed.
		 * If the length of the callerId is odd,
		 * a character "F" is appended to it.
		 * The "F" must not be read.
		 */
		char callerId[callerIdLength + 1];
		int8_t invert = 1; 
		for(uint16_t i = 0; (i < callerIdLength) && (pdu[i] != '\0'); i++)
		{
			callerId[i] = pdu[startPosition + i + invert];
			invert *= -1;
		}
		/*
		 * Make the string null-terminated.
		 */
		callerId[callerIdLength] = '\0';
		/*
		 * Convert the callerId-string to a number.
		 */
		cid = getCallerIdFromString(callerId);
	}
	return cid;
}
/**
 * Convert invalid characters in a message
 * generated from a pdu to a special
 * defined invalid character.
 * @param message the message generated from a pdu.
 * @param length the length of the message
 * because it isn't null-terminated.
 */
void convertInvalidCharactersFromMessage(char* message, const uint16_t length)
{
	for(uint16_t i = 0; i < length; i++)
	{
		/*
		 * Only keep the characters which have
		 * the same value in both standard ASCII and
		 * GSM 03.38 (7 bits GSM alphabet). 
		 */
		if ((message[i] != 10) && (message[i] != 13) &&
		(!(
		((message[i] >= ' ') && (message[i] <= '#')) ||
		((message[i] >= '%') && (message[i] <= '?')) || 
		((message[i] >= 'A') && (message[i] <= 'Z')) ||
		((message[i] >= 'a') && (message[i] <= 'z')))	
		))
		{
			message[i] = PDU_INVALID_CHARACTER;
		}
	}
}
/**
 * Extract the message from pdu.
 * The message will start at the first position
 * of the given pdu, this is done to save memory.
 * @param pdu the pdu to extract the message from,
 * it will also be used for "returning" the message.
 */
void getMessageFromPdu(char* pdu)
{
	if (isValidPdu(pdu))
	{
		/*
		 * The position in the pdu at which
		 * the message starts.
		 */
		uint16_t startPosition = 28 + (getSmscLengthFromPdu(pdu) * 2);
		/*
		 * The length of the sender callerId.
		 */
		uint16_t senderLength = getSenderLengthFromPdu(pdu);
		/*
		 * If the sender callerId-length is odd,
		 * the callerId will be padded with an "F",
		 * so the callerId-length increases with 1.
		 */
		if (senderLength % 2 != 0)
		{
			senderLength++;
		}
		/*
		 * Increase the position in the pdu
		 * at which the message starts
		 * with the sender callerId-length.
		 */
		startPosition += senderLength;
		/*
		 * This will contain the length of the message.
		 */
		uint16_t p = 0;			
		/*
		 * Convert each 2 hexadecimal characters
		 * from the message to the value it represents
		 * and store those values at the beginning of the pdu.
		 */
		for(uint16_t i = startPosition, j = 0; pdu[i] != '\0'; i += 2)
		{
			p = ((i - startPosition) / 2) + j;
			pdu[p] = getValueFromHexPairFromString(pdu, i);
			/**
			 * After each 7th character,
			 * skip over one position.
			 * This is done because 8 characters
			 * are stored in 7 bytes.
			 * The current character in the position
			 * can be left there since it will be ignored
			 * when the real pdu-conversion takes place.
			 */
			if ((p % 8) == 6)
			{
				j++;
			}
		}
		p++;
		/*
		 * This will hold the bits
		 * coming from the previous character.
		 */
		uint8_t fromPreviousCharacter = 0;
		/*
		 * Convert the values from the previous step
		 * stored at the beginning of the pdu to the message.
		 */
		for(uint16_t i = 0; i < p; i++)
		{
			/*
			 * If this is not an 8th character.
			 */
			if ((i % 8) < 7)
			{
				/*
				 * These are the bits from the current character
				 * that will be transferred to the next character.
				 * It will the 1st most left bit for the 1st character,
				 * the 2 most left bits for the 2nd character and so forth.
				 * This is accomplished by inserting
				 * the necessarry 0s from the left,
				 * so that all that remains are the bits
				 * that need to be transferred.
				 */
				uint8_t fromCurrentCharacter = ((uint8_t) pdu[i]) >> (7 - (i % 8));
				/*
				 * Insert as many 0s from the right
				 * as there are bits coming over
				 * from the previous character.
				 * The +1 is there for the next line.
				 */
				pdu[i] = ((uint8_t) pdu[i]) << ((i % 8) + 1);
				/*
				 * Insert one 0 from the left
				 * to destroy the most left bit.
				 * This is done to get from 8 bits
				 * per byte to 7 bits per byte.
				 */
				pdu[i] = ((uint8_t) pdu[i]) >> 1;
				/*
				 * Insert the bits coming
				 * from the previous character
				 * into the next bit.
				 * This is done by an or-operation
				 * so that both of the "halfs" will fit
				 * into the new character.
				 */
				pdu[i] = ((uint8_t) pdu[i]) | fromPreviousCharacter;
				/*
				 * All operations are done,
				 * so the bits coming from the current character
				 * will be the bits coming from the previous character
				 * in the next round.
				 */
				fromPreviousCharacter = fromCurrentCharacter;
			}
			/*
			 * If this is an 8th character.
			 */
			else
			{
				/*
				 * The 8th character will contain
				 * only the 7 bits from the 7th character.
				 */
				pdu[i] = fromPreviousCharacter;
				/*
				 * No bits from the 8 character
				 * will be transferred
				 * to the next character.
				 */
				fromPreviousCharacter = 0;
			}
		}
		/*
		 * If there is anything left over
		 * from the last character,
		 * put it in the next character.
		 */
		if (fromPreviousCharacter > 0)
		{
			pdu[p] = fromPreviousCharacter;
			p++;
		}
		/*
		 * Make the string null-terminated.
		 */
		pdu[p] = '\0';
		/*
		 * Convert invalid characters in the message
		 * generated from a pdu to a special
		 * defined invalid character.
		 */
		convertInvalidCharactersFromMessage(pdu, p);
	}
}
#endif
