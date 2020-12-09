class PluginSyberiaCharacters extends PluginBase
{
	private autoptr map<string, ref CharProfile> m_cachedProfiles;
	
	void PluginSyberiaCharacters()
	{
		m_cachedProfiles = new map<string, ref CharProfile>();
		SyberiaDatabaseInit.InitIfNot();
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
	
	void Create(ref PlayerIdentity identity, ref CharProfile newProfile, Class callbackClass, string callbackFnc)
	{
		string uid = identity.GetId();
		if (m_cachedProfiles.Contains(uid))
		{
			delete m_cachedProfiles.Get(uid);
			m_cachedProfiles.Remove(uid);
		}

		TStringArray queries = new TStringArray;
		newProfile.CreateQuery(queries);
				
		ref auto params = new ref Param3<ref PlayerIdentity, Class, string>(identity, callbackClass, callbackFnc);
		GetDatabase().TransactionAsync(SYBERIA_DB_NAME, queries, this, "OnCreateNewCharacter", params);
	}
	
	void Save(ref PlayerIdentity identity)
	{
		ref CharProfile profile;	
		string uid = identity.GetId();
		if (m_cachedProfiles.Find(uid, profile))
		{
			GetDatabase().QueryAsync(SYBERIA_DB_NAME, profile.UpdateQuery(), this, "OnUpdateCharacter");
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
	
	protected void OnCreateNewCharacter(DatabaseResponse response, ref Param args)
	{
		ref auto metadata = Param3<ref PlayerIdentity, Class, string>.Cast(args);	
		ref PlayerIdentity sender = metadata.param1;	
		bool result = false;
		
		if (response && response.GetRowsCount() == 1)
		{
			int newId = response.GetValue(0, 0).ToInt();
			if (newId > 0)
			{
				result = true;
			}
		}	
		
		GetGame().GameScript.CallFunctionParams(
				metadata.param2, metadata.param3, null, 
				new Param2<ref PlayerIdentity, bool>(sender, result));
		
		delete args;
	}
	
	protected void OnDeleteCharacter(DatabaseResponse response, ref Param args) {}
	
	protected void OnUpdateCharacter(DatabaseResponse response, ref Param args) {}
};

PluginSyberiaCharacters GetSyberiaCharacters() 
{
    return PluginSyberiaCharacters.Cast(GetPlugin(PluginSyberiaCharacters));
}
