class PluginSyberiaCharacters extends PluginBase
{
	private autoptr map<string, ref CharProfile> m_cachedProfiles; 
	
	void PluginSyberiaCharacters()
	{
		m_cachedProfiles = new map<string, ref CharProfile>();
		
		DatabaseResponse response;
		ref array<string> queries = new array<string>;
		queries.Insert("CREATE TABLE IF NOT EXISTS characters (uid TEXT PRIMARY KEY, " + CharProfile.ToFieldsDesc() + ");");
		if (!GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response))
		{
			Error("FAIELD TO INITIALIZE DATABASE CONNECTION FROM PluginSyberiaCharacters");
		}	
		delete queries;
	}
	
	void ~PluginSyberiaCharacters()
	{
		DatabaseResponse response;
		ref array<string> queries = new array<string>;
		foreach (string uid, ref CharProfile profile : m_cachedProfiles)
		{
			queries.Insert("UPDATE characters SET " + profile.ToSaveProfileData() + " WHERE uid = '" + uid + "';");
			delete profile;
		}
		
		GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);
		delete queries;
	}
	
	ref CharProfile Get(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		ref CharProfile result = null;		
		if (m_cachedProfiles.Find(uid, result))
		{
			return result;
		}
		
		DatabaseResponse response = null;
		if (GetDatabase().QuerySync(SYBERIA_DB_NAME, "SELECT " + CharProfile.ToSaveProfileFields() + " FROM characters WHERE uid = '" + uid + "';", response))
		{
			if (response && response.GetRowsCount() == 1) 
			{
				result = new CharProfile();
				result.LoadFromDatabase(response);
				m_cachedProfiles.Insert(uid, result);
			}
		}

		return result;
	}
	
	void Create(ref PlayerIdentity identity, ref CharProfile newProfile)
	{
		string uid = identity.GetId();
		if (m_cachedProfiles.Contains(uid))
		{
			delete m_cachedProfiles.Get(uid);
			m_cachedProfiles.Remove(uid);
		}
				
		m_cachedProfiles.Insert(uid, newProfile);
		GetDatabase().QueryAsync(SYBERIA_DB_NAME, "INSERT OR REPLACE INTO characters(uid, " + CharProfile.ToSaveProfileFields() + ") VALUES('" + uid + "', " + newProfile.ToSaveProfileValue() + ");", this, "OnUpdateOrCreateCharacter");	
	}
	
	void Save(ref PlayerIdentity identity)
	{
		ref CharProfile profile;	
		string uid = identity.GetId();
		if (m_cachedProfiles.Find(uid, profile))
		{
			GetDatabase().QueryAsync(SYBERIA_DB_NAME, "UPDATE characters SET " + profile.ToSaveProfileData() + " WHERE uid = '" + uid + "';", this, "OnUpdateOrCreateCharacter");
		}
	}
	
	void Delete(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		if (m_cachedProfiles.Contains(uid))
		{
			delete m_cachedProfiles.Get(uid);
			m_cachedProfiles.Remove(uid);
		}
		
		GetDatabase().QueryAsync(SYBERIA_DB_NAME, "DELETE FROM characters WHERE uid = '" + uid + "';", this, "OnDeleteCharacter");
	}
	
	protected void OnDeleteCharacter(DatabaseResponse response) {}
	
	protected void OnUpdateOrCreateCharacter(DatabaseResponse response) {}
};

PluginSyberiaCharacters GetSyberiaCharacters() 
{
    return PluginSyberiaCharacters.Cast(GetPlugin(PluginSyberiaCharacters));
}
