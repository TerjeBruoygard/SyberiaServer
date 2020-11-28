class PluginSyberiaOptions extends PluginBase
{
	int m_startSoulsCount = 3;
	int m_respawnSoulsPrice = 1;	
	int m_newchar_points = 10;
	int m_roleplay_mode = 0;
	
	ref array<ref SpawnpointInfo> m_spawnpoints;
	ref array<string> m_defaultGearBody;
	ref array<string> m_defaultGearPants;
	ref array<string> m_defaultGearFoot;
	ref array<string> m_defaultGearHead;
	ref array<string> m_defaultGearWeapon;
	ref array<string> m_defaultGearItems;
	
	void PluginSyberiaOptions()
	{		
		string path = "$profile:PluginSyberiaOptions.json";
		
		if (FileExist(path))
		{
			JsonFileLoader<PluginSyberiaOptions>.JsonLoadFile(path, this);
		}
		else
		{
			m_spawnpoints = new array<ref SpawnpointInfo>;
			m_spawnpoints.Insert(new SpawnpointInfo("random", "0 0 0", -1));
			m_spawnpoints.Insert(new SpawnpointInfo("testpoint", "14500 0 1000", 100));
			
			m_defaultGearBody = { 
				"Hoodie_Blue",
				"Hoodie_Black",
				"Hoodie_Brown",
				"Hoodie_Green",
				"Hoodie_Grey",
				"Hoodie_Red"
			};
			
			m_defaultGearPants = { 
				"Jeans_Black",
				"Jeans_BlueDark",
				"Jeans_Blue",
				"Jeans_Brown",
				"Jeans_Green",
				"Jeans_Grey",
				"CanvasPants_Beige",
				"CanvasPants_Blue",
				"CanvasPants_Grey"
			};
			
			m_defaultGearFoot = {
				"AthleticShoes_Black",
				"AthleticShoes_Blue",
				"AthleticShoes_Brown",
				"AthleticShoes_Green",
				"AthleticShoes_Grey"
			};
			
			m_defaultGearHead = new array<string>;
			m_defaultGearWeapon = new array<string>;
			m_defaultGearItems = new array<string>;
			
			JsonFileLoader<PluginSyberiaOptions>.JsonSaveFile(path, this);
		}
	}
	
	void ~PluginSyberiaOptions()
	{
		foreach (ref SpawnpointInfo si : m_spawnpoints) delete si;
		delete m_spawnpoints;
		delete m_defaultGearBody;
		delete m_defaultGearPants;
		delete m_defaultGearFoot;
		delete m_defaultGearHead;
		delete m_defaultGearWeapon;
		delete m_defaultGearItems;
	}
};

PluginSyberiaOptions GetSyberiaOptions() 
{
    return PluginSyberiaOptions.Cast(GetPlugin(PluginSyberiaOptions));
}