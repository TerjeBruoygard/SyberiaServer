modded class PluginManager
{
	override void Init()
	{
		if (GetGame().IsServer())
		{
            RegisterPlugin("PluginSyberiaOptions", false, true);
            RegisterPlugin("PluginSyberiaCharactersDB", false, true);
		}

		super.Init();
	}
}