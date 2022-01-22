modded class PluginManager
{
	override void Init()
	{
		if (GetGame().IsServer())
		{
            RegisterPlugin("PluginSyberiaCharacters", false, true);
			RegisterPlugin("PluginZones", false, true);
            RegisterPlugin("PluginBuildingSystem", false, true);
		}

		super.Init();
	}
}