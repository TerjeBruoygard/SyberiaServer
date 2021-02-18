class CharProfile
{
	// Database fields
	int m_id;
	string m_uid;
	string m_name;
	int m_souls;
	string m_classname = "SurvivorM_Mirek";
	bool m_needToConfigureGear = true;
	bool m_needToForceRespawn = false;
	int m_respawnCounter = 0;
	ref SkillsContainer m_skills;
	
	// Inmemory fields
	ref array<int> m_startGear = null;
	
	void ~CharProfile()
	{
		if (m_skills) delete m_skills;
		
		if (m_startGear) delete m_startGear;
	}
	
	static void InitQueries(ref array<string> queries)
	{
		queries.Insert("CREATE TABLE IF NOT EXISTS characters (id INTEGER PRIMARY KEY AUTOINCREMENT, uid TEXT UNIQUE, name TEXT UNIQUE, displayName TEXT NOT NULL, souls INT NOT NULL, classname TEXT NOT NULL, needToConfigureGear BOOLEAN NOT NULL, needToForceRespawn BOOLEAN NOT NULL, respawnCounter INT NOT NULL, lastUpdate TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL);");
	}
	
	static void UpgradeQueries(ref array<string> queries)
	{
		queries.Insert("ALTER TABLE characters ADD COLUMN skills TEXT NOT NULL DEFAULT '';");
	}
	
	string UpdateQuery()
	{
		string fieldsSet = "souls=" + m_souls + ", ";
		fieldsSet = fieldsSet + "needToConfigureGear=" + ((int)m_needToConfigureGear) + ", ";
		fieldsSet = fieldsSet + "needToForceRespawn=" + ((int)m_needToForceRespawn) + ", ";
		fieldsSet = fieldsSet + "respawnCounter=" + m_respawnCounter + ", ";
		fieldsSet = fieldsSet + "skills='" + m_skills.SerializeAsDbField() + "', ";
		fieldsSet = fieldsSet + "lastUpdate=CURRENT_TIMESTAMP";
		return "UPDATE characters SET " + fieldsSet + " WHERE id = " + m_id + ";";
	}
	
	void CreateQuery(ref array<string> queries)
	{
		string lowercaseName = m_name + "";
		lowercaseName.ToLower();
		
		string insertValues = "'" + m_uid + "', '" + lowercaseName + "', '" + m_name + "', " + m_souls;
		insertValues = insertValues + ", '" + m_classname + "', " + ((int)m_needToConfigureGear);
		insertValues = insertValues + ", " + ((int)m_needToForceRespawn) + ", " + m_respawnCounter;
		insertValues = insertValues + ", '" + m_skills.SerializeAsDbField() + "'";
		queries.Insert( "INSERT INTO characters(uid, name, displayName, souls, classname, needToConfigureGear, needToForceRespawn, respawnCounter, skills) VALUES(" + insertValues + ");" );
		queries.Insert( "SELECT last_insert_rowid();" );
	}
	
	string DeleteQuery()
	{
		return "DELETE FROM characters WHERE id = " + m_id + ";";
	}
	
	static string SelectQuery(string uid)
	{
		return "SELECT id, uid, displayName, souls, classname, needToConfigureGear, needToForceRespawn, respawnCounter, skills FROM characters WHERE uid = '" + uid + "';";
	}
	
	void LoadFromDatabase(ref DatabaseResponse response)
	{
		m_id = response.GetValue(0, 0).ToInt();
		m_uid = response.GetValue(0, 1);
		m_name = response.GetValue(0, 2);
		m_souls = response.GetValue(0, 3).ToInt();
		m_classname = response.GetValue(0, 4);
		m_needToConfigureGear = response.GetValue(0, 5).ToInt();
		m_needToForceRespawn = response.GetValue(0, 6).ToInt();
		m_respawnCounter = response.GetValue(0, 7).ToInt();
		
		if (!m_skills) m_skills = SkillsContainer.Create();
		m_skills.DeserializeAsDbField( response.GetValue(0, 8) );
	}
};