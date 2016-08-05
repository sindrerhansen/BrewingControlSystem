bool Tank_PWM_ReelayRegulator(double setpoint, double actual, double ratio, bool overElement)
{
	bool output;

	if (overElement)
	{
		output = PWM_Reelay(setpoint, actual, ratio);
	}
	else
	{

		output = false;
	}

	return output;
}
