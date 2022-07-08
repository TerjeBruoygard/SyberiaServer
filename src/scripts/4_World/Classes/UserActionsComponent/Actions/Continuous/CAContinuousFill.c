modded class CAContinuousFill
{
	override void CalcAndSetQuantity( ActionData action_data )
	{
		if (action_data.m_MainItem && action_data.m_MainItem.IsTemperatureVisible() && m_liquid_type == LIQUID_WATER)
		{
			vector pos = action_data.m_Target.GetCursorHitPos();
			string surface_type = "";
			GetGame().SurfaceGetType(pos[0], pos[2], surface_type);
			if (surface_type == "nam_snow" && (!action_data.m_Target.GetObject() || !action_data.m_Target.GetObject().IsWell()))
			{
				Mission mission = GetGame().GetMission();
				if (mission && mission.GetWorldData())
				{
					action_data.m_MainItem.SetTemperature( mission.GetWorldData().GetBaseEnvTemperature() );
				}
				
				PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
				if (plugin)
				{
					plugin.TransmitAgents(NULL, action_data.m_MainItem, AGT_WATER_POND, 1);
				}
			}
			else
			{
				float temperature = action_data.m_MainItem.GetTemperature();
				if (temperature > GetSyberiaConfig().m_temperatureFillWaterBottleTotal) action_data.m_MainItem.AddTemperature(m_SpentQuantity * GetSyberiaConfig().m_temperatureFillWaterBottleMod * -1);
				else action_data.m_MainItem.AddTemperature(m_SpentQuantity * GetSyberiaConfig().m_temperatureFillWaterBottleMod);
			}
		}
		
		super.CalcAndSetQuantity( action_data );
	}
};