modded class ActionSurgeryBase
{
    override void ApplySurgery( ItemBase item, PlayerBase operator, PlayerBase player, bool self)
    {
		string result = "";
		float skillRawChance = 0;
		float skillChanceGain = 0;
        float itemSpesisChance = item.GetInfectionChance();
		float skillSepsisChance = 1.0 - operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SEPSIS_CHANCE_DEC, 0, 0);
		float toolsDegradationMod = 1.0 - operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_TOOLS_DEGRADATION_DEC, 0, 0);
        if ( (GetSyberiaOptions().m_client.m_operateVisceraHimself || !self) && player.GetSybStats().m_visceraHit)
        {
			skillRawChance = operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SURGEON_VISCERA_HITS, 0, 0);
			skillChanceGain = GetSyberiaConfig().m_startSurgeryVisceraChance;
			if (Math.RandomFloat01() < skillRawChance + skillChanceGain)
			{
            	player.m_BleedingManagerServer.RemoveVisceraHit();
				result = result + "#syb_surgery_viscera_ok ";
			}
			else
			{
				result = result + "#syb_surgery_viscera_fail ";
			}
        }
        else if (player.GetSybStats().m_bulletHits > 0)
        {
			skillRawChance = operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SURGEON_BULLET_HITS, 0, 0);
			skillChanceGain = GetSyberiaConfig().m_startSurgeryBulletChance;
			if (Math.RandomFloat01() < skillRawChance + skillChanceGain)
			{
            	player.m_BleedingManagerServer.RemoveBulletHit(false);
				result = result + "#syb_surgery_bullet_ok ";
			}
			else
			{
				player.m_BleedingManagerServer.RemoveBulletHit(true);
				player.m_BleedingManagerServer.AddBulletHit();
				result = result + "#syb_surgery_bullet_fail ";
			}
        }
        else if (player.GetSybStats().m_knifeHits > 0)
        {
			skillRawChance = operator.GetPerkFloatValue(SyberiaPerkType.SYBPERK_MEDICINE_SURGEON_KNIFE_HITS, 0, 0);
			skillChanceGain = GetSyberiaConfig().m_startSurgeryKnifeChance;
			if (Math.RandomFloat01() < skillRawChance + skillChanceGain)
			{
            	player.m_BleedingManagerServer.RemoveKnifeHit(false);
				result = result + "#syb_surgery_knife_ok ";
			}
			else
			{
				player.m_BleedingManagerServer.RemoveKnifeHit(true);
				player.m_BleedingManagerServer.AddKnifeHit();
				result = result + "#syb_surgery_knife_fail ";
			}
        }
		
		if (Math.RandomFloat01() < 1.0 - skillRawChance)
		{
			int sideEffect = Math.RandomIntInclusive(1, 3);
			if (sideEffect == 1 && Math.RandomFloat01() < skillSepsisChance * 0.1)
			{
				player.m_BleedingManagerServer.SetBloodInfection(true);
				result = result + "#syb_surgery_side_effect1 ";
			}
			else if (sideEffect == 2)
			{
				player.DecreaseHealth("", "Blood", GetSyberiaConfig().m_surgerySideEffectBloodLoseCount);
				result = result + "#syb_surgery_side_effect2 ";
			}
			else if (sideEffect == 3)
			{
				player.DecreaseHealth("", "Health", GetSyberiaConfig().m_surgerySideEffectHealthLoseCount);
				result = result + "#syb_surgery_side_effect3 ";
			}
		}
        
        if (operator.HasDirtyHands())
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
            result = result + "#syb_surgery_sepsis ";
		}
        else if (Math.RandomFloat01() < skillSepsisChance && !operator.HasDisinfectedHands())
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
			result = result + "#syb_surgery_sepsis ";
		}
        else if (Math.RandomFloat01() < itemSpesisChance * skillSepsisChance)
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
			result = result + "#syb_surgery_sepsis ";
		}
        
        ItemBase gloves = operator.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
        }
        else
        {
            operator.SetBloodyHands(true);
        }

        if (item.HasQuantity())
        {
            item.AddQuantity(-50 * toolsDegradationMod, true);
        }
        else
        {
            item.Delete();
        }
		
		if (result != "")
		{
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, operator.GetIdentity(), new Param1<string>(result));	
		}
		
		operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineSurgery);
    }
};