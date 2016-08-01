void ParsingStringSerial0()
{
	if (input_0_StringComplete)
	{
		input_0_String.trim();
		int index = 0;
		int deviderIndex = 0;

		if (input_0_String.startsWith("CMD"))
		{
			input_0_String.remove(0, 3);
			int CMD = input_0_String.toInt();
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

		else if (input_0_String.startsWith("CONFIRMED"))
		{
			messageConfirmd = true;
		}

		else if (input_0_String.startsWith("STA"))
		{
			input_0_String.remove(0, 3);
			int STA = input_0_String.toInt();
			BrewingState = STA;
		}

		else if (input_0_String.startsWith("CSTA"))
		{
			input_0_String.remove(0, 4);
			int CSTA = input_0_String.toInt();
			CleaningState = CSTA;
		}

		else if (input_0_String.startsWith("SET"))
		{
			input_0_String.remove(0, 3);
			for (unsigned int i = 0; i <= input_0_String.length(); i++)
			{
				if (systemDevider == input_0_String.charAt(i))
				{
					resivedItems[index] = input_0_String.substring(deviderIndex, i);
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

		else if (input_0_String.startsWith("OVERRIDE"))
		{
			input_0_String.remove(0, 8);
			input_0_String.trim();
			int _overrideCMD = input_0_String.toInt();
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

		else if (input_0_String.startsWith("FLOW")) //String FLOW_
		{
			input_0_String.remove(0, 5);
			if (input_0_String.startsWith("RES"))
			{
				Serial1.println("x");
			}
			else if (input_0_String.startsWith("REB"))
			{
				Serial3.println("x");
			}
			else if (input_0_String.startsWith("SetMesh"))
			{

			}
			else if (input_0_String.startsWith("SetBoil"))
			{

			}
		}

		else if (input_0_String.startsWith("PREPCLEAN"))
		{
			if (BrewingState == 0)
			{
				CleaningState = 10;
			}
		}
		else if (input_0_String.startsWith("CLEAN"))
		{
			if (BrewingState == 0)
			{
				CleaningState = 20;
			}
		}
		else if (input_0_String.startsWith("RESETSYSTEM"))
		{
			Serial1.println("x");
			Serial3.println("x");
		}

		input_0_String = "";                                                       //clear the string:
		input_0_StringComplete = false;                                            //reset the flag used to tell if we have received a completed string from the PC
	}
}
