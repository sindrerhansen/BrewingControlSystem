
void ParsingStringFromMashFlowIn(String innString)
{
	innString.trim();
	String _totalVolume = innString.substring(0, 6);
	_totalVolume.trim();
	float _volume = _totalVolume.toFloat();
	if (abs(lastTotVolumeHLT - _volume) < 0.5)
	{
		MashTank.AddedVolume = _volume;
		MashTank.CurentVolume = MashTank.AddedVolume - BoilTank.AddedVolume;
	}
	lastTotVolumeHLT = _volume;

	
}