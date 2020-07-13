class PluginSyberiaCharacters extends PluginBase
{
	private string m_rootPath = "$saves:characters";
	
	private autoptr map<string, ref CharProfile> m_cachedProfiles; 
	
	void PluginSyberiaCharacters()
	{
		m_cachedProfiles = new map<string, ref CharProfile>();
		MakeDirectory(m_rootPath);
	}
	
	void ~PluginSyberiaCharacters()
	{
		foreach (ref CharProfile profile : m_cachedProfiles)
		{
			string profilePath = m_rootPath + "\\" + uid + ".json";
			JsonFileLoader.JsonSaveFile(profilePath, profile);
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
		
		string profilePath = m_rootPath + "\\" + uid + ".json";		
		if (FileExist(profilePath))
		{
			JsonFileLoader.JsonLoadFile(profilePath, result);
		}
		
		if (result)
		{
			m_cachedProfiles.Insert(uid, result);
		}
		
		return result;
	}
	
	ref CharProfile Create(ref PlayerIdentity identity)
	{
		string uid = identity.GetId();
		if (m_cachedProfiles.Contains(uid))
		{
			m_cachedProfiles.Remove(uid);
		}
		
		string profilePath = m_rootPath + "\\" + uid + ".json";
		ref CharProfile result = new CharProfile();
		result.m_name = identity.GetName();
		result.m_souls = GetSyberiaOptions().m_startSoulsCount;
		
		m_cachedProfiles.Insert(uid, result);
		JsonFileLoader.JsonSaveFile(profilePath, result);
		return result; 
	}
	
	void Save(ref PlayerIdentity identity)
	{
		ref CharProfile profile;	
		string uid = identity.GetId();
		string profilePath = m_rootPath + "\\" + uid + ".json";
		if (m_cachedProfiles.Find(uid, profile))
		{
			JsonFileLoader.JsonSaveFile(profilePath, profile);
		}
	}
};

PluginSyberiaCharacters GetSyberiaCharacters() 
{
    return PluginSyberiaCharacters.Cast(GetPlugin(PluginSyberiaCharacters));
}
