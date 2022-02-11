modded class BuildingLivespace
{
	private int m_recordId;
	private ref array<string> m_owners;
	private ref array<string> m_members;
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
		m_synchData.m_livespaceId = livespaceId;
		m_synchData.m_livespacePath = "CfgBuildingInfo " + house.GetType() + " Livespace" + livespaceId;
		m_data = new LivespaceData(m_synchData.m_livespacePath);
		
		LoadOwnersAndMembers(data);	
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
	
	void GetLivespaceHomebookData(ref LivespaceHomebookData data)
	{
		data.m_owners = m_owners;
		data.m_members = m_members;
	}
	
	private void LoadOwnersAndMembers(ref map<string, string> data)
	{
		m_owners = new array<string>;
		if (data.Contains("ownrs"))
		{
			string ownersStr = data.Get("ownrs");
			ownersStr.Split(",", m_owners);
		}
		
		m_members = new array<string>;
		if (data.Contains("mmbrs"))
		{
			string membersStr = data.Get("mmbrs");
			membersStr.Split(",", m_members);
		}
	}
	
	string SerializeOwners()
	{
		string result = "";
		for (int i = 0; i < m_owners.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_owners.Get(i);
		}
		
		return result;
	}
	
	string SerializeMembers()
	{
		string result = "";
		for (int i = 0; i < m_members.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_members.Get(i);
		}
		
		return result;
	}
	
	private void LoadBarricadesData(ref map<string, string> data)
	{
		m_synchData.m_barricadeLevels.Clear();
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
			
			m_synchData.m_barricadeLevels.Insert(level);
		}
	}
	
	string SerializeBarricades()
	{
		string result = "";
		for (int i = 0; i < m_synchData.m_barricadeLevels.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_synchData.m_barricadeLevels.Get(i).ToString();
		}
		
		return result;
	}
	
	override void SetupBarricades()
	{
		super.SetupBarricades();
	}
	
	void SetBarricadeLevel(int id, int level)
	{
		int lastLevel = m_synchData.m_barricadeLevels[id];
		if (lastLevel != level)
		{
			m_synchData.m_barricadeLevels[id] = level;
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
		m_synchData.m_doorLevels.Clear();
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
			
			m_synchData.m_doorLevels.Insert(level);
		}
	}
	
	string SerializeDoors()
	{
		string result = "";
		for (int i = 0; i < m_synchData.m_doorLevels.Count(); i++)
		{
			if (i > 0)
			{
				result = result + ",";
			}
			
			result = result + m_synchData.m_doorLevels.Get(i).ToString();
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
				GetHouse().LockDoor(linkedDoorId);
			}
			
			this.CloseDoor(doorData.m_selfDoorId);
			this.LockDoor(doorData.m_selfDoorId);
		}
		
		super.SetupDoors();
	}
	
	void SetDoorLevel(int id, int level)
	{
		int lastLevel = m_synchData.m_doorLevels[id];
		if (lastLevel != level)
		{
			m_synchData.m_doorLevels[id] = level;
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
	
	int FindLivespaceDoorIdBySelfDoorIndex(int doorIndex)
	{
		if (doorIndex != -1)
		{
			for (int i = 0; i < m_data.m_doors.Count(); i++)
			{
				ref LivespaceDoorData doorData = m_data.m_doors.Get(i);
				if (doorData.m_selfDoorId == doorIndex)
				{
					return i; 
				}	
			}
		}
		
		return -1;
	}
	
	int FindLivespaceDoorIdByLinkedDoorIndex(int doorIndex)
	{
		if (doorIndex != -1)
		{
			for (int i = 0; i < m_data.m_doors.Count(); i++)
			{
				ref LivespaceDoorData doorData = m_data.m_doors.Get(i);
				if (doorData.m_linkedDoorIds.Find(doorIndex) != -1)
				{
					return i; 
				}	
			}
		}
		
		return -1;
	}
	
	bool OpenLivespaceDoor(PlayerBase player, int livespaceDoorId)
	{		
		if (livespaceDoorId == -1)
		{
			return false;
		}
		
		ref LivespaceDoorData doorData = m_data.m_doors.Get(livespaceDoorId);
		if (doorData == null)
		{
			return false;
		}
		
		if (!player || !player.GetIdentity())
		{
			return true;
		}
		
		PlayerIdentity identity = player.GetIdentity();
		if (!IsOwner(identity) && !IsMember(identity))
		{
			return true;
		}
		
		this.UnlockDoor(doorData.m_selfDoorId);
		this.OpenDoor(doorData.m_selfDoorId);	
		
		foreach (int linkedDoorId : doorData.m_linkedDoorIds)
		{
			GetHouse().UnlockDoor(linkedDoorId);
			GetHouse().OpenDoor(linkedDoorId);
		}	
		
		return true;
	}
	
	bool CloseLivespaceDoor(PlayerBase player, int livespaceDoorId)
	{
		if (livespaceDoorId == -1)
		{
			return false;
		}
		
		ref LivespaceDoorData doorData = m_data.m_doors.Get(livespaceDoorId);
		if (doorData == null)
		{		
			return false;
		}
				
		foreach (int linkedDoorId : doorData.m_linkedDoorIds)
		{
			GetHouse().CloseDoor(linkedDoorId);
			GetHouse().LockDoor(linkedDoorId);
		}
		
		this.CloseDoor(doorData.m_selfDoorId);	
		this.LockDoor(doorData.m_selfDoorId);		
		return true;
	}
	
	private void SychDirtyLevels(PlayerIdentity sender = NULL)
	{
		if (!m_ready) return;
			
		int houseNetId1, houseNetId2;
		m_house.GetNetworkID(houseNetId1, houseNetId2);
		m_synchData.m_houseNetId1 = houseNetId1;
		m_synchData.m_houseNetId2 = houseNetId2;
		
		Param1<ref LivespaceSynchData> params = new Param1<ref LivespaceSynchData>(m_synchData);
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
	
	bool IsOwner(PlayerIdentity identity)
	{
		if (!identity) return false;
		
		string uid = identity.GetId();
		return m_owners.Find(uid) != -1;
	}
	
	bool IsMember(PlayerIdentity identity)
	{
		if (!identity) return false;
		
		string uid = identity.GetId();
		return m_members.Find(uid) != -1;
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
		
		if (m_owners)
		{
			delete m_owners;
		}
		
		if (m_members)
		{
			delete m_members;
		}
		
		PluginBuildingSystem buildingPlugin = PluginBuildingSystem.Cast(GetPlugin(PluginBuildingSystem));
		if (buildingPlugin)
		{
			buildingPlugin.DeleteLivespaceRecord(this);
		}
	}
};