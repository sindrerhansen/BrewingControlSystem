
#include <Wire.h>

volatile unsigned long  total_flow;
float totalLiter;
unsigned int  l_hour;							// Calculated liters/hour                      
static int flowmeter = 2;						// Flow Meter Pin number
unsigned long cloopTime;
String input_String;

bool input_StringComplete;


#define MASTER_ADDRESS 0x9
#define TEMPERATURE_COLLECTOR_ADDRESS 0x8
#define MASH_TANK_FLOW_IN_ADDRESS 0x7
#define BOIL_TANK_FLOW_IN_ADDRESS 0x6

static String MyName = String(MASH_TANK_FLOW_IN_ADDRESS);					// This is to define flow sensor  

void flow()										// Interrupt function
{
	total_flow++;
}


void setup()
{
	pinMode(flowmeter, INPUT);
	Serial.begin(38400);
	Wire.begin(MASH_TANK_FLOW_IN_ADDRESS);	//Change to sensor used
	Wire.onReceive(wireReceiveEvent);
	total_flow = 0;
	attachInterrupt(0, flow, RISING);			// Setup Interrupt 
	sei();										// Enable interrupts  
	cloopTime = millis();
}

void serialEvent(){
	while (Serial.available()) {
		char inChar = (char)Serial.read();
		input_String += inChar;
		if (inChar == '\n') {
			input_StringComplete = true;
		}
	}
}

void wireReceiveEvent(int howMany) {
	char charReadBuffer[20] = "";
	int charIndicator = 0;
	while (Wire.available() > 0) {
		char c = Wire.read();
		charReadBuffer[charIndicator] = c;
		charIndicator++;
	}
	String inStr(charReadBuffer);
	input_String = inStr;
	input_StringComplete = true;

}
void loop()
{
	if (millis() >= (cloopTime + 500))
	{
		cloopTime = millis();			     // Updates cloopTime
        totalLiter = total_flow / 444.4444444 ;
        Serial.println(totalLiter);		
		
		// Sending on I2C buss
		char charSendBuffer[20];
		String sendString = MyName + String(totalLiter);
		sendString.toCharArray(charSendBuffer, 20);
		Wire.beginTransmission(MASTER_ADDRESS);
		Wire.write(charSendBuffer);
		Wire.endTransmission();

	}

	if (input_StringComplete)
	{
		input_String.trim();
		
		if (input_String=="x")
		{
			total_flow = 0;
		}
		input_String = "";
		input_StringComplete = false;
	}
}
