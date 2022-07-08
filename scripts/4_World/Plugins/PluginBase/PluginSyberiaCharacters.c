class PluginSyberiaCharacters extends PluginBase
{
	private string m_rootPath = "$profile:CharProfiles";
	
	private autoptr map<string, ref CharProfile> m_cachedProfiles; 
	
	void PluginSyberiaCharacters()
	{
		m_cachedProfiles = new map<string, ref CharProfile>();
		MakeDirectory(m_rootPath);
	}
	
	void ~PluginSyberiaCharacters()
	{
		foreach (string uid, ref CharProfile profile : m_cachedProfiles)
		{
			string profilePath = GetProfilePath(uid);
			JsonFileLoader<ref CharProfile>.JsonSaveFile(profilePath, profile);
			delete profile;
		}
	}
	
	ref CharProfile Get(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		ref CharProfile result = null;		
		if (m_cachedProfiles.Find(uid, result))
		{
			return result;
		}
		
		string profilePath = GetProfilePath(uid);		
		if (FileExist(profilePath))
		{
			JsonFileLoader<ref CharProfile>.JsonLoadFile(profilePath, result);
		}
		
		if (result)
		{
			m_cachedProfiles.Insert(uid, result);
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
		
		string profilePath = GetProfilePath(uid);		
		m_cachedProfiles.Insert(uid, newProfile);
		JsonFileLoader<ref CharProfile>.JsonSaveFile(profilePath, newProfile);
	}
	
	void Save(ref PlayerIdentity identity)
	{
		ref CharProfile profile;	
		string uid = identity.GetId();
		string profilePath = GetProfilePath(uid);
		if (m_cachedProfiles.Find(uid, profile))
		{
			JsonFileLoader<ref CharProfile>.JsonSaveFile(profilePath, profile);
		}
	}
	
	void Delete(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		string profilePath = GetProfilePath(uid);
		if (m_cachedProfiles.Contains(uid))
		{
			delete m_cachedProfiles.Get(uid);
			m_cachedProfiles.Remove(uid);
		}
		
		DeleteFile(profilePath);
	}
	
	private string GetProfilePath(string uid)
	{
		return m_rootPath + "\\" + uid + ".json";
	}
};

PluginSyberiaCharacters GetSyberiaCharacters() 
{
    return PluginSyberiaCharacters.Cast(GetPlugin(PluginSyberiaCharacters));
}
