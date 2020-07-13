modded class PluginManager
{
	override void Init()
	{
		if (GetGame().IsServer())
		{
            RegisterPlugin("PluginSyberiaOptions", false, true);
            RegisterPlugin("PluginSyberiaCharacters", false, true);
		}

		super.Init();
	}
}