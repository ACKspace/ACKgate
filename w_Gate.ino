/**
 * This file contains the function
 * to setup the gate and to open the gate.
 * There is nothing needed to close the gate.
 * The gate will close automatically.
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
#warning Gate
/*
 * The text to be printed when the gate opens.
 */
#define TEXT_OPEN_GATE "Open the gate!"
/**
 * Prepare the gate for opening
 * by defining the outputpin
 * and setting it to LOW.
 */
void setupGate()
{
	pinMode(GATE_OUTPUT_PIN, OUTPUT);
	digitalWrite(GATE_OUTPUT_PIN, LOW);
}
/**
 * Open the gate by
 * setting the outputpin from LOW to HIGH,
 * wait for the specified delay
 * and then setting the pin to LOW again.
 */
void openGate()
{
	printString(TEXT_OPEN_GATE);
	digitalWrite(GATE_OUTPUT_PIN, HIGH);
	delay(GATE_OPEN_DELAY);
	digitalWrite(GATE_OUTPUT_PIN, LOW);
}
#endif
