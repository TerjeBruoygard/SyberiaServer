modded class ActionLightItemOnFire
{
    override void OnFinishProgressServer( ActionData action_data )
	{
		bool can_be_ignited = false;
		ItemBase target_item = ItemBase.Cast( action_data.m_Target.GetObject() );
		ItemBase item = action_data.m_MainItem;
		ItemBase ignited_item;				//item that was lit on fire
		ItemBase fire_source_item;			//item that was the source of fire
		float chance = GetSyberiaConfig().m_igniteFireplaceBaseSkillcheckChance;
		chance = chance + action_data.m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_SURVIVAL_MAKEFIRE_INC, 0, 0);
			
		if (Math.RandomFloat01() < chance)
		{
			super.OnFinishProgressServer(action_data);
		}
		else
		{
			if ( item.CanIgniteItem( target_item ) )
			{
				ignited_item = target_item;
				fire_source_item = item;
				can_be_ignited = true;
			}
			else if ( item.CanBeIgnitedBy( target_item ) )
			{
				ignited_item = item;
				fire_source_item = target_item;	
				can_be_ignited = true;		
			}
			
			if (can_be_ignited)
			{
				fire_source_item.OnIgnitedTargetFailed( ignited_item );
				ignited_item.OnIgnitedThisFailed( fire_source_item ); 
				GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, action_data.m_Player.GetIdentity(), new Param1<string>("#syb_failed_ignite_skillcheck"));	
			}
		}
	}
};