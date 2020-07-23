class CharProfile
{
	string m_name;
	int m_souls;
	string m_classname = "SurvivorM_Mirek";
	bool m_needToConfigureGear = true;
	bool m_needToForceRespawn = false;
	ref SkillsContainer m_skills;
	
	void CharProfile(string name, int souls)
	{
		m_name = name;
		m_souls = souls;
	}
};