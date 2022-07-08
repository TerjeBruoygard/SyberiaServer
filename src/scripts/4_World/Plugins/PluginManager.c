modded class PluginManager
{
	override void Init()
	{
		if (GetGame().IsServer())
		{
            RegisterPlugin("PluginSyberiaCharacters", false, true);
			RegisterPlugin("PluginZones", false, true);
		}

		super.Init();
	}
}