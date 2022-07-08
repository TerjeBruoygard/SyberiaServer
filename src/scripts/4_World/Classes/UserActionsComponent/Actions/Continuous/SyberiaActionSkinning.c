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
			float skill = (action_data.m_Player.GetSkillValue(SyberiaSkillType.SYBSKILL_HUNTING) / 5.0) + 1.5;
			action_data.m_MainItem.DecreaseHealth("", "", (1 / skill) * action_data.m_MainItem.GetMaxHealth("", ""));
		}
	}
};