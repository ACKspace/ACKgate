/**
 * A temporary solution which adds callerIds
 * that are allowed to open the gate to eeprom.
 * This is needed as long
 * as the admin-functionality isn't fully completed.
 */

/*
 * Only compile this file
 * when the mainprogram is being run.
 */
#if (!TEST)
/*
 * Generate a file specific warning during compilation,
 * so that it is possible to identify
 * which files are being compiled. 
 */
#warning MainTemp
/**
 * If admin-functionality isn't enabled,
 * clear the eeprom and
 * add certain known numbers "by hand".
 */
void addCallerIds()
{
}
#endif
