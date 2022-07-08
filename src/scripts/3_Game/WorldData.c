modded class WorldData
{
	float m_sybTimer;
	float m_airCurrentTemperature;
	
	override void Init()
	{
		super.Init();
		
		m_airCurrentTemperature = GetSyberiaConfig().m_airDayTemperatureMin;
		m_sybTimer = 0;
		
		if (IsDayTime())
		{
			m_airCurrentTemperature = Math.Lerp(GetSyberiaConfig().m_airDayTemperatureMin, GetSyberiaConfig().m_airDayTemperatureMax, Math.RandomFloat01());
		}
		else
		{
			m_airCurrentTemperature = Math.Lerp(GetSyberiaConfig().m_airNightTemperatureMin, GetSyberiaConfig().m_airNightTemperatureMax, Math.RandomFloat01());
		}
	}
	
	bool IsDayTime()
	{
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		float curTime = hour + ( minute / 60.0 );
		float monthday = month + ( day / 32.0 );
		float approxSunrise = GetApproxSunriseTime( monthday );
		float approxSunset = GetApproxSunsetTime( monthday );
		return ( curTime >= approxSunrise ) && ( curTime <= approxSunset );
	}
	
	override void UpdateBaseEnvTemperature(float timeslice)
	{
		super.UpdateBaseEnvTemperature(timeslice);
		
		m_sybTimer = m_sybTimer + timeslice;
		if (m_sybTimer > 30)
		{
			m_sybTimer = 0;
			
			float randShift = Math.RandomFloat01() - 0.5;
			if ( IsDayTime() ) {
				// day
				m_airCurrentTemperature = Math.Clamp(m_airCurrentTemperature + randShift, GetSyberiaConfig().m_airDayTemperatureMin, GetSyberiaConfig().m_airDayTemperatureMax);
			}
			else {
				// night
				m_airCurrentTemperature = Math.Clamp(m_airCurrentTemperature + randShift, GetSyberiaConfig().m_airNightTemperatureMin, GetSyberiaConfig().m_airNightTemperatureMax);
			}
		}
	}
	
	override float GetBaseEnvTemperature()
	{
		return m_airCurrentTemperature;
	}
};