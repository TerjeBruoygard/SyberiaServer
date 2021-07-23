modded class ActionCloseDoors
{
	override void OnStartServer( ActionData action_data )
	{
		Building building;
		if ( Class.CastTo(building, action_data.m_Target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if ( doorIndex != -1 )
			{
				building.CloseDoor(doorIndex);
				if (building.IsInherited(BuildingDoorBase))
				{
					BuildingDoorBase.Cast(building).CloseDoorLinked(doorIndex);
				}
			}
		}
	}
};