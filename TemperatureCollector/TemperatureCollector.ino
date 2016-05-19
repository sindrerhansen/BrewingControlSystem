#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

static int ONE_WIRE_BUS = 2;
static char systemDevider = '_';
static char valueDevider = ':';
static String MyName = "TC";

bool Input_stringcomplete;


String input_String;
String AllInfoString;									// This is to define sensor  

#define MEGA_MASTER_ADDRESS 0x9
#define NANO_FLOW_TO_MASHTANK_ADDRESS 0x8
#define NANO_FLOW_TO_BOILTANK_ADDRESS 0x7
#define NANO_TEMPERATURECOLLECTOR_ADDRESS 0x6

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature TemperatureSensors(&oneWire);

void setup()
{
	//Setting up serial
	Serial.begin(9600);
	TemperatureSensors.begin();
	//Setting up I2C communikkasjon
	Wire.begin(NANO_TEMPERATURECOLLECTOR_ADDRESS);	//Change to sensor used
	Wire.onReceive(wireReceiveEvent);

}

void serialEvent() {
	while (Serial.available()) {
		char inChar = (char)Serial.read();
		input_String += inChar;
		if (inChar == '\n') {
			Input_stringcomplete = true;
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
	Input_stringcomplete = true;

}

void loop()
{
	// Getting Temperatures
	TemperatureSensors.requestTemperatures();
	AllInfoString = "";
	int numberOfSensors = TemperatureSensors.getDeviceCount();

	float HltTemperatureTank = TemperatureSensors.getTempCByIndex(6);
	AllInfoString += String(HltTemperatureTank) + valueDevider;

	float MashTankTemperatureTank = TemperatureSensors.getTempCByIndex(2);
	AllInfoString += String(MashTankTemperatureTank) + valueDevider;

	float MashTankTemperatureHeatingRetur = TemperatureSensors.getTempCByIndex(1);
	AllInfoString += String(MashTankTemperatureHeatingRetur) + valueDevider;

	float BoilTankTemperatureTank = TemperatureSensors.getTempCByIndex(4);
	AllInfoString += String(BoilTankTemperatureTank) + valueDevider;

	float ambientTemperature = TemperatureSensors.getTempCByIndex(3);
	AllInfoString += String(ambientTemperature) + valueDevider;

	float rimsRightOutesideTemperature = TemperatureSensors.getTempCByIndex(0);
	AllInfoString += String(rimsRightOutesideTemperature) + valueDevider;

	float rimsLeftOutesideTemperature = TemperatureSensors.getTempCByIndex(7);
	AllInfoString += String(rimsLeftOutesideTemperature) + valueDevider;

	float coolerReturnTemperature = TemperatureSensors.getTempCByIndex(5);
	AllInfoString += String(coolerReturnTemperature) + valueDevider;

	Serial.println(AllInfoString);
	//Serial.println(numberOfSensors);


	// Sending on I2C buss
	char charSendBuffer[20];
	String sendString = MyName + String(AllInfoString);
	sendString.toCharArray(charSendBuffer, 20);
	Wire.beginTransmission(MEGA_MASTER_ADDRESS);
	Wire.write(charSendBuffer);
	Wire.endTransmission();



	if (Input_stringcomplete)
	{
		input_String.trim();
		//  

		// Do your thing

		//

		input_String = "";
		Input_stringcomplete = false;
	}
}