modded class CAContinuousFill
{
	override void CalcAndSetQuantity( ActionData action_data )
	{
		if (action_data.m_MainItem && action_data.m_MainItem.IsTemperatureVisible() && m_liquid_type == LIQUID_WATER)
		{
			float temperature = action_data.m_MainItem.GetTemperature();
			if (temperature > TEMPERATURE_FILL_WATER_BOTTLE_TOTAL) action_data.m_MainItem.AddTemperature(m_SpentQuantity * TEMPERATURE_FILL_WATER_BOTTLE_MOD * -1);
			else action_data.m_MainItem.AddTemperature(m_SpentQuantity * TEMPERATURE_FILL_WATER_BOTTLE_MOD);
		}
		
		super.CalcAndSetQuantity( action_data );
	}
};