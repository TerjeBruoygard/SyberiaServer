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
	
	void SendToxicZonesToPlayer(ref PlayerIdentity identity)
	{
		ref array<ref ToxicZone> toxicZones = new array<ref ToxicZone>;
		if (m_config && m_config.m_customZones)
		{
			foreach (ref ZoneDefinition zone : m_config.m_customZones)
			{
				if (zone.m_gas > 0)
				{
					ref ToxicZone tz = new ToxicZone;
					tz.m_position = zone.m_position;
					tz.m_radius = zone.m_radius;
					tz.m_value = zone.m_gas;
					toxicZones.Insert(tz);
				}
			}
		}
		
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_SYNC_TOXIC_ZONES, identity, new Param1<ref array<ref ToxicZone>>( toxicZones ) );
		
		foreach (ref ToxicZone dtz : toxicZones)
		{
			delete dtz;
		}
		delete toxicZones;
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