/**
 *
 */

/*
 * Only compile this file
 * when the mainprogram is being run
 * and administrator-functionality is enabled.
 */
#if ((!TEST) && (ADMIN))
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Admin
/**
 * The list of callerIds which are
 * permitted administrator privileges.
 */
#define ADMIN_CALLER_IDS { 31612345678ULL }

#define COMMAND_SEPERATOR "."
#define COMMAND_ADD_CALLER_ID 'a'
#define COMMAND_DELETE_CALLER_ID 'd'
#define COMMAND_LIST_EEPROM 'l'
/**
 * Determines if the caller with the given callerId
 * has administrator privileges.
 * @param cid the callerId to be determined
 * to has administrator privileges or not.
 * @return true if the callerId has administrator privileges,
 * false if the callerId has not administrator privileges.
 */
boolean isAdministrator(const callerid_t cid)
{
	boolean isAdminstrator = false;
	callerid_t adminNumbers[] = ADMIN_CALLER_IDS;
	uint8_t length = sizeof(adminNumbers) / sizeof(callerid_t);
	for(uint8_t i = 0; (i < length) && !isAdminstrator; i++)
	{
		if (adminNumbers[i] == cid)
		{
			isAdminstrator = true;
		}
	}
	return isAdminstrator;
}

boolean processAdministratorCommands(const callerid_t cid, const char* command)
{
	boolean isAdmin = isAdministrator(cid);
	if (isAdmin)
	{
		processCommand(command);
	}
	return isAdmin;
}

void processCommand(const char* command)
{
	if (command[0] == COMMAND_LIST_EEPROM)
	{
		showAllCallerIdsInEeprom();
	}
	else if (command[0] == COMMAND_ADD_CALLER_ID)
	{
		command++;
		storeCallerIdInEeprom(addCountryCallingCode(getCallerIdFromString(command)));
		showAllCallerIdsInEeprom();
	}
	else if (command[0] == COMMAND_DELETE_CALLER_ID)
	{
		command++;
		removeCallerIdFromEeprom(addCountryCallingCode(getCallerIdFromString(command)));
		showAllCallerIdsInEeprom();
	}
}
#endif
