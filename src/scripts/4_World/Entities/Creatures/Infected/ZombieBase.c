modded class ZombieBase extends DayZInfected
{
	float m_doorsCheckTimer;
	
	override void Init()
	{
		super.Init();
		m_doorsCheckTimer = 0;
	}
	
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (GetSyberiaConfig().m_zombieOpenDoorEnable && m_ActualTarget)
		{
			m_doorsCheckTimer = m_doorsCheckTimer + pDt;		
			if (m_doorsCheckTimer > 1.5)
			{
				m_doorsCheckTimer = 0;
				HandleHouseDoors();
			}
		}
		
		return false;
	}
	
	private void HandleHouseDoors()
	{
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition(GetPosition(), GetSyberiaConfig().m_zombieFindHouseDistance, objects, NULL);

		if (objects.Count() > 0)
		{
			foreach (Object obj : objects)
			{
				Building building;
				if( Building.CastTo(building, obj) && building.GetType().Contains("Wreck") == false )
				{
					HandleBuilding(building);
				}
			}
		}
	}
	
	private void HandleBuilding(Building building)
	{
		int doorsCount = GetGame().ConfigGetChildrenCount("CfgVehicles " + building.GetType() + " Doors");
		for (int i = 0; i < doorsCount; i++)
		{
			if ( !building.IsDoorOpen(i) && !building.IsDoorLocked(i))
			{
				vector doorPos = building.GetDoorSoundPos(i);
				if (vector.Distance(doorPos, GetPosition()) < GetSyberiaConfig().m_zombieOpenDoorDistance)
				{
					building.OpenDoor(i);
					EntityAI soundFx = EntityAI.Cast( GetGame().CreateObject("ZombieDoorBreak" + Math.RandomIntInclusive(1, 5) + "_SoundEmitter", doorPos) );
					if (soundFx)
					{
						int anim_type = Math.RandomIntInclusive(9,13);
						StartCommand_Attack(soundFx, anim_type, anim_type);
					}	
				}
			}
		}
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (damageResult != null && source != null)
		{			
			PlayerBase sourcePlayer = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
			if (sourcePlayer)
			{
				string ammoType = GetGame().ConfigGetTextOut( "CfgAmmo " + ammo + " DamageApplied " + "type" );
				if (ammoType == "Melee")
				{
					float additionalDmg = damageResult.GetDamage(dmgZone, "Health");
					if ( ammo.Contains("_Heavy") )
					{
						additionalDmg = additionalDmg * sourcePlayer.GetPerkFloatValue(SyberiaPerkType.SYBPERK_STRENGTH_HEAVY_ATTACK_STRENGTH, 0, 0);
						
						if (dmgZone == "Head" && sourcePlayer.GetPerkBoolValue(SyberiaPerkType.SYBPERK_STRENGTH_KNOCKOUT_HEAVY_ITEMS))
						{
							SetHealth("", "Health", 0);
						}
						
						if (!sourcePlayer.HasVisibleZVirus() && sourcePlayer.GetItemInHands() && Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthHeavyAttackChance)
						{
							sourcePlayer.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthHeavyAttackValue);
						}
					}
					else
					{
						additionalDmg = additionalDmg * sourcePlayer.GetPerkFloatValue(SyberiaPerkType.SYBPERK_STRENGTH_FAST_ATTACK_STRENGTH, 0, 0);
						
						if (!sourcePlayer.HasVisibleZVirus() && sourcePlayer.GetItemInHands() && Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthLightAttackChance)
						{
							sourcePlayer.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthLightAttackValue);
						}
					}
					
					if (additionalDmg > 0)
					{
						DecreaseHealth("", "Health", additionalDmg);
					}
				}
			}
		}
		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
				
		EntityAI entityKiller = EntityAI.Cast(killer);
		if (entityKiller)
		{
			PlayerBase killerPlayer = PlayerBase.Cast(entityKiller.GetHierarchyRootPlayer());
			if (killerPlayer)
			{
				killerPlayer.AddMindDegradation( GetSyberiaConfig().m_zombieKillDecreaseMind[0], GetSyberiaConfig().m_zombieKillDecreaseMind[1] );
			}
		}
	}
	
	override bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_MOVE)
		{
			if ( m_ActualAttackType )
			{
				m_ActualAttackType.m_IsHeavy = 0;
				
				int mindState = pInputController.GetMindState();
				if (mindState == DayZInfectedConstants.MINDSTATE_CHASE)
				{
					m_ActualAttackType.m_Distance = GetSyberiaConfig().m_zombieAttackDistanceChase;
				}
				else if (mindState == DayZInfectedConstants.MINDSTATE_FIGHT)
				{
					m_ActualAttackType.m_Distance = GetSyberiaConfig().m_zombieAttackDistanceFight;
				}
			}
		}
		else if ( pCurrentCommandID == DayZInfectedConstants.COMMANDID_ATTACK )
		{
			if ( m_ActualAttackType )
			{
				m_ActualAttackType.m_IsHeavy = 0;
				m_ActualAttackType.m_Distance = GetSyberiaConfig().m_zombieAttackDistanceFight;
			}
			else
			{
				return true;
			}
		}
		
		return super.FightLogic(pCurrentCommandID, pInputController, pDt);
	}
};