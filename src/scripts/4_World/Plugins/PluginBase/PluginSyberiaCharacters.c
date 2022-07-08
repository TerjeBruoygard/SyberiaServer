class PluginSyberiaCharacters extends PluginBase
{
	private autoptr map<string, ref CharProfile> m_cachedProfiles;
	
	void PluginSyberiaCharacters()
	{
		m_cachedProfiles = new map<string, ref CharProfile>();
		
		DatabaseResponse response;
		ref array<string> queries = new array<string>;
		CharProfile.InitQueries(queries);
		GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);	
		delete queries;
	}
	
	void ~PluginSyberiaCharacters()
	{
		DatabaseResponse response;
		ref array<string> queries = new array<string>;
		foreach (string uid, ref CharProfile profile : m_cachedProfiles)
		{
			queries.Insert(profile.UpdateQuery());
			delete profile;
		}
		
		GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);
		delete queries;
	}
	
	ref CharProfile Get(ref PlayerIdentity identity, bool cacheOnly = false)
	{
		string uid = identity.GetId();
		ref CharProfile result = null;		
		if (m_cachedProfiles.Find(uid, result))
		{
			return result;
		}
		
		if (cacheOnly) 
		{
			return null;
		}
		
		DatabaseResponse response = null;
		if (GetDatabase().QuerySync(SYBERIA_DB_NAME, CharProfile.SelectQuery(uid), response))
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
				

		
		ref array<string> queries = new array<string>;
		newProfile.CreateQuery(queries);
		
		DatabaseResponse response = null;
		m_cachedProfiles.Insert(uid, newProfile);
		GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);

		if (response && response.GetRowsCount() == 1)
		{
			newProfile.m_id = response.GetValue(0, 0).ToInt();
		}	
		
		delete queries;
	}
	
	void Save(ref PlayerIdentity identity)
	{
		ref CharProfile profile;	
		string uid = identity.GetId();
		if (m_cachedProfiles.Find(uid, profile))
		{
			GetDatabase().QueryAsync(SYBERIA_DB_NAME, profile.UpdateQuery(), this, "OnUpdateOrCreateCharacter");
		}
	}
	
	void Delete(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		ref CharProfile result = null;		
		if (m_cachedProfiles.Find(uid, result))
		{
			m_cachedProfiles.Remove(uid);
			GetDatabase().QueryAsync(SYBERIA_DB_NAME, result.DeleteQuery(), this, "OnDeleteCharacter");						
			delete result;
		}
	}
	
	protected void OnDeleteCharacter(DatabaseResponse response) {}
	
	protected void OnUpdateOrCreateCharacter(DatabaseResponse response) {}
};

PluginSyberiaCharacters GetSyberiaCharacters() 
{
    return PluginSyberiaCharacters.Cast(GetPlugin(PluginSyberiaCharacters));
}
