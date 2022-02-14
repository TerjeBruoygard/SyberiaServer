modded class SyberiaObjectiveZone
{
	int m_pluginZoneId = -1;
	
	override void EEInit()
	{
		super.EEInit();
		
		PluginZones pluginZones = PluginZones.Cast(GetPlugin(PluginZones));
		if (pluginZones)
		{
			m_pluginZoneId = pluginZones.AddObjectiveZone(this);
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		PluginZones pluginZones = PluginZones.Cast(GetPlugin(PluginZones));
		if (pluginZones && m_pluginZoneId > 0)
		{
			pluginZones.DeleteObjectiveZone(this, m_pluginZoneId);
			m_pluginZoneId = -1;
		}
	}
};