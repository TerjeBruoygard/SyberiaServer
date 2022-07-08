class CharProfile
{
	string m_name;
	int m_souls;
	string m_classname = "SurvivorM_Mirek";
	bool m_needToConfigureGear = true;
	bool m_needToForceRespawn = false;
	ref SkillsContainer m_skills;
	ref array<int> m_startGear = null;
	
	void CharProfile(string name, int souls)
	{
		m_name = name;
		m_souls = souls;
	}
	
	void ~CharProfile()
	{
		if (m_skills) delete m_skills;
		
		if (m_startGear) delete m_startGear;
	}
};