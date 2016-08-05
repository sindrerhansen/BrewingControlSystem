bool RIMS_PWM_ReelayRegulator(double setpoint, double tempInn, double tempOut, double ratio, double RIMS_outesideTemp)
{
	bool output = false;

	if (tempOut < (tempInn + 9.0) && tempOut < (setpoint + 5.0) && RIMS_outesideTemp < tempInn && RIMS_outesideTemp < setpoint)
	{
		output = PWM_Reelay(setpoint, tempInn, 0.9);
	}
	else
	{
		output = false;
	}

	return output;
}
