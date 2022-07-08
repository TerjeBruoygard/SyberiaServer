modded class BuildingDoorBase
{
	BuildingLivespace m_root;
	
	void SetMetadata(BuildingLivespace root, int doorId, int linkedDoorId)
	{
		m_root = root;
		m_doorId = doorId;
		m_linkedDoorId = linkedDoorId;
		SetSynchDirty();
	}
	
	void OpenDoorLinked(int index)
	{	
		if (m_root && m_root.m_house && m_linkedDoorId != -1)
		{	
			m_root.m_house.OpenDoor(m_linkedDoorId);
		}
	}
	
	void CloseDoorLinked(int index)
	{
		if (m_root && m_root.m_house && m_linkedDoorId != -1)
		{
			m_root.m_house.CloseDoor(m_linkedDoorId);
		}
	}
};