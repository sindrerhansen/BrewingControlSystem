bool TankTemperaturOnOffRegulator(double setpoint, double actual, bool overElement)
{
	bool output;
	if (overElement)
	{
		if (actual <= setpoint)
		{
			output = true;
		}
		else
		{
			output = false;
		}
	}

	else
	{
		output = false;
		//MessageToUser = "Add water to hot liquor tank!!";
	}

	return output;
}
