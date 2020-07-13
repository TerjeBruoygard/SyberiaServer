class PluginSyberiaOptions extends PluginBase
{
	int m_startSoulsCount = 3;
	int m_respawnSoulsPrice = 1;
	
	int m_newchar_points = 10;
	
	void PluginSyberiaOptions()
	{
		string path = "$profile:PluginSyberiaOptions.json";
		
		if (FileExist(path))
		{
			JsonFileLoader.JsonLoadFile(path, this);
		}
		else
		{
			JsonFileLoader.JsonSaveFile(path, this);
		}
	}
	
	void ~PluginSyberiaOptions()
	{

	}
};

PluginSyberiaOptions GetSyberiaOptions() 
{
    return PluginSyberiaOptions.Cast(GetPlugin(PluginSyberiaOptions));
}