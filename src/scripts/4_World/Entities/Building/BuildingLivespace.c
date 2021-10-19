modded class BuildingLivespace
{
	private House m_house;

	static BuildingLivespace SpawnLivespace(House house, int livespaceId)
	{
		string configPath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		string classname = GetGame().ConfigGetTextOut(configPath + " classname");
		vector offset = GetGame().ConfigGetVector(configPath + " offset");
		vector position = house.ModelToWorld(offset);
		
		BuildingLivespace livespace = BuildingLivespace.Cast( GetGame().CreateObject(classname, position) );
		SybLogSrv("Spawn livespace " + classname + " => " + livespace);
			
		livespace.SetPosition(position);
		livespace.SetYawPitchRoll(house.GetYawPitchRoll());
		livespace.Setup(house, livespaceId);
		return livespace;
	}
	
   	void Setup(House house, int livespaceId)
	{
		m_house = house;
		m_livespaceId = livespaceId;
		m_livespacePath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		m_data = new LivespaceData(m_livespacePath);
		
		SetupDoors();
		SetupBarricades();
		m_ready = true;
		
		SychDirtyLevels();
	}
	
	override void SetupBarricades()
	{
		for (int i = 0; i < m_data.m_barricades.Count(); i++)
		{
			m_barricadeLevels.Insert(1);
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
		int maxLevel = m_data.m_barricades.Get(id).m_levels.Count();
		int level = GetBarricadeLevel(id) + 1;
		if (level <= maxLevel)
		{
			SetBarricadeLevel(id, level);
		}
	}
	
	override void SetupDoors()
	{
		for (int i = 0; i < m_data.m_doors.Count(); i++)
		{
			m_doorLevels.Insert(1);
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
		int maxLevel = m_data.m_doors.Get(id).m_levels.Count();
		int level = GetDoorLevel(id) + 1;
		if (level <= maxLevel)
		{
			SetDoorLevel(id, level);
		}
	}
	
	ref LivespaceDoorData FindDoorByComponent(int componentId)
	{
		foreach (ref LivespaceDoorData doorData : m_data.m_doors)
		{
			foreach (string compName : doorData.m_components)
			{
				if (IsActionComponentPartOfSelection(componentId, compName))
				{					
					return doorData;
				}
			}			
		}
		
		return null;
	}
	
	void OpenDoorLinked(int componentId)
	{
		ref LivespaceDoorData doorData = FindDoorByComponent(componentId);
		if (doorData != null)
		{
			foreach (int linkedDoorId : doorData.m_linkedDoorIds)
			{
				GetHouse().OpenDoor(linkedDoorId);
			}
			
			foreach (string component : doorData.m_components)
			{
				if ( m_actionComponents.Contains(component) )
				{
					int doorIndex = GetDoorIndex( m_actionComponents.Get(component) );
					if (doorIndex != -1)
					{
						OpenDoor( doorIndex );
					}
				}
			}
		}
	}
	
	void CloseDoorLinked(int componentId)
	{
		ref LivespaceDoorData doorData = FindDoorByComponent(componentId);
		if (doorData != null)
		{
			foreach (int linkedDoorId : doorData.m_linkedDoorIds)
			{
				GetHouse().CloseDoor(linkedDoorId);
			}
			
			foreach (string component : doorData.m_components)
			{
				if ( m_actionComponents.Contains(component) )
				{
					int doorIndex = GetDoorIndex( m_actionComponents.Get(component) );
					if (doorIndex != -1)
					{
						CloseDoor( doorIndex );
					}
				}
			}
		}
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