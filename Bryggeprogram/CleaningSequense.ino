void CleaningSequense()
{
	switch (CleaningState)
	{
	case 0:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
		}

		// ideal state nothing is happening 

		break;

	case 10:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
		}

		Hlt.CirculationPump.Value = true;
		Hlt.TemperatureTankSetPoint = 50.0;
		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		break;

	case 20:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
		}

		Hlt.CirculationPump.Value = true;
		if (MashTank.CurentVolume < 20)
		{
			Hlt.TransferPump.Value = true;
		}
		else
		{
			MessageToUser = "Add cleaning chemicals to mash tank";
			if (messageConfirmd)
			{
				CleaningState = 30;
				messageConfirmd = false;
			}
		}
		if (MashTank.CurentVolume>5)
		{
			MashTank.CirculationPump.Value = true;
		}
		break;

	case 30:
		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;

		MashTank.CirculationPump.Value = true;
		MashTank.TransferPump.Value = true;
		if (BoilTank.CurentVolume >= 12)
		{
			CleaningState = 31;
		}

		break;

	case 31:
		MashTank.TemperatureTankSetPoint = 60;
		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		timeSpan = 10 * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);

		BoilTank.TransferPump.Value = true;

		if (remainingTime <= 0)
		{
			CleaningState = 40;
		}

		break;

	case 40:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;

		MashTank.CirculationPump.Value = true;
		BoilTank.TransferPump.Value = true;
		Hlt.TransferPump.Value = true;

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if (BoilTank.CurentVolume < 55)
		{
			MashTank.TransferPump.Value = true;
		}

		else if (MashTank.CurentVolume >= 10)
		{
			CleaningState = 50;
		}

		break;

	case 50:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
		}

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		MashTank.CirculationPump.Value = true;

		if (BoilTank.TemperatureTank > 96)
		{
			CleaningState = 51;
		}

		break;

	case 51:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		timeSpan = 15 * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		BoilTank.TransferPump.Value = true;

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if (remainingTime <= 0)
		{
			CleaningState = 52;
		}
		break;

	case 52:

		if (previouslyCleaningState != CleaningState)
		{
			previouslyCleaningState = CleaningState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		timeSpan = 15 * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		BoilTank.TransferPump.Value = true;

		if (remainingTime <= 0)
		{
			CleaningState = 0;
		}

		break;
	default:
		CleaningState = 0;
		break;
	}
}
