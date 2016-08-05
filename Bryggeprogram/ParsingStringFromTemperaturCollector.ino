void ParsingStringFromTemperaturCollector(String innString)
{
	int valueStartIndex = 0;
	int conter = 0;
	String _resiveArray[8];
	
	innString.trim();
	for (int i = 0; i <= innString.length(); i++)
	{
		if (valueDevider == innString.charAt(i))
		{
			_resiveArray[conter] = innString.substring(valueStartIndex, i);
			valueStartIndex = i + 1;
			conter++;
		}
	}
	if (_resiveArray[0].toFloat() >= 0)
	{
		Hlt.TemperatureTank = _resiveArray[0].toFloat();
	}

	if (_resiveArray[1].toFloat() >= 0)
	{
		MashTank.TemperatureTank = _resiveArray[1].toFloat();
	}

	if (_resiveArray[2].toFloat() >= 0)
	{
		MashTank.TemperatureHeatingRetur = _resiveArray[2].toFloat();
	}

	if (_resiveArray[3].toFloat() >= 0)
	{
		BoilTank.TemperatureTank = _resiveArray[3].toFloat();
	}

	if (_resiveArray[4].toFloat() >= 0)
	{
		ambientTemperature = _resiveArray[4].toFloat();
	}
	if (_resiveArray[5].toFloat() >= 0)
	{
		RimsRightOuteSideTemp = _resiveArray[5].toFloat();
	}
	if (_resiveArray[6].toFloat() >= 0)
	{
		RimsLeftOuteSideTemp = _resiveArray[6].toFloat();
	}

	if (_resiveArray[7].toFloat() >= 0)
	{
		BoilTank.TemperatureHeatingRetur = _resiveArray[7].toFloat();
	}
}
