modded class BuildingLivespace
{
	House m_house;

	static BuildingLivespace SpawnLivespace(House house, int livespaceId)
	{
		string configPath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId + " classname";
		string classname = GetGame().ConfigGetTextOut(configPath);
		BuildingLivespace livespace = BuildingLivespace.Cast( GetGame().CreateObject(classname, house.GetPosition()) );
		SybLogSrv("Spawn livespace " + classname + " => " + livespace);
		livespace.SetPosition(house.GetPosition());
		livespace.SetYawPitchRoll(house.GetYawPitchRoll());
		livespace.Setup(house, livespaceId);
		return livespace;
	}
	
   	void Setup(House house, int livespaceId)
	{
		m_house = house;
		m_livespaceId = livespaceId;
		m_livespacePath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		
		if (!GetGame().ConfigIsExisting(m_livespacePath))
		{
			GetGame().ObjectDelete(this);
			return;
		}
		
		SetupDoors();
		SetupBarricades();
		m_ready = true;
		
		SychDirtyLevels();
	}
	
	override void SetupBarricades()
	{
		int barricadeId = 0;
		while ( GetGame().ConfigIsExisting(m_livespacePath + " Barricade" + barricadeId) )
		{
			m_barricadeLevels.Insert(1);
			barricadeId = barricadeId + 1;
		}
		
		super.SetupBarricades();
	}
	
	int GetBarricadeLevel(int id)
	{
		return m_barricadeLevels[id];
	}
	
	void SetBarricadeLevel(int id, int level)
	{
		int lastLevel = m_barricadeLevels[id];
		if (lastLevel != level)
		{
			m_barricadeLevels[id] = level;
			SychDirtyLevels();
		}
	}
	
	void UpgradeBarricade(int id)
	{
		int maxLevel = GetGame().ConfigGetInt(m_livespacePath + " Barricade" + id + " maxLevel");
		int level = GetBarricadeLevel(id) + 1;
		if (level <= maxLevel)
		{
			SetBarricadeLevel(id, level);
		}
	}
	
	override void SetupDoors()
	{
		int doorId = 0;
		while ( GetGame().ConfigIsExisting(m_livespacePath + " Door" + doorId) )
		{
			m_doorLevels.Insert(1);
			doorId = doorId + 1;
		}
		
		super.SetupDoors();
	}

	int GetDoorLevel(int id)
	{
		return m_doorLevels[id];
	}
	
	void SetDoorLevel(int id, int level)
	{
		int lastLevel = m_doorLevels[id];
		if (lastLevel != level)
		{
			m_doorLevels[id] = level;
			SychDirtyLevels();
		}
	}
	
	void UpgradeDoor(int id)
	{
		int maxLevel = GetGame().ConfigGetInt(m_livespacePath + " Door" + id + " maxLevel");
		int level = GetDoorLevel(id) + 1;
		if (level <= maxLevel)
		{
			SetDoorLevel(id, level);
		}
	}
	
	void OpenDoorLinked(int componentId)
	{
		SybLogSrv("OpenDoorLinked " + componentId);
	}
	
	void CloseDoorLinked(int componentId)
	{
		SybLogSrv("OpenDoorLinked " + componentId);
	}
	
	private void SychDirtyLevels(PlayerIdentity sender = NULL)
	{
		if (!m_ready) return;
			
		int houseNetId1, houseNetId2;
		m_house.GetNetworkID(houseNetId1, houseNetId2);
		
		Param6<string, int, int, int, ref array<int>, ref array<int>> params = 
			new Param6<string, int, int, int, ref array<int>, ref array<int>>(m_livespacePath, m_livespaceId, houseNetId1, houseNetId2, m_doorLevels, m_barricadeLevels);
		RPCSingleParam(SyberiaERPC.SYBERPC_SYNCH_LIVESPACE_SERVER, params, true, sender);
	}
	
	override House GetHouse()
	{
		return m_house;
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == SyberiaERPC.SYBERPC_SYNCH_LIVESPACE_CLIENT)
		{
			SychDirtyLevels(sender);
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
	}
};