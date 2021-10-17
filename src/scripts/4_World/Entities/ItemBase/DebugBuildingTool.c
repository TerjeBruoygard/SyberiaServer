modded class DebugBuildingTool
{
	ref array<BuildingLivespace> m_livespaces = new array<BuildingLivespace>;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		foreach (BuildingLivespace livespaceToDel : m_livespaces) {
			GetGame().ObjectDelete(livespaceToDel);
		}
		
		delete m_livespaces;
	}
	
	override void LinkHouse(House house)
	{
		// Cleanup
		foreach (BuildingLivespace livespaceToDel : m_livespaces) {
			GetGame().ObjectDelete(livespaceToDel);
		}		
		delete m_livespaces;

		// Misc
		m_rulerCache = "0 0 0";
		m_linkedHouse = house;
		m_livespaces = new array<BuildingLivespace>;
		
		// Creation
		vector rot;
		string configPath = "CfgBuildingInfo " + house.GetType();
		if (GetGame().ConfigIsExisting(configPath))
		{
			int livespaceId = 0;
			while ( GetGame().ConfigIsExisting(configPath + " Livespace" + livespaceId) )
			{
				BuildingLivespace livespace = BuildingLivespace.SpawnLivespace(house, livespaceId);
				m_livespaces.Insert(livespace);
				livespaceId = livespaceId + 1;
			}
		}
	}
	
	override void UpgradeElement(BuildingLivespace livespace, int componentId)
	{
		SybLogSrv("DebugBuildingTool " + livespace + " upgrade element: " + componentId);
		/*if (!m_linkedHouse)
			return;
		
		if (!element)
			return;
		
		if (!element.GetLivespace())
			return;
		
		if (element.IsInherited(BuildingDoorBase))
		{
			BuildingDoorBase door = BuildingDoorBase.Cast( element );
			door.GetLivespace().UpgradeDoor(door.GetDoorId());
		}	
		else if (element.IsInherited(BuildingWindowBase))
		{
			BuildingWindowBase window = BuildingWindowBase.Cast( element );
			window.GetLivespace().UpgradeWindow(window.GetWindowId());
		}*/
	}
};