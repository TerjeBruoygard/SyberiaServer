modded class ActionHomebookClaimOwnership
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		House house;
		if ( Class.CastTo(house, action_data.m_Target.GetObject()) )
		{		
			vector position = action_data.m_Target.GetCursorHitPos();
			PluginBuildingSystem buildingPlugin = PluginBuildingSystem.Cast(GetPlugin(PluginBuildingSystem));
			if (buildingPlugin)
			{
				BuildingLivespace livespace = buildingPlugin.FindByHousePoint(house, position);
				if (livespace != null)
				{
					return;
				}
				
				int livespaceId = BuildingLivespace.CheckPossibleLivespaceId(house, position);
				if (livespaceId == -1)
				{
					return;
				}
				
				action_data.m_MainItem.Delete();
				buildingPlugin.CreateLivespace(house, livespaceId, action_data.m_Player);		
				GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, action_data.m_Player.GetIdentity(), new Param1<string>("#syb_claim_home_message"));	
			}
		}
	}
};