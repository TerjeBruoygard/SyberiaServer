modded class ActionSkinning
{
	// Spawns the loot according to the Skinning class in the dead agent's config
	override void OnFinishProgressServer( ActionData action_data )
	{
		float knifeDmgMultiplier = 1;
		Object targetObject = action_data.m_Target.GetObject();		
		if (targetObject && targetObject.IsInherited(AnimalBase))
		{
			AnimalBase body_EB = AnimalBase.Cast(targetObject);
			body_EB.SetAsSkinned();
			body_EB.DoSkinning(action_data.m_Player, action_data.m_MainItem);
			knifeDmgMultiplier = targetObject.ConfigGetFloat("knifeDamageModifier");
		}
		else if (targetObject && targetObject.IsInherited(Edible_Base))
		{
			Edible_Base edibleBase = Edible_Base.Cast(targetObject);
			edibleBase.DoSkinning(action_data.m_Player, action_data.m_MainItem);
			knifeDmgMultiplier = targetObject.ConfigGetFloat("knifeDamageModifier");
		}
		
		if (knifeDmgMultiplier <= 0)
		{
			knifeDmgMultiplier = 1;
		}
		
		if (action_data.m_MainItem)
		{
			float skill = 1 - action_data.m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_TOOLS, 0, 0);
			float dmgValue = GetSyberiaConfig().m_animalsButchingKnifeDamage;
			float decreaseHealthValue = skill * dmgValue * knifeDmgMultiplier;
			MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, decreaseHealthValue);
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