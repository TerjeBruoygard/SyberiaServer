class PluginZones extends PluginBase
{
	ref PluginZones_Config m_config;
	
	override void OnInit()
	{
		MakeDirectory("$profile:Syberia");

		string path = "$profile:Syberia\\ZonesConfig.json";
		if (FileExist(path))
		{
			m_config = new PluginZones_Config;
			JsonFileLoader<ref PluginZones_Config>.JsonLoadFile(path, m_config);
		}
	}
	
	void ~PluginZones()
	{
		if (m_config)
			delete m_config;
	}
};

class PluginZones_Config
{
	ref ZoneDefinition m_defaultZone;
	
	ref array<ref ZoneDefinition> m_customZones;
	
	void ~PluginZones_Config()
	{
		if (m_defaultZone)
		{
			delete m_defaultZone;
		}
		
		if (m_customZones)
		{
			foreach (ref ZoneDefinition zone : m_customZones)
			{
				delete zone;
			}
			
			delete m_customZones;
		}
	}
};