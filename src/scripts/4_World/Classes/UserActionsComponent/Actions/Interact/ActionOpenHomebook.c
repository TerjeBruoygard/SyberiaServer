modded class ActionOpenHomebook
{
    override void OnEndServer( ActionData action_data )
	{
		BuildingLivespace livepsace;
		if (!BuildingLivespace.CastTo(livepsace, action_data.m_Target.GetObject())) return;
		if (!livepsace.IsReady()) return;
		if (!action_data.m_Player) return;
		
		PluginBuildingSystem pluginBuildingSystem = PluginBuildingSystem.Cast(GetPlugin(PluginBuildingSystem));
		if (pluginBuildingSystem)
		{
			pluginBuildingSystem.SendHomebookMenuOpen(action_data.m_Player, livepsace);
		}
	};
};