modded class ActionOpenDoors
{
	override void OnStartServer( ActionData action_data )
	{
		House building;
		if ( Class.CastTo(building, action_data.m_Target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if ( doorIndex != -1 )
			{
				int livespaceDoorIndex = -1;
				BuildingLivespace livespace = null;
				if (building.IsInherited(BuildingLivespace))
				{
					livespace = BuildingLivespace.Cast(building);
					livespaceDoorIndex = livespace.FindLivespaceDoorIdBySelfDoorIndex(doorIndex);
				}
				
				PluginBuildingSystem buildingPlugin = PluginBuildingSystem.Cast(GetPlugin(PluginBuildingSystem));
				if (buildingPlugin && livespace == null)
				{
					livespace = buildingPlugin.FindByHousePoint(building, building.GetDoorSoundPos(doorIndex));
					if (livespace != null)
					{
						livespaceDoorIndex = livespace.FindLivespaceDoorIdByLinkedDoorIndex(doorIndex);
					}
				}
				
				if (livespace != null && livespaceDoorIndex != -1)
				{
					if (livespace.OpenLivespaceDoor(action_data.m_Player, livespaceDoorIndex))
					{
						return;
					}
				}
			}
		}
		
		super.OnStartServer(action_data);
	}
};