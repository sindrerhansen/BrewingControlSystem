void I2C_ReceiveEvent(int i)
{
	char charReadBuffer[100] = "";
	int charIndicator = 0;
	while (Wire.available() > 0) {
		char c = Wire.read();
		charReadBuffer[charIndicator] = c;
		charIndicator++;
	}
	String receivedString(charReadBuffer);
	if (receivedString.startsWith(String(TEMPERATURE_COLLECTOR_ADDRESS)))
	{
		
		ParsingStringFromTemperaturCollector(receivedString);
	}

	else if (receivedString.startsWith(String(MASH_TANK_FLOW_IN_ADDRESS)))
	{
		receivedString.remove(0, 1);
		ParsingStringFromMashFlowIn(receivedString);
	}

	else if (receivedString.startsWith(String(BOIL_TANK_FLOW_IN_ADDRESS)))
	{
		receivedString.remove(0, 1);
		ParsingStringFromBoilFlowIn(receivedString);
	}
}
