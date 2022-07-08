modded class ActionBandageBase: ActionContinuousBase
{
	override void ApplyBandage( ItemBase item, PlayerBase player )
	{	
		PlayerBase operator = PlayerBase.Cast( item.GetHierarchyRootPlayer() ); 
		if (!operator) return;
		
		player.ApplyAdvMedicineItem(item.GetType(), 1.0);
		
		float itemSpesisChance = item.GetInfectionChance();
		float skillSepsisChance = 1.0 - operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SEPSIS_CHANCE_DEC, 0, 0);
		if (player.HasDirtyHands() && Math.RandomFloat01() < skillSepsisChance)
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
		}
        else if (!player.HasDisinfectedHands() && Math.RandomFloat01() < skillSepsisChance * 0.1)
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
		}
		else if (Math.RandomFloat01() < itemSpesisChance * skillSepsisChance)
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
		}
		
		if (item.HasQuantity())
		{
			item.AddQuantity(-1, true);
		}
		else
		{
			item.Delete();
		}
        
		ItemBase gloves = player.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
        }
		
        operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineBandage);
	}
};