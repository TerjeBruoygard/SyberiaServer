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
			if (player && Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthSilentAttackChance)
			{
				player.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthSilentAttackValue);
			}
		}
		
		return result;
	}
}