modded class BuildingWindowBase
{
	void SetMetadata(int windowId)
	{
		m_windowId = windowId;
		SetSynchDirty();
	}
};