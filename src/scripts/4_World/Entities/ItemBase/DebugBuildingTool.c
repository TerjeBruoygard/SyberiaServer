modded class DebugBuildingTool
{
	BuildingLivespace m_root;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		GetGame().ObjectDelete(m_root);
	}
	
	override void LinkHouse(House house)
	{
		// Cleanup
		GetGame().ObjectDelete(m_root);

		// Misc
		m_rulerCache = "0 0 0";
		m_linkedHouse = house;
		m_root = null;
		
		// Creation
		vector rot;
		string configPath = "CfgBuildingInfo " + house.GetType();
		if (GetGame().ConfigIsExisting(configPath))
		{
			int livespaceId = 0;
			while ( GetGame().ConfigIsExisting(configPath + " Livespace" + livespaceId) )
			{
				m_root = BuildingLivespace.Cast( GetGame().CreateObject("BuildingLivespace", house.GetPosition()) );
				m_root.SetPosition(house.GetPosition());
				m_root.SetYawPitchRoll(house.GetYawPitchRoll());
				m_root.Setup(house, livespaceId);

				livespaceId = livespaceId + 1;
			}
		}
	}
	
	override void UpgradeElement(BuildingLeveledElement element)
	{
		if (!m_linkedHouse)
			return;
		
		if (!m_root)
			return;
		
		if (!element)
			return;
		
		if (element.IsInherited(BuildingDoorBase))
		{
			BuildingDoorBase door = BuildingDoorBase.Cast( element );
			if (door && door.m_root)
			{
				door.m_root.UpgradeDoor(door.m_doorId);
			}
		}	
	}
};