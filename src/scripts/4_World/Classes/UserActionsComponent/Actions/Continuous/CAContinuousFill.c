modded class CAContinuousFill
{
	override void CalcAndSetQuantity( ActionData action_data )
	{
		if (action_data.m_MainItem && action_data.m_MainItem.IsTemperatureVisible() && m_liquid_type == LIQUID_WATER)
		{
			float temperature = action_data.m_MainItem.GetTemperature();
			if (temperature > GetSyberiaConfig().m_temperatureFillWaterBottleTotal) action_data.m_MainItem.AddTemperature(m_SpentQuantity * GetSyberiaConfig().m_temperatureFillWaterBottleMod * -1);
			else action_data.m_MainItem.AddTemperature(m_SpentQuantity * GetSyberiaConfig().m_temperatureFillWaterBottleMod);
		}
		
		super.CalcAndSetQuantity( action_data );
	}
};