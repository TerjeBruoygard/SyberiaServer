modded class BuildingLivespace
{
	private int m_recordId;
	private House m_house;
	private bool m_dbDirty;

	static BuildingLivespace SpawnLivespace(int id, House house, int livespaceId, ref map<string, string> data)
	{
		string configPath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		string classname = GetGame().ConfigGetTextOut(configPath + " classname");
		vector offset = GetGame().ConfigGetVector(configPath + " offset");
		vector position = house.ModelToWorld(offset);
		
		BuildingLivespace livespace = BuildingLivespace.Cast( GetGame().CreateObject(classname, position) );
		SybLogSrv("Spawn livespace " + classname + " => " + livespace);
			
		livespace.SetPosition(position);
		livespace.SetYawPitchRoll(house.GetYawPitchRoll());
		livespace.Setup(id, house, livespaceId, data);
		
		PluginBuildingSystem buildingPlugin = PluginBuildingSystem.Cast(GetPlugin(PluginBuildingSystem));
		if (buildingPlugin)
		{
			buildingPlugin.InsertLivespace(livespace);
		}
		
		return livespace;
	}
	
   	void Setup(int id, House house, int livespaceId, ref map<string, string> data)
	{
		m_recordId = id;
		m_house = house;
		m_dbDirty = false;
		m_livespaceId = livespaceId;
		m_livespacePath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		m_data = new LivespaceData(m_livespacePath);
		
		LoadDoorsData(data);
		LoadBarricadesData(data);		
		
		SetupDoors();
		SetupBarricades();
		m_ready = true;
		
		SychDirtyLevels();
		
		if (m_recordId == -1)
		{
			// Create in DB if not exists
			m_dbDirty = true;
		}
	}
	
	bool PopDbDirty()
	{
		if (m_dbDirty)
		{
			m_dbDirty = false;
			return true;
		}
		
		return false;
	}
	
	private void LoadBarricadesData(ref map<string, string> data)
	{
		m_barricadeLevels.Clear();
		array<string> parts = new array<string>;
		string barricadesData = "";
		if (data.Contains("wdat"))
		{
			barricadesData = data.Get("wdat");
		}
		barricadesData.Split(",", parts);
		for (int i = 0; i < m_data.m_barricades.Count(); i++)
		{
			int level = 0;
			if (parts.Count() > i && parts.Get(i) != "")
			{
				level = parts.Get(i).ToInt();
			}
			
			m_barricadeLevels.Insert(level);
		}
	}
	
	string SerializeBarricades()
	{
		string result = "";
		for (int i = 0; i < m_barricadeLevels.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_barricadeLevels.Get(i).ToString();
		}
		
		return result;
	}
	
	override void SetupBarricades()
	{
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
			m_dbDirty = true;
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
	
	private void LoadDoorsData(ref map<string, string> data)
	{
		m_doorLevels.Clear();
		array<string> parts = new array<string>;
		string doorsData = "";
		if (data.Contains("ddat"))
		{
			doorsData = data.Get("ddat");
		}
		doorsData.Split(",", parts);
		for (int i = 0; i < m_data.m_doors.Count(); i++)
		{
			int level = 0;
			if (parts.Count() > i && parts.Get(i) != "")
			{
				level = parts.Get(i).ToInt();
			}
			
			m_doorLevels.Insert(level);
		}
	}
	
	string SerializeDoors()
	{
		string result = "";
		for (int i = 0; i < m_doorLevels.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_doorLevels.Get(i).ToString();
		}
		
		return result;
	}
	
	override void SetupDoors()
	{
		for (int i = 0; i < m_data.m_doors.Count(); i++)
		{
			ref LivespaceDoorData doorData = m_data.m_doors[i];
			foreach (int linkedDoorId : doorData.m_linkedDoorIds)
			{
				GetHouse().CloseDoor(linkedDoorId);
			}
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
			m_dbDirty = true;
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
	
	ref LivespaceDoorData FindDoorByDoorIndex(int doorIndex)
	{
		if (doorIndex != -1)
		{
			foreach (ref LivespaceDoorData doorData : m_data.m_doors)
			{
				if (doorData.m_selfDoorId == doorIndex)
				{
					return doorData; 
				}	
			}
		}
		
		return null;
	}
	
	void OpenDoorLinked(int doorIndex)
	{
		ref LivespaceDoorData doorData = FindDoorByDoorIndex(doorIndex);
		if (doorData != null)
		{
			foreach (int linkedDoorId : doorData.m_linkedDoorIds)
			{
				GetHouse().OpenDoor(linkedDoorId);
			}
			
			//OpenDoor( doorData.m_selfDoorId );			
		}
	}
	
	void CloseDoorLinked(int doorIndex)
	{
		ref LivespaceDoorData doorData = FindDoorByDoorIndex(doorIndex);
		if (doorData != null)
		{
			foreach (int linkedDoorId : doorData.m_linkedDoorIds)
			{
				GetHouse().CloseDoor(linkedDoorId);
			}
			
			//CloseDoor( doorData.m_selfDoorId );
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
	
	int GetRecordId()
	{
		return m_recordId;
	}
	
	void SetRecordId(int id)
	{
		m_recordId = id;
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