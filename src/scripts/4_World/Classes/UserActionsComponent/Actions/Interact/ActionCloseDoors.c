modded class ActionCloseDoors
{
	override void OnStartServer( ActionData action_data )
	{
		House building;
		if ( Class.CastTo(building, action_data.m_Target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if ( doorIndex != -1 )
			{
				building.CloseDoor(doorIndex);
				if (building.IsInherited(BuildingLivespace))
				{
					BuildingLivespace.Cast(building).CloseDoorLinked(doorIndex);
				}
			}
		}
	}
};