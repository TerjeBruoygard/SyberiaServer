modded class BuildingLivespace
{
	House m_house;
	BuildingHomeBook m_homebook;
	ref array<BuildingDoorBase> m_doors = new array<BuildingDoorBase>;
	
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
		
		SetupHomebook();
		SetupDoors();
		
		m_ready = true;		
	}
	
	private void SetupHomebook()
	{
		vector pos = m_house.ModelToWorld(GetGame().ConfigGetVector(m_livespacePath + " homeBookPos"));
		m_homebook = BuildingHomeBook.Cast( GetGame().CreateObject("BuildingHomeBook", pos) );
		
		vector rot = m_house.GetLocalYawPitchRoll();
		rot[0] = rot[0] + GetGame().ConfigGetFloat(m_livespacePath + " homeBookRot");
		m_homebook.SetPosition(pos);
		m_homebook.SetYawPitchRoll(rot);
	}
	
	private void SetupDoors()
	{
		int doorId = 0;
		while ( GetGame().ConfigIsExisting(m_livespacePath + " Door" + doorId) )
		{
			m_doors.Insert(null);
			m_doorLevels.Insert(1);
			SetupDoor(doorId);
			doorId = doorId + 1;
		}
	}
	
	private void SetupDoor(int id)
	{
		if (m_doors[id] != null)
		{
			GetGame().ObjectDelete(m_doors[id]);
			m_doors[id] = null;
		}
		
		string doorPath = m_livespacePath + " Door" + id;
		int maxLevel = GetGame().ConfigGetInt(doorPath + " maxLevel");
		int level = (int)Math.Clamp(m_doorLevels[id], 0, maxLevel);
		if (level > 0)
		{
			int doorType = GetGame().ConfigGetInt(doorPath + " type");
			int doorId = GetGame().ConfigGetInt(doorPath + " id");
			string doorClass = "BuildingDoor_T" + doorType + "_L" + level;
			
			vector localPos = GetGame().ConfigGetVector(doorPath + " pos");
			vector localOffset = GetGame().ConfigGetVector(CFG_VEHICLESPATH + " " + doorClass + " indoorOffset");
			localPos[0] = localPos[0] + localOffset[0];
			localPos[1] = localPos[1] + localOffset[1];
			localPos[2] = localPos[2] + localOffset[2];
			vector doorPos = m_house.ModelToWorld(localPos);
			float doorRot = GetGame().ConfigGetFloat(doorPath + " rot");
			BuildingDoorBase doorObj = BuildingDoorBase.Cast( GetGame().CreateObject(doorClass, doorPos) );
			if (doorObj)
			{
				vector rot = m_house.GetLocalYawPitchRoll();
				rot[0] = rot[0] + doorRot;
				doorObj.SetPosition(doorPos);
				doorObj.SetYawPitchRoll(rot);
				doorObj.SetMetadata(this, id, doorId);
				m_doors[id] = doorObj;
			}
			
			m_house.CloseDoor(doorId);
		}
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
			SetupDoor(id);
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
	
	override House GetHouse()
	{
		return m_house;
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		SybLog("BuildingLivespace: OnRPC " + rpc_type);
		
		if (rpc_type == SyberiaERPC.SYBERPC_SYNCH_LIVESPACE_CLIENT)
		{
			if (!m_ready) return;
			
			int houseNetId1, houseNetId2;
			m_house.GetNetworkID(houseNetId1, houseNetId2);
			
			int homebookNetId1, homebookNetId2;
			m_homebook.GetNetworkID(homebookNetId1, homebookNetId2);
			
			Param7<string, int, int, int, int, int, ref array<int>> params = 
				new Param7<string, int, int, int, int, int, ref array<int>>(m_livespacePath, m_livespaceId, houseNetId1, houseNetId2, homebookNetId1, homebookNetId2, m_doorLevels);
			RPCSingleParam(SyberiaERPC.SYBERPC_SYNCH_LIVESPACE_SERVER, params, true, sender);
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		if (m_homebook)
		{
			GetGame().ObjectDelete(m_homebook);
		}
		
		foreach (BuildingDoorBase door : m_doors)
		{
			if (door)
			{
				GetGame().ObjectDelete(door);
			}
		}
		delete m_doors;
	}
};