void ParsingStringSerial3()
{
	if (input_3_StringComplete)
	{
		input_3_String.trim();
		String _totalVolume = input_3_String.substring(0, 6);
		_totalVolume.trim();
		float _volume = _totalVolume.toFloat();
		if (abs(lastTotVolumeBoil - _volume) < 0.5)
		{
			BoilTank.AddedVolume = _volume;
			BoilTank.CurentVolume = BoilTank.AddedVolume;
		}
		lastTotVolumeBoil = _volume;
		input_3_String = "";
		input_3_StringComplete = false;

	}
}
