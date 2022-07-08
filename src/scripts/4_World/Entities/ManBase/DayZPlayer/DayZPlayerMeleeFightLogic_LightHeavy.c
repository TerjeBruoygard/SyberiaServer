modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override protected void DamageHands(DayZPlayer DZPlayer, ClothingBase gloves, InventoryItem itemInHands)
	{
		EntityAI target = m_MeleeCombat.GetTargetEntity();
		
		//We did not hit anything
		if ( !target )
			return;
		
		//Check if server side
		bool affectHands = true;
        if (itemInHands)
        {
            if ( (itemInHands.IsWeapon() || itemInHands.IsMeleeWeapon()) && !IsItemUsingFistAmmo(itemInHands) )
			{
				affectHands = false;
			}
			else
			{
				itemInHands.DecreaseHealth( "", "", GetSyberiaConfig().m_damageNonWeaponItemInHandsOnAttack );
			}
        }
           
		//If gloves, damage gloves
		if (affectHands)
		{
			if ( gloves && gloves.GetHealthLevel() < GameConstants.STATE_RUINED )
			{
				gloves.DecreaseHealth("", "", GetSyberiaConfig().m_damageGlovesOnHandsOnAttack);
			}
			else if (Math.RandomFloat01() <= GetSyberiaConfig().m_damageHandsOnAttackChance)
			{
				PlayerBase player = PlayerBase.Cast(DZPlayer);
				player.SetBloodyHands(true);
				
				if (target.IsZombie() && Math.RandomFloat01() <= GetSyberiaConfig().m_zvirusZombieBloodTransferChance)
				{
					player.GetBleedingManagerServer().SetZVirus( true );
				}
			}
		}
	}
	
	private bool IsItemUsingFistAmmo(InventoryItem itemInHands)
	{
		if (itemInHands && GetGame().ConfigIsExisting("CfgVehicles " + itemInHands.GetType() + " MeleeModes Default ammo"))
		{
			if (GetGame().ConfigGetTextOut("CfgVehicles " + itemInHands.GetType() + " MeleeModes Default ammo") == "MeleeFist")
			{
				return true;
			}
		}
		
		return false;
	}
	
	override protected bool EvaluateFinisherAttack(InventoryItem weapon, Object target)
	{		
		bool result = super.EvaluateFinisherAttack(weapon, target);		
		if (result)
		{
			// Add strength skill
			PlayerBase player = PlayerBase.Cast(m_DZPlayer);
			if (player && Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStealthSilentAttackChance)
			{
				player.AddExperience(SyberiaSkillType.SYBSKILL_STEALTH, GetSyberiaConfig().m_skillsExpStealthSilentAttackValue);
			}
		}
		
		return result;
	}
	
	override int GetFinisherType(InventoryItem weapon, EntityAI target)
	{
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		if (player)
		{
			if (player.GetPerkBoolValue(SyberiaPerkType.SYBPERK_STEALTH_TWOHANDED_CRIT) && weapon.IsTwoHandedBehaviour())
			{
				m_HitType = EMeleeHitType.WPN_STAB;
				int result = super.GetFinisherType(weapon, target);
				if (result != -1)
				{
					return EMeleeHitType.WPN_STAB_FINISHER;
				}
			}
			else if (player.GetPerkBoolValue(SyberiaPerkType.SYBPERK_STEALTH_KNIFE_CRIT))
			{
				return super.GetFinisherType(weapon, target);
			}
		}
		
		return -1;
	}
}