bool PWM_Reelay(double setpoint, double actual, double ratio)
{
	bool output = false;
	Tc = millis();
	double PWD_Window = 5000;

	if (actual < setpoint)
	{
		if (ratio > 0)
		{
			if (ratio < 1)
			{
				if (Ts + PWD_Window*ratio > Tc)
				{
					output = true;
				}
				else
				{
					output = false;
				}
				if (Ts + PWD_Window < Tc)
				{
					Ts = Tc;
				}
			}
			else
			{
				output = true;
			}
		}
		else
		{
			output = false;
		}
	}

	else
	{
		output = false;
	}
	if (actual < (setpoint - 3.0))
	{
		output = true;
	}

	return output;
}
