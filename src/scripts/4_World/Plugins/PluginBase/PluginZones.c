class PluginZones extends PluginBase
{
	ref PluginZones_Config m_config;
	int m_uniqueId = 1;
	
	override void OnInit()
	{
		MakeDirectory("$profile:Syberia");

		string path = "$profile:Syberia\\ZonesConfig.json";
		if (FileExist(path))
		{
			m_config = new PluginZones_Config;
			JsonFileLoader<ref PluginZones_Config>.JsonLoadFile(path, m_config);
			if (m_config && m_config.m_customZones)
			{
				int i = 0;
				while (i < m_config.m_customZones.Count())
				{
					ref ZoneDefinition zone = m_config.m_customZones.Get(i);
					if (zone.m_id >= m_uniqueId)
					{
						m_uniqueId = zone.m_id + 1;
					}
					
					if (zone.m_spawnChance == 1.0 || Math.RandomFloat01() < zone.m_spawnChance)
					{
						i = i + 1;			
					}
					else
					{
						m_config.m_customZones.Remove(i);						
					}
				}				
			}
		}
	}
	
	int AddObjectiveZone(EntityAI object)
	{
		if (m_config && m_config.m_customZones)
		{
			ref ZoneDefinition zone = new ZoneDefinition();
			zone.m_id = m_uniqueId;
			zone.m_position = object.GetPosition();
			zone.m_radius = object.ConfigGetFloat("radius");
			zone.m_height = object.ConfigGetFloat("height");
			zone.m_enterMessage = object.ConfigGetString("enterMessage");
			zone.m_leaveMessage = object.ConfigGetString("leaveMessage");
			zone.m_leaveTime = object.ConfigGetFloat("leaveTime");
			zone.m_godMode = object.ConfigGetInt("godMode");
			zone.m_inverseDammage = object.ConfigGetInt("inverseDammage");
			zone.m_blockBuildingMode = object.ConfigGetInt("blockBuildingMode");
			zone.m_blockInteractionWithPlayers = object.ConfigGetInt("blockInteractionWithPlayers");
			zone.m_radiation = object.ConfigGetFloat("radiation");
			zone.m_psi = object.ConfigGetFloat("psi");
			zone.m_gas = object.ConfigGetFloat("gas");
			zone.m_spawnChance = 1.0;
			
			m_uniqueId = m_uniqueId + 1;
			m_config.m_customZones.Insert(zone);
			
			if (zone.m_gas > 0)
			{
				SendToxicZonesToAll();
			}
			
			return zone.m_id;
		}
		else
		{
			return -1;
		}
	}
	
	void DeleteObjectiveZone(EntityAI object, int id)
	{
		if (m_config && m_config.m_customZones && id > 0)
		{
			for (int i = 0; i < m_config.m_customZones.Count(); i++)
			{
				ref ZoneDefinition zone = m_config.m_customZones.Get(i);
				if (zone.m_id == id)
				{
					m_config.m_customZones.Remove(i);

					if (zone.m_gas > 0)
					{
						SendToxicZonesToAll();
					}
					
					delete zone;
					return;
				}
			}
		}
	}
	
	private void PrepareToxicZonesToSynch(ref array<ref ToxicZone> toxicZones)
	{
		if (m_config && m_config.m_customZones)
		{
			foreach (ref ZoneDefinition zone : m_config.m_customZones)
			{
				if (zone.m_gas > 0)
				{
					ref ToxicZone tz = new ToxicZone;
					tz.m_position = zone.m_position;
					tz.m_radius = zone.m_radius;
					toxicZones.Insert(tz);
				}
			}
		}
	}
	
	void SendToxicZonesToPlayer(PlayerIdentity identity)
	{
		ref array<ref ToxicZone> toxicZones = new array<ref ToxicZone>;
		PrepareToxicZonesToSynch(toxicZones);
		GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_SYNC_TOXIC_ZONES, identity, new Param1<ref array<ref ToxicZone>>( toxicZones ) );
		
		foreach (ref ToxicZone dtz : toxicZones)
		{
			delete dtz;
		}
		delete toxicZones;
	}
	
	void SendToxicZonesToAll()
	{
		ref array<ref ToxicZone> toxicZones = new array<ref ToxicZone>;
		PrepareToxicZonesToSynch(toxicZones);
		GetSyberiaRPC().SendToAll( SyberiaRPC.SYBRPC_SYNC_TOXIC_ZONES, new Param1<ref array<ref ToxicZone>>( toxicZones ) );
		
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