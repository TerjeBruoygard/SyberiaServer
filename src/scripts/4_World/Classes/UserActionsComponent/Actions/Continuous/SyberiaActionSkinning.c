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
		}
		else if (targetObject && targetObject.IsInherited(Edible_Base))
		{
			Edible_Base edibleBase = Edible_Base.Cast(targetObject);
			edibleBase.DoSkinning(action_data.m_Player, action_data.m_MainItem);
		}
		
		if (action_data.m_MainItem)
		{
			float skill = 1 - action_data.m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_TOOLS, 0, 0);
			float dmgFactor = GetSyberiaConfig().m_animalsButchingKnifeDamage;
			float itemMaxHealth = action_data.m_MainItem.GetMaxHealth("", "");
			action_data.m_MainItem.DecreaseHealth("", "", skill * dmgFactor * itemMaxHealth );
		}
		
		ItemBase gloves = action_data.m_Player.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
        }
        else
        {
            action_data.m_Player.SetBloodyHands(true);
        }
	}
};