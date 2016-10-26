/**
 * The main-program which runs the setup
 * for all the needed devices
 * (serial-connection, eeprom,
 * gate, serial-connection)
 * and then runs a loop which processes
 * the data from the telephone.
 */

/*
 * Only compile this
 * when the mainprogram is being run.
 */
#if (!TEST)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning Main
/**
 * Setup all the devices:
 * - serial-connection
 * - eeprom
 * - gate
 * - telephone-connection.
 */
void setup()
{
	setupSerial();
	//#if ADMIN
	addCallerIds();
	//#endif
	showAllCallerIdsInEeprom();
	setupGate();
	setupTelephone();
}
/**
 * Enter an endless loop which processes
 * the data from the telephone.
 */
void loop()
{
	loopTelephone();
}
/**
 * Called when there is an incoming call.
 * @param cid the callerId from the incoming call.
 */
void incomingCall(callerid_t cid)
{
	/*
	 * If the callerId from the incoming call
	 * is stored in eeprom, open the gate.
	 */
	if (isCallerIdInEeprom(cid))
	{
		openGate();
	}
}
/*
 * Only enable the sms specific functionality
 * if the administration-interface is enabled.
 */
#if ADMIN
/**
 * Called when there is an incoming message.
 * @param cid the callerId from the incoming message.
 * @param message the text from the incoming message.
 */
void incommingMessage(callerid_t cid, char* message)
{
	processAdministratorCommands(cid, message);
}
#endif
#endif
