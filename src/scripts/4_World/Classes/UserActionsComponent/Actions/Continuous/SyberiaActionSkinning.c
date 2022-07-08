modded class ActionSkinning
{
	// Spawns the loot according to the Skinning class in the dead agent's config
	override void OnFinishProgressServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();		
		if (targetObject && targetObject.IsInherited(AnimalBase))
		{
			AnimalBase body_EB = AnimalBase.Cast(targetObject);
			body_EB.SetAsSkinned();
			body_EB.DoSkinning(action_data.m_Player, action_data.m_MainItem);
			PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
			module_lifespan.UpdateBloodyHandsVisibility( action_data.m_Player, true );
		}
		else
		{
			super.OnFinishProgressServer(action_data);
		}
		
		if (action_data.m_MainItem)
		{
			float skill = 1 - action_data.m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_TOOLS, 0, 0);
			float dmgFactor = GetSyberiaConfig().m_animalsButchingKnifeDamage;
			float itemMaxHealth = action_data.m_MainItem.GetMaxHealth("", "");
			action_data.m_MainItem.DecreaseHealth("", "", skill * dmgFactor * itemMaxHealth );
		}
	}
};