modded class ActionBandageBase: ActionContinuousBase
{
	override void ApplyBandage( ItemBase item, PlayerBase player )
	{	
		PlayerBase operator = PlayerBase.Cast( item.GetHierarchyRootPlayer() ); 
		if (!operator) return;
		
		player.ApplyAdvMedicineItem(item.GetType(), 1.0);
		
		float sepsisChance = 1.0 - operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SEPSIS_CHANCE_DEC, 0, 0);
		if (Math.RandomFloat01() < sepsisChance && player.HasDirtyHands())
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
        
        operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineBandage);
	}
};