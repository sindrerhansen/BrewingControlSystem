void ParsingStringFromBoilFlowIn(String innString)
{
	innString.trim();
	String _totalVolume = innString.substring(0, 6);
	_totalVolume.trim();
	float _volume = _totalVolume.toFloat();
	if (abs(lastTotVolumeBoil - _volume) < 0.5)
	{
		BoilTank.AddedVolume = _volume;
		BoilTank.CurentVolume = BoilTank.AddedVolume;
	}
	lastTotVolumeBoil = _volume;

}
