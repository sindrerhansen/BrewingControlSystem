
void ParsingStringSerial1()
{
	if (input_1_StringComplete)
	{
		input_1_String.trim();
		String _totalVolume = input_1_String.substring(0, 6);
		_totalVolume.trim();
		float _volume = _totalVolume.toFloat();
		if (abs(lastTotVolumeHLT - _volume) < 0.5)
		{
			MashTank.AddedVolume = _volume;
			MashTank.CurentVolume = MashTank.AddedVolume - BoilTank.AddedVolume;
		}
		lastTotVolumeHLT = _volume;
		input_1_String = "";
		input_1_StringComplete = false;
	}
}