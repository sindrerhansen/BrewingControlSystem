// This code is for a Arduino Mega. By Sindre
//#include "Regulators.h"
#include "Serial.h"

//Define Variables
double Setpoint, Input, Output;
unsigned long Ts;
unsigned long Tc;

int WindowSize = 5000;
unsigned long windowStartTime;
bool ElementOnOff = false;
String element = "";
unsigned long now = millis();
unsigned long TotalEnergyUsed = 0;
unsigned int HartConter = 0;


#pragma region Constants

const long prePumpeTimeSparge = 20;
const int MashCirculationStartTreshold = 10;
const float BoilTempThreshold = 97.0;
const int SerialSendingRate = 500; // Milliseconds

#pragma endregion Constants

#pragma region Structs
struct Sequence
{
	float AddVolumeSP;
	float HltTemperatureSP;
	float TemperatureSP;
	float TimeMinutsSP;
};

struct TankInfo
{
	struct Pump
	{
		int OutputPin;
		bool Value;
		bool ValueOverride;
		bool OverrideEnable;
		bool ValueOut;
	};
	struct HeatingElement
	{
		int OutputPin;
		bool Value;
		bool ValueOverride;
		bool OverrideEnable;
		bool ValueOut;
	};
	struct Valve
	{
		int OutputPin;
		bool Value;
		bool ValueOverride;
		bool OverrideEnable;
		bool ValueOut;
	};
	struct LevelSwitch
	{
		int InputPin;
		bool State;
	};
	float AddedVolume;
	float CurentVolume;
	float TemperatureTank;
	float TemperatureTankSetPoint;
	float TemperatureHeatingRetur;

	HeatingElement Element1;
	HeatingElement Element2;
	Pump CirculationPump;
	Pump TransferPump;
	Valve DrainValve;
	Valve InnValve;
	LevelSwitch LevelHigh;
	LevelSwitch LevelOverHeatingElements;
};
#pragma endregion Structs

#pragma region Declering Tanks
TankInfo Hlt;
TankInfo MashTank;
TankInfo BoilTank;
TankInfo AllTanks[3];
#pragma endregion Declering Tanks

#pragma region Declering Sequences
Sequence MashInn;
Sequence MashStep1;
Sequence MashStep2;
Sequence MashStep3;
Sequence MashStep4;
Sequence Sparge;
bool SpargeIsDone = false;
Sequence Boil;
#pragma endregion Declering Sequences

#pragma region Declaring Variables
float ambientTemperature = 0;
float RimsRightOuteSideTemp = 0;
float RimsLeftOuteSideTemp = 0;
long refTime = 0;
long refTime2 = 0;
long elapsedTimeMinutes = 0;
long elapsedTimeSeconds = 0;
long lastTime = 0;
long timeSpan = 0;
long mashDrainOffTime = 400;
long remainingTime = 0;
unsigned long timez = 0;
unsigned long cloopTime;
float lastTotVolumeHLT = 0;
float lastTotVolumeBoil = 0;
float totalAddedVolume = 0;
float lastBoilVolume=0;
float transferRate = 0;
int BrewingState = 0;
int previouslyBrewingState = 0;
bool startBrewing = false;
bool StartCleaning = false;
bool messageConfirmd = false;
bool Cleaning = false;
int CleaningState = 0;
int previouslyCleaningState = 0;
String test = "";
String test2 = "";

String input_0_String = "";
boolean input_0_StringComplete = false;

String input_1_String = "";
boolean input_1_StringComplete = false;

String input_2_String = "";
boolean input_2_StringComplete = false;

String input_3_String = "";
boolean input_3_StringComplete = false;

static char systemDevider = '_';
static char valueDevider = ':';

String resivedItems[20];
String AllInfoString = "";
String MessageToUser = "";



#pragma endregion Declaring Variables

void setup() {
	
	Init();

}

void serialEvent(){
	while (Serial.available()) {
		char inChar = (char)Serial.read();
		input_0_String += inChar;
		if (inChar == '\n') {
			input_0_StringComplete = true;
		}
	}
}

void serialEvent1(){
	while (Serial1.available()) {
		char inChar = (char)Serial1.read();
		input_1_String += inChar;
		if (inChar == '\n') {
			input_1_StringComplete = true;
		}
	}
}

void serialEvent2(){
	while (Serial2.available()) {
		char inChar = (char)Serial2.read();
		input_2_String += inChar;
		if (inChar == '\n') {
			input_2_StringComplete = true;
		}
	}
}

void serialEvent3(){
	while (Serial3.available()) {
		char inChar = (char)Serial3.read();
		input_3_String += inChar;
		if (inChar == '\n') {
			input_3_StringComplete = true;
		}
	}
}

void loop() {
#pragma region Resetting outputs 
 	Hlt.Element1.Value = false;
	Hlt.Element2.Value = false;
	Hlt.CirculationPump.Value = false;
	Hlt.TransferPump.Value = false;
	Hlt.InnValve.Value = false;

	MashTank.Element1.Value = false;
	MashTank.Element2.Value = false;
	MashTank.CirculationPump.Value = false;
	MashTank.TransferPump.Value = false;
	MashTank.InnValve.Value = false;

	BoilTank.Element1.Value = false;
	BoilTank.Element2.Value = false;
	BoilTank.CirculationPump.Value = false;
	BoilTank.TransferPump.Value = false;
	BoilTank.InnValve.Value = false;

	timeSpan = 0;
	remainingTime = 0;
	SpargeIsDone = false;
	totalAddedVolume = MashInn.AddVolumeSP + MashStep1.AddVolumeSP + MashStep2.AddVolumeSP + MashStep3.AddVolumeSP + MashStep4.AddVolumeSP + Sparge.AddVolumeSP;

	MessageToUser = "";
#pragma endregion Resetting outputs
	// From User interface
	ParsingStringSerial0();

	// From Mash Tank flow counter
	ParsingStringSerial1();

	// From temperature collector
	ParsingStringSerial2();

	// From Boil Tank flow counter
	ParsingStringSerial3();

#pragma region Reading Digital Sensors

	BoilTank.LevelOverHeatingElements.State = !digitalRead(BoilTank.LevelOverHeatingElements.InputPin);
	Hlt.LevelOverHeatingElements.State = !digitalRead(Hlt.LevelOverHeatingElements.InputPin);

#pragma endregion Reading Digital Sensors

	if (BrewingState==0 && CleaningState!=0)
	{
		Cleaning = true;
	}

	else if (BrewingState != 0 && CleaningState == 0)
	{
		Cleaning = false;
	}

	else if (BrewingState == 0 && CleaningState == 0)
	{
		Cleaning = false;
	}


	if (!Cleaning)
	{ 
		BrewingStateSequense();
	}

	else
	{		
		CleaningSequense();
	}


#pragma region Setting_Outputs 

	AllTanks[1] = Hlt;
	AllTanks[2] = MashTank;
	AllTanks[3]= BoilTank;

	for (int tank = 1; tank <= (sizeof(AllTanks) / sizeof(TankInfo)); tank++)
	{
		if (AllTanks[tank].Element1.OverrideEnable){ AllTanks[tank].Element1.ValueOut = AllTanks[tank].Element1.ValueOverride; }
		else{ AllTanks[tank].Element1.ValueOut = AllTanks[tank].Element1.Value; }
		digitalWrite(AllTanks[tank].Element1.OutputPin, AllTanks[tank].Element1.ValueOut);

		if (AllTanks[tank].Element2.OverrideEnable){ AllTanks[tank].Element2.ValueOut = AllTanks[tank].Element2.ValueOverride; }
		else{ AllTanks[tank].Element2.ValueOut = AllTanks[tank].Element2.Value; }
		digitalWrite(AllTanks[tank].Element2.OutputPin, AllTanks[tank].Element2.ValueOut);

		if (AllTanks[tank].CirculationPump.OverrideEnable){ AllTanks[tank].CirculationPump.ValueOut = AllTanks[tank].CirculationPump.ValueOverride; }
		else{ AllTanks[tank].CirculationPump.ValueOut = AllTanks[tank].CirculationPump.Value; }
		digitalWrite(AllTanks[tank].CirculationPump.OutputPin, !AllTanks[tank].CirculationPump.ValueOut);

		if (AllTanks[tank].TransferPump.OverrideEnable){ AllTanks[tank].TransferPump.ValueOut = AllTanks[tank].TransferPump.ValueOverride; }
		else{ AllTanks[tank].TransferPump.ValueOut = AllTanks[tank].TransferPump.Value; }
		digitalWrite(AllTanks[tank].TransferPump.OutputPin, !AllTanks[tank].TransferPump.Value);

		if (AllTanks[tank].DrainValve.OverrideEnable){ AllTanks[tank].DrainValve.ValueOut = AllTanks[tank].DrainValve.ValueOverride; }
		else{ AllTanks[tank].DrainValve.ValueOut = AllTanks[tank].DrainValve.Value; }
		digitalWrite(AllTanks[tank].DrainValve.OutputPin, AllTanks[tank].DrainValve.ValueOut);

		//if (AllTanks[tank].InnValve.OverrideEnable){ AllTanks[tank].InnValve.ValueOut = AllTanks[tank].InnValve.ValueOverride; }
		//else { AllTanks[tank].InnValve.ValueOut = AllTanks[tank].InnValve.Value; }
		//digitalWrite(AllTanks[tank].InnValve.OutputPin, AllTanks[tank].InnValve.ValueOut);
	}
#pragma endregion Setting_Outputs

#pragma region SendingMessageToSerial
	if (millis() >= (cloopTime + SerialSendingRate))
	{

		if (HartConter==32000)
		{
			HartConter = 0;
		}
		else
		{
			HartConter++;
		}
		cloopTime = millis();			     // Updates cloopTime

		AllInfoString += "HltTe" + String(Hlt.TemperatureTank) + systemDevider;
		AllInfoString += "MatTe" + String(MashTank.TemperatureTank) + systemDevider;
		AllInfoString += "MarTe" + String(MashTank.TemperatureHeatingRetur) + systemDevider;
		AllInfoString += "BotTe" + String(BoilTank.TemperatureTank) + systemDevider;
		AllInfoString += "BoCoT" + String(BoilTank.TemperatureHeatingRetur) + systemDevider;
		AllInfoString += "AmbTe" + String(ambientTemperature) + systemDevider;
		
		AllInfoString += "STATE" + String(BrewingState) + systemDevider;
		AllInfoString += "Messa" + MessageToUser + systemDevider;;

		AllInfoString += "HltSp" + String(Hlt.TemperatureTankSetPoint) + systemDevider;
		AllInfoString += "HltE1" + String(Hlt.Element1.Value) + systemDevider;
		AllInfoString += "HltCp" + String(Hlt.CirculationPump.Value) + systemDevider;
		AllInfoString += "HltTp" + String(Hlt.TransferPump.Value) + systemDevider;

		AllInfoString += "MatSp" + String(MashTank.TemperatureTankSetPoint) + systemDevider;
		AllInfoString += "MatE1" + String(MashTank.Element1.Value) + systemDevider;
		AllInfoString += "MatE2" + String(MashTank.Element2.Value) + systemDevider;
		AllInfoString += "MatCp" + String(MashTank.CirculationPump.Value) + systemDevider;
		AllInfoString += "MatTp" + String(MashTank.TransferPump.Value) + systemDevider;
		AllInfoString += "MatVo" + String(MashTank.CurentVolume) + systemDevider;
		AllInfoString += "MatAV" + String(MashTank.AddedVolume) + systemDevider;
		AllInfoString += "RimRO" + String(RimsRightOuteSideTemp) + systemDevider;
		AllInfoString += "RimLO" + String(RimsLeftOuteSideTemp) + systemDevider;

		AllInfoString += "BotSp" + String(BoilTank.TemperatureTankSetPoint) + systemDevider;
		AllInfoString += "BotE1" + String(BoilTank.Element1.Value) + systemDevider;
		AllInfoString += "BotE2" + String(BoilTank.Element2.Value) + systemDevider;
		AllInfoString += "BotCp" + String(BoilTank.CirculationPump.Value) + systemDevider;
		AllInfoString += "BotTp" + String(BoilTank.TransferPump.Value) + systemDevider;
		AllInfoString += "BotVo" + String(BoilTank.CurentVolume) + systemDevider;

		AllInfoString += "TimSp" + String(timeSpan) + systemDevider;
		AllInfoString += "Timer" + String(elapsedTimeSeconds) + systemDevider;
		AllInfoString += "RemTi" + String(remainingTime) + systemDevider;
		AllInfoString += "CleSt" + String(CleaningState) + systemDevider;
		AllInfoString += "HartConter" + String(HartConter) + systemDevider;

		Serial.println(AllInfoString);
		AllInfoString = "";

	}
#pragma endregion SendingMessageToSerial

	delay(10);
}