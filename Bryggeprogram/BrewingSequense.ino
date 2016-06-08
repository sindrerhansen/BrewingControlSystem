void BrewingStateSequense()
{
	switch (BrewingState)
	{
	case 0:

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
		}

		// ideal state nothing is happening 

		break;

	case 10: // Prepar HLT tank temperature

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
		}
		Hlt.CirculationPump.Value = true;
		Hlt.TemperatureTankSetPoint = MashInn.HltTemperatureSP;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);
		if (startBrewing) {
			BrewingState = 20;
		}

		break;

	case 20: // Transfering water from HLT to Mash tank, waiting for grain

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
		}
		Hlt.CirculationPump.Value = true;

		MashTank.TemperatureTankSetPoint = MashInn.TemperatureSP;
		Hlt.TemperatureTankSetPoint = MashInn.HltTemperatureSP;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		if (MashTank.CurentVolume > MashCirculationStartTreshold)
		{
			MashTank.CirculationPump.Value = true;
			MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
			MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
		}
		if (MashTank.CurentVolume < MashInn.AddVolumeSP)
		{
			Hlt.TransferPump.Value = true;
		}


		if ((MashTank.CurentVolume >= MashInn.AddVolumeSP) && (MashTank.TemperatureTank >= MashTank.TemperatureTankSetPoint))
		{

			MessageToUser += "Add grain";
			if (messageConfirmd)
			{
				BrewingState = 30;
				messageConfirmd = false;
			}
		}

		break;

	case 30://Mash step 1 timer and temp regulator

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();    // start timer 
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep1.TemperatureSP;
		timeSpan = MashStep1.TimeMinutsSP * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		Hlt.CirculationPump.Value = true;
		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);
		MashTank.Element2.Value = MashTank.Element1.Value;
		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);

		if (remainingTime <= 0)
		{
			BrewingState = 31;
		}

		break;

	case 31: //Heating Mash to next setpoint (Step 2)

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = -elapsedTimeSeconds;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep2.TemperatureSP;
		Hlt.CirculationPump.Value = true;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);


		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);

		if (MashTank.TemperatureTank >= MashTank.TemperatureTankSetPoint)
		{
			BrewingState = 32;
		}

		break;

	case 32://Mash step 2 timer and temp regulator

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep2.TemperatureSP;
		timeSpan = MashStep2.TimeMinutsSP * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		Hlt.CirculationPump.Value = true;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.CirculationPump.Value = true;

		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);

		if (remainingTime <= 0)
		{
			BrewingState = 33;
		}
		previouslyBrewingState = BrewingState;
		break;

	case 33://Heating Mash to next setpoint (Step 3)

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = -elapsedTimeSeconds;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep3.TemperatureSP;

		Hlt.CirculationPump.Value = true;
		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);

		if (MashTank.TemperatureTank >= MashTank.TemperatureTankSetPoint)
		{
			BrewingState = 34;
		}

		break;

	case 34://Mash step 3 timer and temp regulator

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep3.TemperatureSP;
		timeSpan = MashStep3.TimeMinutsSP * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		Hlt.CirculationPump.Value = true;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);

		if (remainingTime <= 0)
		{
			BrewingState = 35;
		}

		break;

	case 35: //Heating Mash to next setpoint (Step 4)

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();

		}
		previouslyBrewingState = BrewingState;

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = -elapsedTimeSeconds;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep4.TemperatureSP;

		Hlt.CirculationPump.Value = true;
		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.8, RimsRightOuteSideTemp);

		if (MashTank.TemperatureTank >= MashTank.TemperatureTankSetPoint)
		{
			BrewingState = 36;
		}

		break;

	case 36: //Mash step 4 timer and temp regulator

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = MashStep4.TemperatureSP;
		timeSpan = MashStep4.TimeMinutsSP * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		Hlt.CirculationPump.Value = true;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.CirculationPump.Value = true;
		MashTank.Element1.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);
		MashTank.Element2.Value = RIMS_PWM_ReelayRegulator(MashTank.TemperatureTankSetPoint, MashTank.TemperatureTank, MashTank.TemperatureHeatingRetur, 0.6, RimsRightOuteSideTemp);

		if (remainingTime <= 0)
		{
			BrewingState = 40;
		}

		break;

	case 40: //Pre sparge transfer
		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
			lastTime = 0;
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = Sparge.TemperatureSP;
		remainingTime = -elapsedTimeSeconds;

		Hlt.CirculationPump.Value = true;
		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);

		MashTank.TransferPump.Value = true;

		if (BoilTank.CurentVolume > (MashInn.AddVolumeSP * 0.5))
		{
			BrewingState = 41;
		}

		if ((elapsedTimeSeconds - lastTime) >= 5)
		{
			lastTime = elapsedTimeSeconds;
			transferRate = (BoilTank.AddedVolume - lastBoilVolume) / 5;
			lastBoilVolume = BoilTank.AddedVolume;
			if (transferRate < 0.01)
			{
				BrewingState = 41;
			}
		}

		break;

	case 41: // Sparge

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
			lastTime = 0;
			lastBoilVolume = BoilTank.CurentVolume;
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		Hlt.TemperatureTankSetPoint = Sparge.HltTemperatureSP;
		MashTank.TemperatureTankSetPoint = Sparge.TemperatureSP;
		remainingTime = -elapsedTimeSeconds;

		Hlt.Element1.Value = TankTemperaturOnOffRegulator(Hlt.TemperatureTankSetPoint, Hlt.TemperatureTank, Hlt.LevelOverHeatingElements.State);
		MashTank.TransferPump.Value = true;
		if (MashTank.AddedVolume <(MashInn.AddVolumeSP + Sparge.AddVolumeSP))
		{
			Hlt.TransferPump.Value = true;
		}
		else
		{
			SpargeIsDone = true;
		}

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if ((elapsedTimeSeconds - lastTime) >= 5)
		{
			lastTime = elapsedTimeSeconds;
			transferRate = (BoilTank.CurentVolume - lastBoilVolume) / 5;
			lastBoilVolume = BoilTank.CurentVolume;
			if ((transferRate < 0.01) && SpargeIsDone)
			{
				BrewingState = 42;
			}
		}

		break;

	case 42: //Waiting to drain more
		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
			lastTime = 0;
			lastBoilVolume = BoilTank.CurentVolume;
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = mashDrainOffTime - elapsedTimeSeconds;

		if (elapsedTimeSeconds > mashDrainOffTime)
		{
			MashTank.TransferPump.Value = true;
		}

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if (elapsedTimeSeconds >= (mashDrainOffTime + 3))
		{
			if ((elapsedTimeSeconds - lastTime) >= 3)
			{
				lastTime = elapsedTimeSeconds;
				transferRate = (BoilTank.AddedVolume - lastBoilVolume) / 3;
				lastBoilVolume = BoilTank.AddedVolume;
				if (transferRate < 0.01)
				{
					BrewingState = 43;
				}
			}
		}

		break;

	case 43: //Waiting to drain more
		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
			lastTime = 0;
			lastBoilVolume = BoilTank.CurentVolume;
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = mashDrainOffTime - elapsedTimeSeconds;

		if (elapsedTimeSeconds > mashDrainOffTime)
		{
			MashTank.TransferPump.Value = true;
		}

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if (elapsedTimeSeconds >= (mashDrainOffTime + 3))
		{
			if ((elapsedTimeSeconds - lastTime) >= 3)
			{
				lastTime = elapsedTimeSeconds;
				transferRate = (BoilTank.AddedVolume - lastBoilVolume) / 3;
				lastBoilVolume = BoilTank.AddedVolume;
				if (transferRate < 0.01)
				{
					BrewingState = 50;
				}

			}
		}

		break;

	case 50://Pre boil getting up to boil temp
		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		remainingTime = -elapsedTimeSeconds;
		Boil.TemperatureSP = BoilTempThreshold;
		BoilTank.TemperatureTankSetPoint = Boil.TemperatureSP;

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			BoilTank.Element2.Value = true;
		}

		if (BoilTank.TemperatureTank>Boil.TemperatureSP)
		{
			BrewingState = 51;
		}

		break;

	case 51:

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		elapsedTimeSeconds = (millis() - refTime) / 1000;
		elapsedTimeMinutes = elapsedTimeSeconds / 60;
		timeSpan = Boil.TimeMinutsSP * 60;
		remainingTime = timeSpan - elapsedTimeSeconds;

		if (BoilTank.LevelOverHeatingElements.State)
		{
			BoilTank.Element1.Value = true;
			if (BoilTank.TemperatureTank<BoilTank.TemperatureTankSetPoint - 0.2)
			{
				BoilTank.Element2.Value = true;
			}

		}

		if (remainingTime <= 600)
		{
			BoilTank.TransferPump.Value = true;
			if (BoilTank.LevelOverHeatingElements.State)
			{
				BoilTank.Element2.Value = true;
			}

		}

		if (remainingTime <= 0)
		{
			refTime = millis();
			BrewingState = 52;
		}

		break;
	case 52:

		if (previouslyBrewingState != BrewingState)
		{
			previouslyBrewingState = BrewingState;
			refTime = millis();
		}

		BoilTank.TransferPump.Value = true;

		break;
	default:
		BrewingState = 0;
		break;
	}
}
