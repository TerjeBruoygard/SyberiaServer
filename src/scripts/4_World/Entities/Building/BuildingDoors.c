modded class BuildingDoorBase
{
	void SetMetadata(int doorId, int linkedDoorId)
	{
		m_doorId = doorId;
		m_linkedDoorId = linkedDoorId;
		SetSynchDirty();
	}
	
	void OpenDoorLinked(int index)
	{	
		if (GetLivespace() && GetLivespace().GetHouse() && m_linkedDoorId != -1)
		{	
			GetLivespace().GetHouse().OpenDoor(m_linkedDoorId);
		}
	}
	
	void CloseDoorLinked(int index)
	{
		if (GetLivespace() && GetLivespace().GetHouse() && m_linkedDoorId != -1)
		{
			GetLivespace().GetHouse().CloseDoor(m_linkedDoorId);
		}
	}
};