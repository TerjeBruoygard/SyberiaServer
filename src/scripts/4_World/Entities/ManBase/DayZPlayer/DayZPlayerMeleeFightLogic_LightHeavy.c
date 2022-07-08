modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override protected void DamageHands(DayZPlayer DZPlayer, ClothingBase gloves, InventoryItem itemInHands)
	{
		EntityAI target = m_MeleeCombat.GetTargetEntity();
		
		//We did not hit anything
		if ( !target )
			return;
		
		//Check if server side
		if ( DZPlayer.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER )
		{
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
		DayZInfected targetInfected = DayZInfected.Cast(target);
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		vector hitPosWS;
		
		//! check and evaluate stealth kill
		if (GetGame().IsServer() && targetInfected && weapon)
		{
			//! perform only for finisher suitable weapons
			if ((weapon.IsMeleeFinisher() || m_HitType == EMeleeHitType.WPN_STAB) &&  weapon.GetHealthLevel("") !=  GameConstants.STATE_RUINED)
			{
				int mindState = targetInfected.GetInputController().GetMindState();
				
				//Check if the infected is aware of the player
				if ((mindState != DayZInfectedConstants.MINDSTATE_CHASE) && (mindState != DayZInfectedConstants.MINDSTATE_FIGHT))
				{
					//! check if attacker is in right pos and angle against victim
					if (IsBehindEntity(60, m_DZPlayer, target))
					{
						EntityAI targetEntity = EntityAI.Cast(target);
	
						//! translate idx to name
						string compName = targetEntity.GetDamageZoneNameByComponentIndex(hitZoneIdx);
		
						// execute attack (dmg part)
						target.AddHealth( "","Health", -target.GetMaxHealth("","") );
						
						// Add strength skill
						PlayerBase player = PlayerBase.Cast(DZPlayer);
						if (player && Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthSilentAttackChance)
						{
							player.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthSilentAttackValue);
						}
						
						return true;
					}
				}
			}
		}
		//Finisher attack not performed
		return false;
	}
}