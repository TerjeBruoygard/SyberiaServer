modded class SyberiaMedicineBase
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		string itemClassname = GetType();
		int medPainkillerLevel = GetGame().ConfigGetInt( "CfgVehicles " + itemClassname + " medPainkillerLevel" );
		if (medPainkillerLevel > 0)
		{
			if (consumer.m_painkillerEffect <= medPainkillerLevel)
			{
				float medPainkillerTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + itemClassname + " medPainkillerTimeSec" );
				consumer.m_painkillerEffect = medPainkillerLevel;
				consumer.m_painkillerTime = consumer.m_painkillerTime + medPainkillerTimeSec;
			}
		}
	}
};