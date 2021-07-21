modded class DebugBuildingTool
{
	ref array<Object> m_previewObjects = new array<Object>;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		CleanupDebugObjects();
	}
	
	private void CleanupDebugObjects()
	{
		while (m_previewObjects.Count() > 0)
		{
			GetGame().ObjectDelete(m_previewObjects.Get(0));
			m_previewObjects.Remove(0);
		}
	}
	
	override void LinkHouse(House house)
	{
		// Cleanup
		CleanupDebugObjects();

		// Misc
		m_rulerCache = "0 0 0";
		m_linkedHouse = house;
		
		// Creation
		vector rot;
		string configPath = "CfgBuildingInfo " + house.GetType();
		if (GetGame().ConfigIsExisting(configPath))
		{
			int livespaceId = 0;
			while ( GetGame().ConfigIsExisting(configPath + " Livespace" + livespaceId) )
			{
				string livespacePath = configPath + " Livespace" + livespaceId;
				vector homeBookPos = house.ModelToWorld(GetGame().ConfigGetVector(livespacePath + " homeBookPos"));
				Object homebook = GetGame().CreateObject("BuildingHomeBook", homeBookPos);
				if (homebook)
				{
					rot = house.GetLocalYawPitchRoll();
					rot[0] = rot[0] + GetGame().ConfigGetFloat(livespacePath + " homeBookRot");
					homebook.SetPosition(homeBookPos);
					homebook.SetYawPitchRoll(rot);
					m_previewObjects.Insert(homebook);
				}
				
				int doorId = 0;
				while ( GetGame().ConfigIsExisting(livespacePath + " Door" + doorId) )
				{
					string doorPath = livespacePath + " Door" + doorId;
					int doorType = GetGame().ConfigGetInt(doorPath + " type");
					vector doorPos = house.ModelToWorld(GetGame().ConfigGetVector(doorPath + " pos"));
					float doorRot = GetGame().ConfigGetFloat(doorPath + " rot");
					Object doorObj = GetGame().CreateObject("BuildingDoor_T" + doorType + "_L1", doorPos);
					if (doorObj)
					{
						rot = house.GetLocalYawPitchRoll();
						rot[0] = rot[0] + doorRot;
						doorObj.SetPosition(doorPos);
						doorObj.SetYawPitchRoll(rot);
						m_previewObjects.Insert(doorObj);
					}
					
					doorId = doorId + 1;
				}
				
				livespaceId = livespaceId + 1;
			}
		}
	}
	
	override void UpgradeElement(BuildingLeveledElement element)
	{
		if (!m_linkedHouse)
			return;
		
		if (!element)
			return;
		
		int index = m_previewObjects.Find(element);
		if (index == -1)
			return;
		
		vector pos = element.GetPosition();
		vector rot = element.GetLocalYawPitchRoll();
		string next = element.GetNextLevel();		
		m_previewObjects.Remove(index);
		GetGame().ObjectDelete(element);
		
		Object newElement = GetGame().CreateObject(next, pos);
		if (newElement)
		{
			newElement.SetPosition(pos);
			newElement.SetYawPitchRoll(rot);
			m_previewObjects.Insert(newElement);
		}		
	}
};