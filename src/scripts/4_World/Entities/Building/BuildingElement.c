modded class BuildingElement
{
    BuildingLivespace m_livespace;
    
    void SetLivespace(BuildingLivespace livespace)
	{
		m_livespace = livespace;
	}
    
    BuildingLivespace GetLivespace()
    {
        return m_livespace;
    }
};