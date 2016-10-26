/**
 * This file contains general callerId-functions.
 * It contains functions
 * to convert a callerId to a string
 * and to convert a string to a callerId.
 */

/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning CallerId
/*
 * Use an 8 bytes unsigned integer to store callerIds in.
 * This means that a callerId can be 2 ^ 64 - 1 at maximum.
 * This is enough to store 18-digit-callerIds.
 */
#define callerid_t uint64_t
/*
 * Maximum length of a callerId.
 * This will prevent a possible overflow
 * in the function "getCallerIdFromString".
 */
#define MAX_CALLER_ID_LENGTH 18
/**
 * Get the length of the string for the given callerId.
 * @param cid the callerId.
 * @return the length of the callerId.
 */
uint8_t getCallerIdLength(callerid_t cid)
{
	uint8_t length = 0;
	do
	{
		cid /= 10;
		length++;
	}
	while (cid);
	return length;
}
/**
 * Convert a callerId to a string.
 * @param cid the callerId to be converted.
 * @param str the string where
 * the converted callerId should be stored.
 */
void getStringFromCallerId(callerid_t cid, char* str)
{
	uint8_t length = getCallerIdLength(cid);
	if (length <= MAX_CALLER_ID_LENGTH)
	{
		callerid_t cidCopy;
		uint8_t i = 0;
		do
		{
			cidCopy = cid;
			cid /= 10;
			str[length - i - 1] = '0' + (cidCopy - (cid * 10));
			i++;
		}
		while(cid);
		str[i] = '\0';
	}
	else
	{
		str[0] = '\0';
	}
}
/**
 * Convert a string to a callerId.
 * @param str the string to be converted.
 * @return the callerId.
 */
callerid_t getCallerIdFromString(const char* str)
{
	callerid_t callerId = 0;
	boolean isValidCallerId = true;
	uint16_t length = strlen(str);
	if (length <= MAX_CALLER_ID_LENGTH)
	{
		for(uint16_t i = 0; (i < length) && isValidCallerId; i++)
		{
			if (isdigit(str[i]))
			{
				callerid_t b = str[i] - '0';
				for(uint16_t j = 0; j < (length - i - 1); j++)
				{
					b *= 10;
				}
				callerId += b;
			}
			else if ((i != 0) || (str[i] != '+'))
			{
				isValidCallerId = false;
				callerId = 0;
			}
		}
	}
	return callerId;
}
/**
 * Add the country calling code. When a call is made, 
 * some telephone-providers provide the callerId
 * without the country calling code,
 * so it needs to be added.
 * @param cid the callerId with or without
 * the country calling code.
 * @return the callerId with
 * the country calling code.
 */
callerid_t addCountryCallingCode(callerid_t cid)
{
	if (getCallerIdLength(cid) == CALLER_ID_WITHOUT_COUNTRY_CALLING_CODE_LENGTH)
	{
		callerid_t countryCallingCode = 1;
		for(int i = 0; i < CALLER_ID_WITHOUT_COUNTRY_CALLING_CODE_LENGTH; i++)
		{
			countryCallingCode *= 10;
		}
		countryCallingCode *= COUNTRY_CALLING_CODE;
		cid += countryCallingCode;
	}
	return cid;
}