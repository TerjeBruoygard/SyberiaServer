modded class PluginManager
{
	override void Init()
	{
		if (GetGame().IsServer())
		{
            RegisterPlugin("PluginSyberiaCharacters", false, true);
		}

		super.Init();
	}
}