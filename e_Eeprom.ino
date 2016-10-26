/**
 * This file contains functions to write, read and delete
 * callerIds from eeprom.
 * The callerIds are stored randomly in eeprom
 * to create a basic wear-leveling algorithm.
 */

/*
 * Only compile this file
 * if the eeprom-functions are being tested
 * or the main-program is being run.
 */
#if ((TEST == TEST_EEPROM) || (!TEST))
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Eeprom
/*
 * Needed for eeprom-storage.
 */
#include <EEPROM.h>
/*
 * The type which is used
 * to read and write data to eeprom.
 */
#define eeprom_t uint8_t
/*
 * The size of the array which is used
 * to read and write eeprom. 8 / 1 = 8. 
 */
#define EEPROM_ARRAY_SIZE sizeof(callerid_t) / sizeof(eeprom_t)
/*
 * The size of the eeprom-type in bits.
 * This is used by the bitshifting-process
 * used by reading and writing to eeprom.
 */
#define EEPROM_SIZE_IN_BITS sizeof(eeprom_t) * 8
/*
 * The maximum number of callerIds
 * that can be stored in eeprom.
 */
#define EEPROM_MAX_POSITION (E2END + 1) / EEPROM_ARRAY_SIZE
/**
 * Determines if a given callerId is a valid callerId.
 * 0 (for not having a callerId) and ~0 (for unwritten eeprom)
 * are invalid callerIds.
 * @param cid the callerId to be checked
 * @return true if it is a valid callerId,
 * false if it is not a valid callerId.
 */
boolean isValidCallerId(const callerid_t cid)
{
	return (cid != 0) && (cid != (~(callerid_t) 0));
}
/**
 * Store a given callerId in eeprom.
 * @param cid the callerId to be stored.
 * @return true if the callerId is successfully stored,
 * false if the callerId is invalid,
 * false if the callerId is already stored,
 * false if there is no eeprom available.
 */
boolean storeCallerIdInEeprom(const callerid_t cid)
{
	boolean stored = false;
	if (isValidCallerId(cid) && !isCallerIdInEeprom(cid))
	{
		uint16_t pos = getAvailableEepromPosition();
		if (isValidEepromPosition(pos))
		{
			stored = writeCallerIdToEeprom(pos, cid);
		}
	}
	return stored; 
}
/**
 * Determines if a callerId is stored in eeprom.
 * @return true if the callerId is stored in eeprom,
 * false if the callerId is invalid,
 * false if the callerId is not stored in eeprom.
 */
boolean isCallerIdInEeprom(const callerid_t cid)
{
	boolean found = false;
	if (isValidCallerId(cid))
	{
		for(uint16_t pos = 0; (pos < EEPROM_MAX_POSITION) && !found; pos++)
		{
			if (readCallerIdFromEeprom(pos) == cid)
			{
				found = true;
			}
		}
	}
	return found;
}
/**
 * Remove a given callerId from eeprom.
 * @param cid the callerId to be removed.
 * @return true if the callerId is successfully removed,
 * false if the callerId is invalid,
 * false if the callerId was not stored in eeprom.
 */
boolean removeCallerIdFromEeprom(const callerid_t cid)
{
	boolean removed = false;
	if (isValidCallerId(cid) && isCallerIdInEeprom(cid))
	{
		for(uint16_t pos = 0; pos < EEPROM_MAX_POSITION; pos++)
		{
			if (readCallerIdFromEeprom(pos) == cid)
			{
				/*
				 * Fill the current eepromPosition with 1's
				 * because that is the default state for
				 * uninitialised eeprom.
				 */
				removed = writeCallerIdToEeprom(pos, ~0);
			}
		}
	} 
	return removed;
}
/**
 * Returns how much callerIds are stored in eeprom.
 * @return the number of callerIds stored in eeprom.
 */
uint16_t getNumberOfCallerIdsInEeprom()
{
	uint16_t numberOfCallerIds = 0;
	for(uint16_t pos = 0; pos < EEPROM_MAX_POSITION; pos++)
	{
		if (isValidCallerId(readCallerIdFromEeprom(pos)))
		{
			numberOfCallerIds++;
		}
	}
	return numberOfCallerIds;
}
/**
 * Print all callerIds in eeprom to serial.
 */
void showAllCallerIdsInEeprom()
{
	for(uint16_t pos = 0; pos < EEPROM_MAX_POSITION; pos++)
	{
		callerid_t cid = readCallerIdFromEeprom(pos);
		if (isValidCallerId(cid))
		{
			printCallerId(cid);
		}
	}
}
/**
 * Clears eeprom.
 * This function is kept simple
 * and independant from other functions
 * on purpose.
 */
void clearEeprom()
{
	for(uint16_t i = 0; i <= E2END; i++)
	{
		/*
		 * Write ~0 (0xFF) to eeprom
		 * because that is the default value.
		 */
		EEPROM.write(i, ~0);
	}
}
/**
 * Determines if an eepromPosition is a valid one.
 * @param eepromPosition the eepromPosition.
 * @return true if it is a valid eepromPosition, false otherwise.
 */
boolean isValidEepromPosition(const uint16_t eepromPosition)
{
	return (eepromPosition >= 0) && (eepromPosition < EEPROM_MAX_POSITION);
}
/**
 * Read a callerId from eeprom.
 * @param eepromPosition the position at which the callerId begins.
 * @return the callerId at the given eepromPosition.
 */
callerid_t readCallerIdFromEeprom(const uint16_t eepromPosition)
{
	callerid_t cid = 0;
	if (isValidEepromPosition(eepromPosition))
	{
		/*
		 * Read the bytes located on eepromPosition
		 * which represent the callerId and put these into an array.
		 */
		eeprom_t eepromArray[EEPROM_ARRAY_SIZE];
		uint16_t beginEepromAddress = eepromPosition * EEPROM_ARRAY_SIZE;
		for(uint16_t i = 0; i < EEPROM_ARRAY_SIZE; i++)	
		{
			eepromArray[i] = EEPROM.read(beginEepromAddress + i);
		}
		/*
		 * Convert the array to a callerId using bitshifting.
		 */
		for(uint16_t i = 0; i < EEPROM_ARRAY_SIZE; i++)
		{
			cid = cid | ((callerid_t) eepromArray[i]) << (EEPROM_SIZE_IN_BITS * i);
		}
	}
	return cid;
}
/**
 * Write a callerId to eeprom.
 * @param eepromPosition the position at which the callerId begins.
 * @param cid the callerId to be written.
 * @return true if the writing succeeded, false otherwise.
 */
boolean writeCallerIdToEeprom(const uint16_t eepromPosition, const callerid_t cid)
{
	boolean written = false;
	if (isValidEepromPosition(eepromPosition))
	{
		/*
		 * Convert the callerId to an array using bitshitfing.
		 */
		eeprom_t eepromArray[EEPROM_ARRAY_SIZE]; 
		for(uint16_t i = 0; i < EEPROM_ARRAY_SIZE; i++)
		{
			eepromArray[i] = (eeprom_t) (cid >> (EEPROM_SIZE_IN_BITS * i));
		}
		/*
		 * Write the array to eeprom.
		 */
		uint16_t beginEepromAddress = eepromPosition * EEPROM_ARRAY_SIZE;
		for(uint16_t i = 0; i < EEPROM_ARRAY_SIZE; i++)
		{
			/*
			 * Only write to the eeprom if the value which is to be written
			 * is different from the value which is already there.
			 */
			if (EEPROM.read(beginEepromAddress + i) != eepromArray[i])
			{
				EEPROM.write(beginEepromAddress + i, eepromArray[i]);
			}
		}
		written = true;
	}
	return written;
}
/**
 * Get a random available eepromPosition.
 * @return an available eepromPosition
 * or EEPROM_MAX_POSITION if there is
 * no eepromPosition available.
 */
uint16_t getAvailableEepromPosition()
{
	uint16_t pos = getAvailableEepromPosition(random(0, EEPROM_MAX_POSITION));
	if (!isValidEepromPosition(pos))
	{
		pos = getAvailableEepromPosition(0);
	}
	return pos; 
}
/**
 * Get the first eepromPosition available
 * starting from startEepromPosition.
 * @param startEepromPosition the eepromPosition
 * which will be the start for the search
 * for an available eepromPosition.
 * @return an available eepromPosition
 * or EEPROM_MAX_POSITION if there is
 * no eepromPosition available.
 */
uint16_t getAvailableEepromPosition(const uint16_t startEepromPosition)
{
	uint16_t pos = EEPROM_MAX_POSITION;
	if (isValidEepromPosition(startEepromPosition))
	{
		for(uint16_t p = startEepromPosition; (p < EEPROM_MAX_POSITION) && !isValidEepromPosition(pos); p++)
		{
			if (!isValidCallerId(readCallerIdFromEeprom(p)))
			{
				pos = p;
			}
		}
	}
	return pos;
}
#endif
