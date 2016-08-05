void ParsingStringFromBrewer(String innString)
{

	innString.trim();
	int index = 0;
	int deviderIndex = 0;

	if (innString.startsWith("CMD"))
	{
		innString.remove(0, 3);
		int CMD = innString.toInt();
		if (CMD == 0)
		{
			BrewingState = 0;
			CleaningState = 0;
			startBrewing = false;
		}

		else if ((CMD == 10) && (BrewingState < 10))
		{
			Serial.println("CMD = 10");
			BrewingState = 10;
		}
		else if ((CMD == 20) && (BrewingState<20))
		{
			startBrewing = true;
			BrewingState = 20;
		}
	}

	else if (innString.startsWith("CONFIRMED"))
	{
		messageConfirmd = true;
	}

	else if (innString.startsWith("STA"))
	{
		innString.remove(0, 3);
		int STA = innString.toInt();
		BrewingState = STA;
	}

	else if (innString.startsWith("CSTA"))
	{
		innString.remove(0, 4);
		int CSTA = innString.toInt();
		CleaningState = CSTA;
	}

	else if (innString.startsWith("SET"))
	{
		innString.remove(0, 3);
		for (unsigned int i = 0; i <= innString.length(); i++)
		{
			if (systemDevider == innString.charAt(i))
			{
				resivedItems[index] = innString.substring(deviderIndex, i);
				index++;
				deviderIndex = i + 1;

			}

		}

		MashInn.TemperatureSP = resivedItems[0].toFloat();					//"MITe"
		MashInn.HltTemperatureSP = resivedItems[1].toFloat();				//"MIHT"
		MashInn.AddVolumeSP = resivedItems[2].toFloat();					//"MIVo"
		MashStep1.TemperatureSP = resivedItems[3].toFloat();				//"M1Te"
		MashStep1.TimeMinutsSP = resivedItems[4].toFloat();					//"M1Ti"
		MashStep2.TemperatureSP = resivedItems[5].toFloat();				//"M2Te"
		MashStep2.TimeMinutsSP = resivedItems[6].toFloat();					//"M2Ti"
		MashStep3.TemperatureSP = resivedItems[7].toFloat();				//"M3Te"
		MashStep3.TimeMinutsSP = resivedItems[8].toFloat();					//"M3Ti"
		Sparge.TemperatureSP = resivedItems[9].toFloat();					//"SpTe"
		Sparge.AddVolumeSP = resivedItems[10].toFloat();					//"SpVo"
		Boil.TimeMinutsSP = resivedItems[11].toFloat();						//"BoTi"

		Sparge.HltTemperatureSP = Sparge.TemperatureSP;

		AllInfoString += "ConSe";
		AllInfoString += String(MashInn.TemperatureSP) + valueDevider;
		AllInfoString += String(MashInn.HltTemperatureSP) + valueDevider;
		AllInfoString += String(MashInn.AddVolumeSP) + valueDevider;
		AllInfoString += String(MashStep1.TemperatureSP) + valueDevider;
		AllInfoString += String(MashStep1.TimeMinutsSP) + valueDevider;
		AllInfoString += String(MashStep2.TemperatureSP) + valueDevider;
		AllInfoString += String(MashStep2.TimeMinutsSP) + valueDevider;
		AllInfoString += String(MashStep3.TemperatureSP) + valueDevider;
		AllInfoString += String(MashStep3.TimeMinutsSP) + valueDevider;
		AllInfoString += String(Sparge.TemperatureSP) + valueDevider;
		AllInfoString += String(Sparge.AddVolumeSP) + valueDevider;
		AllInfoString += String(Boil.TimeMinutsSP) + valueDevider;
		AllInfoString += systemDevider;
		//	Serial.println(sendMessage);
	}

	else if (innString.startsWith("OVERRIDE"))
	{
		innString.remove(0, 8);
		innString.trim();
		int _overrideCMD = innString.toInt();
		if (_overrideCMD<10)
		{

		}
		else if (_overrideCMD >= 10 && _overrideCMD<20)
		{

		}

		else if (_overrideCMD >= 20 && _overrideCMD<30)
		{

		}

		else if (_overrideCMD >= 30 && _overrideCMD<40)
		{

		}
	}

	else if (innString.startsWith("FLOW")) //String FLOW_
	{
		innString.remove(0, 5);
		if (innString.startsWith("RES"))
		{
			Serial1.println("x");
		}
		else if (innString.startsWith("REB"))
		{
			Serial3.println("x");
		}
		else if (innString.startsWith("SetMesh"))
		{

		}
		else if (innString.startsWith("SetBoil"))
		{

		}
	}

	else if (innString.startsWith("PREPCLEAN"))
	{
		if (BrewingState == 0)
		{
			CleaningState = 10;
		}
	}
	else if (innString.startsWith("CLEAN"))
	{
		if (BrewingState == 0)
		{
			CleaningState = 20;
		}
	}
	else if (innString.startsWith("RESETSYSTEM"))
	{
		Serial1.println("x");
		Serial3.println("x");
	}

}
