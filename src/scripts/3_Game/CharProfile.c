class CharProfile
{
	// Database fields
	string m_name;
	int m_souls;
	string m_classname = "SurvivorM_Mirek";
	bool m_needToConfigureGear = true;
	bool m_needToForceRespawn = false;
	int m_respawnCounter = 0;
	
	// Inmemory fields
	ref SkillsContainer m_skills;
	ref array<int> m_startGear = null;
	
	void ~CharProfile()
	{
		if (m_skills) delete m_skills;
		
		if (m_startGear) delete m_startGear;
	}
	
	static string ToFieldsDesc()
	{
		return "name TEXT NOT NULL, souls INT NOT NULL, classname TEXT NOT NULL, needToConfigureGear BOOLEAN NOT NULL, needToForceRespawn BOOLEAN NOT NULL, respawnCounter INT NOT NULL";
	}
	
	static string ToSaveProfileFields()
	{
		return "name, souls, classname, needToConfigureGear, needToForceRespawn, respawnCounter";
	}
	
	string ToSaveProfileValue()
	{
		return "'" + m_name + "', " + m_souls + ", '" + m_classname + "', " + ((int)m_needToConfigureGear) + ", " + ((int)m_needToForceRespawn) + ", " + m_respawnCounter;
	}
	
	string ToSaveProfileData()
	{
		string result = "name='" + m_name + "', ";
		result = result + "souls=" + m_souls + ", ";
		result = result + "classname='" + m_classname + "', ";
		result = result + "needToConfigureGear=" + ((int)m_needToConfigureGear) + ", ";
		result = result + "needToForceRespawn=" + ((int)m_needToForceRespawn) + ", ";
		result = result + "respawnCounter=" + m_respawnCounter;
		return result;
	}
	
	void LoadFromDatabase(ref DatabaseResponse response)
	{
		m_name = response.GetValue(0, 0);
		m_souls = response.GetValue(0, 1).ToInt();
		m_classname = response.GetValue(0, 2);
		m_needToConfigureGear = response.GetValue(0, 3) == "True";
		m_needToForceRespawn = response.GetValue(0, 4) == "True";
		m_respawnCounter = response.GetValue(0, 5).ToInt();
	}
};