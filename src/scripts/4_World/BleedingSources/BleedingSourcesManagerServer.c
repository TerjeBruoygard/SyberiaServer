modded class BleedingSourcesManagerServer
{
	void SetZVirus(bool value)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (value)
			{
				if (m_Player.GetSybStats().m_zombieVirus == 0)
				{
					m_Player.GetSybStats().m_zombieVirus = 1;
				}
				else if (m_Player.GetSybStats().m_zombieVirus == 2)
				{
					m_Player.m_zvirusTimer = m_Player.m_zvirusTimer + GetSyberiaConfig().m_zvirusOnHitIncrementTimer;
				}
			}
			else
			{
				if (m_Player.GetSybStats().m_zombieVirus > 0)
				{
					m_Player.AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityZVirus);
				}
				
				m_Player.GetSybStats().m_zombieVirus = 0;
			}
			m_Player.MarkSybStatsDirty(1000);
		}
	}
		
	void SetBloodInfection(bool value)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (value)
			{
				if (m_Player.GetSybStats().m_sepsis == 0)
				{
					m_Player.GetSybStats().m_sepsis = 1;
				}
			}
			else
			{
				if (m_Player.GetSybStats().m_sepsis > 0)
				{
					m_Player.AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunitySepsis);
				}
				
				m_Player.GetSybStats().m_sepsis = 0;
			}
			m_Player.MarkSybStatsDirty(1001);
		}
	}
	
    private void ApplyBandageDammageHands()
    {
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			ItemBase gloves = m_Player.GetItemOnSlot("Gloves");
			if (gloves)
			{
				gloves.SetCleanness(0);
			}
			else
			{
				m_Player.SetBloodyHands(true);
			}
		}
    }
    
	void ApplyBandage(int level)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetSybStats().m_bulletHits > (m_Player.GetSybStats().m_bulletBandage1 + m_Player.GetSybStats().m_bulletBandage2))
			{
				if (level == 2)
				{
					ApplyBandageDammageHands();
					m_Player.GetSybStats().m_bulletBandage1 = m_Player.GetSybStats().m_bulletBandage1 + 1;
					m_Player.MarkSybStatsDirty(1002);
					return;
				}
				if (level == 3)
				{
					ApplyBandageDammageHands();
					m_Player.GetSybStats().m_bulletBandage2 = m_Player.GetSybStats().m_bulletBandage2 + 1;
					m_Player.MarkSybStatsDirty(1003);
					return;
				}
			}
			
			if (m_Player.GetSybStats().m_knifeHits > (m_Player.GetSybStats().m_knifeBandage1 + m_Player.GetSybStats().m_knifeBandage2))
			{
				if (level == 2)
				{
					ApplyBandageDammageHands();
					m_Player.GetSybStats().m_knifeBandage1 = m_Player.GetSybStats().m_knifeBandage1 + 1;
					m_Player.MarkSybStatsDirty(1004);
					return;
				}
				if (level == 3)
				{
					ApplyBandageDammageHands();
					m_Player.GetSybStats().m_knifeBandage2 = m_Player.GetSybStats().m_knifeBandage2 + 1;
					m_Player.MarkSybStatsDirty(1005);
					return;
				}
			}
			
			if (level > 0)
			{
				RemoveMostSignificantBleedingSource();
				return;
			}
		}
	}
	
	void AddBulletHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			m_Player.GetSybStats().m_bulletHits = m_Player.GetSybStats().m_bulletHits + 1;
			SetPainLevel(2);
			m_Player.MarkSybStatsDirty(1006);
		}
	}
	
	void RemoveBulletHit(bool removeBandage)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetSybStats().m_bulletHits > 0)
			{
				m_Player.GetSybStats().m_bulletHits = m_Player.GetSybStats().m_bulletHits - 1;
				if (removeBandage)
				{
					if (m_Player.GetSybStats().m_bulletBandage1 > 0) m_Player.GetSybStats().m_bulletBandage1 = m_Player.GetSybStats().m_bulletBandage1 - 1;
					else if (m_Player.GetSybStats().m_bulletBandage2 > 0) m_Player.GetSybStats().m_bulletBandage2 = m_Player.GetSybStats().m_bulletBandage2 - 1;
				}
				m_Player.MarkSybStatsDirty(1007);
			}
		}
	}
	
	void AddKnifeHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			m_Player.GetSybStats().m_knifeHits = m_Player.GetSybStats().m_knifeHits + 1;
			SetPainLevel(2);
			m_Player.MarkSybStatsDirty(1008);
		}
	}
	
	void RemoveKnifeHit(bool removeBandage)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetSybStats().m_knifeHits > 0)
			{
				m_Player.GetSybStats().m_knifeHits = m_Player.GetSybStats().m_knifeHits - 1;
				if (removeBandage)
				{
					if (m_Player.GetSybStats().m_knifeBandage1 > 0) m_Player.GetSybStats().m_knifeBandage1 = m_Player.GetSybStats().m_knifeBandage1 - 1;
					else if (m_Player.GetSybStats().m_knifeBandage2 > 0) m_Player.GetSybStats().m_knifeBandage2 = m_Player.GetSybStats().m_knifeBandage2 - 1;
				}
				m_Player.MarkSybStatsDirty(1009);
			}
		}
	}
	
	void AddHematomaHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_HEMATOMA_CHANCE, 0, 0) )
				return;
			
			m_Player.GetSybStats().m_hematomaHits = m_Player.GetSybStats().m_hematomaHits + 1;
			
			if (Math.RandomFloat01() < GetSyberiaConfig().m_hematomaPainChance)
			{
				SetPainLevel(1);
			}
			
			m_Player.MarkSybStatsDirty(1010);
		}
	}
	
	void RemoveHematomaHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetSybStats().m_hematomaHits > 0)
			{
				m_Player.GetSybStats().m_hematomaHits = m_Player.GetSybStats().m_hematomaHits - 1;
				m_Player.MarkSybStatsDirty(1011);
			}
		}
	}
	
	void AddVisceraHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			m_Player.GetSybStats().m_visceraHit = m_Player.GetSybStats().m_visceraHit + 1;
			SetPainLevel(3);
			m_Player.MarkSybStatsDirty(1012);
		}
	}
	
	void RemoveVisceraHit()
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetSybStats().m_visceraHit > 0)
			{
				m_Player.GetSybStats().m_visceraHit = m_Player.GetSybStats().m_visceraHit - 1;
				m_Player.MarkSybStatsDirty(1013);
			}
		}
	}
	
	void SetConcussionHit(bool value, bool uncounsion = true)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			m_Player.GetSybStats().m_concussionHit = value;
			if (value)
			{
				if (uncounsion)
				{
					m_Player.m_UnconsciousEndTime = -60;
					m_Player.SetHealth("","Shock",0);
				}
				SetPainLevel(1);
			}
			m_Player.SetSynchDirty();
			m_Player.MarkSybStatsDirty(1014);
		}
	}
	
	int GetPainLevel()
	{
		int painLevel = 0;
		if(m_Player)
		painLevel = m_Player.GetSybStats().m_painLevel;

		return painLevel;
	}
	
	void SetPainLevel(int value)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			if (m_Player.GetPerkBoolValue(SyberiaPerkType.SYBPERK_IMMUNITY_PAIN_STRONG))
			{
				value = value - 1;
			}
			
			if (value == 1) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl1TimeSec;
			else if (value == 2) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl2TimeSec;
			else if (value == 3) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl3TimeSec;
			
			if (m_Player.GetSybStats().m_painLevel < value)
			{
				m_Player.GetSybStats().m_painLevel = value;
				m_Player.MarkSybStatsDirty(1015);
			}
		}
	}
	
	override bool AttemptAddBleedingSource(int component)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return false;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return false;
			}
			#endif
			if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_CUTHIT_CHANCE, 0, 0) )
				return false;
		}		
		return super.AttemptAddBleedingSource(component);
	}
	
	override bool AttemptAddBleedingSourceBySelection(string selection_name)
	{
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return false;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return false;
			}
			#endif
			if (selection_name == "RightFoot" || selection_name == "LeftFoot")
			{
				if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_SURVIVAL_CUTHIT_LEGS_DEC, 0, 0) )
				{
					return false;
				}
			}
			else if (selection_name == "LeftForeArmRoll" || selection_name == "RightForeArmRoll")
			{
				if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_SURVIVAL_CUTHIT_HANDS_DEC, 0, 0) )
				{
					return false;
				}
				
				m_Player.SetBloodyHands(true);
			}
		}
		
		return super.AttemptAddBleedingSourceBySelection(selection_name);
	}
	
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		//SybLogSrv("ProcessHit => Damage: " + damage + "; Source: " + source.GetType() + "; Component: " + component + "; Zone: " + zone + "; Ammo: " + ammo);
		if (m_Player && m_Player.IsAlive())
		{
			#ifdef EXPANSIONMODAI
			if (m_Player.IsAI())
			{
				return;
			}
			#endif
			#ifdef PersonBotsib_serv
			if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
			{
				return;
			}
			#endif
			float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );		
			string ammoType = GetGame().ConfigGetTextOut( "CfgAmmo " + ammo + " DamageApplied " + "type" );
			bleed_threshold = Math.Clamp(bleed_threshold,0,1);
			
			float meleeHeadProtection = 0;
			ItemBase helmet = m_Player.GetItemOnSlot("Headgear");
			if (helmet)
			{
				meleeHeadProtection = helmet.ConfigGetFloat("meleeProtection");
			}
			
			if (source.IsZombie())
			{
				if (!m_Player.IsStayingInBlock())
				{
					bool blockZedDamage = false;
					if (zone == "Torso")
					{
						ItemBase itemCheckZedVest = m_Player.GetItemOnSlot("Vest");
						if (itemCheckZedVest && !itemCheckZedVest.IsRuined())
						{
							float plateArmorZed = GetGame().ConfigGetFloat( "CfgVehicles " + itemCheckZedVest.GetType() + " DamageSystem GlobalArmor Melee Blood damage" );
							if (plateArmorZed < 0.1)
							{
								blockZedDamage = true;
							}
						}
					}
					
					float zvirusInfectionChance = GetSyberiaConfig().m_zvirusZombieHitChance;
					float zedHematomaChance = GetSyberiaConfig().m_hematomaZombieHitChance;
					if (blockZedDamage)
					{
						zedHematomaChance = zedHematomaChance * 0.5;
					}
					
					if (!blockZedDamage && Math.RandomFloat01() < GetSyberiaConfig().m_bleedingZombieHitChance)
					{
						zvirusInfectionChance = zvirusInfectionChance * 1.5;
						AttemptAddBleedingSource(component);
						if (Math.RandomFloat01() < GetSyberiaConfig().m_sepsisZombieHitChance)
						{
							SetBloodInfection(true);
						}
					}
					else if (Math.RandomFloat01() < zedHematomaChance)
					{
						AddHematomaHit();
						if (zone == "Head" && Math.RandomFloat01() < 0.3 && Math.RandomFloat01() > meleeHeadProtection)
						{
							SetConcussionHit(true, false);
						}
						
						if (m_Player.IsFaceBlocked(false))
						{
							zvirusInfectionChance = zvirusInfectionChance * 0.5;
						}
					}
					
					if (Math.RandomFloat01() < zvirusInfectionChance)
					{
						SetZVirus(true);
					}
					
					if (!blockZedDamage && Math.RandomFloat01() < GetSyberiaConfig().m_bleedingKnifehitZombieChance)
					{
						AddKnifeHit();
					}
					
					if (Math.RandomFloat01() < GetSyberiaConfig().m_concussionZombieHitChance && Math.RandomFloat01() > meleeHeadProtection)
					{
						SetConcussionHit(true, false);
					}
				}
				
				m_Player.AddMindDegradation( GetSyberiaConfig().m_zombieHitDecreaseMind[0], GetSyberiaConfig().m_zombieHitDecreaseMind[1] );
			}
			else if ( ammo.ToType().IsInherited(Nonlethal_Base) )
			{
				AddHematomaHit();
			}
			else if ( ammo == "BearTrapHit" )
			{
				AddKnifeHit();
			}
			else if ( ammo == "BarbedWireHit" )
			{
				if (Math.RandomFloat01() < 0.2)
				{
					AttemptAddBleedingSource(component);
					
					if (Math.RandomFloat01() < 0.05)
					{
						SetBloodInfection(true);
					}
				}
			}
			else if (ammoType == "Melee")
			{
				if (!m_Player.IsStayingInBlock())
				{
					bool blockMeleeDamage = false;
					if (zone == "Torso")
					{
						ItemBase itemCheckMeleVest = m_Player.GetItemOnSlot("Vest");
						if (itemCheckMeleVest && !itemCheckMeleVest.IsRuined())
						{
							float plateArmorMele = GetGame().ConfigGetFloat( "CfgVehicles " + itemCheckMeleVest.GetType() + " DamageSystem GlobalArmor Melee Blood damage" );
							if (plateArmorMele < 0.1)
							{
								blockMeleeDamage = true;
							}
						}
					}
					
					float affectSkeleton = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " affectSkeleton" );
					if (affectSkeleton > 1 && !ammo.Contains("Axe"))
					{
						AddHematomaHit();
						if (zone == "Head" && Math.RandomFloat01() > meleeHeadProtection)
						{
							SetConcussionHit(true);
						}
					}
					else if (!blockMeleeDamage) 
					{
						if (bleed_threshold >= Math.RandomFloat01())
						{
							if (source.IsAnimal() || ammo.Contains("_Heavy") || Math.RandomFloat01() >= 0.4)
							{
								AddKnifeHit();
								if (zone == "Torso" && Math.RandomFloat01() < GetSyberiaConfig().m_visceraKnifehitTorsoChance)
								{
									AddVisceraHit();
								}
							}
							else
							{
								AttemptAddBleedingSource(component);
							}
							
							if (Math.RandomFloat01() < GetSyberiaConfig().m_sepsisKnifeHitChance)
							{
								SetBloodInfection(true);
							}
						}
						else
						{
							if (Math.RandomFloat01() < GetSyberiaConfig().m_hematomaPlayerhandsHitChance)
							{
								AddHematomaHit();
							}
						}
					}
				}
			}
			else if (ammoType == "Projectile")
			{
				bool isBulletStopped = false;
				if (zone == "Head" || zone == "Brain")
				{
					SetConcussionHit(true);

					ItemBase itemCheckH = m_Player.GetItemOnSlot("Headgear");
					if (itemCheckH && !itemCheckH.IsRuined())
					{
						float distanceModH = Math.Clamp(1300 - vector.Distance(source.GetPosition(), m_Player.GetPosition()), 100, 1000) * 0.001;
						float bulletSpeedH = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " typicalSpeed" ) * 0.1;
						float bulletCaliberH = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " caliber" );
						float bulletWeightH = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " weight" );
						float plateArmorH = GetGame().ConfigGetFloat( "CfgVehicles " + itemCheckH.GetType() + " firearmProtection" );
						float injectModifierH = (bulletSpeedH * bulletCaliberH * bulletWeightH * distanceModH) * GetSyberiaConfig().m_bodyGuardModifier;
						//SybLogSrv("BODY ARMOR VEST PROJECTILE GUARD TEST: " + plateArmor + " / " + injectModifier);
						if (plateArmorH > injectModifierH)
						{
							// Bullet stoped by armor
							isBulletStopped = true;
						}
					}
				}
				if (zone == "Torso")
				{
					ItemBase itemCheckV = m_Player.GetItemOnSlot("Vest");
					if (itemCheckV && !itemCheckV.IsRuined())
					{
						float distanceModV = Math.Clamp(1300 - vector.Distance(source.GetPosition(), m_Player.GetPosition()), 100, 1000) * 0.001;
						float bulletSpeedV = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " typicalSpeed" ) * 0.1;
						float bulletCaliberV = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " caliber" );
						float bulletWeightV = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " weight" );
						float plateArmorV = GetGame().ConfigGetFloat( "CfgVehicles " + itemCheckV.GetType() + " bulletProofProtection" );
						float injectModifierV = (bulletSpeedV * bulletCaliberV * bulletWeightV * distanceModV) * GetSyberiaConfig().m_bodyGuardModifier;
						//SybLogSrv("BODY ARMOR VEST PROJECTILE GUARD TEST: " + plateArmor + " / " + injectModifier);
						if (plateArmorV > injectModifierV)
						{
							// Bullet stoped by armor
							isBulletStopped = true;
						}
					}
					
					if (!isBulletStopped && Math.RandomFloat01() < GetSyberiaConfig().m_visceraBullethitTorsoChance)
					{
						AddVisceraHit();
					}
				}
				
				if (isBulletStopped)
				{
					AddHematomaHit();
				}
				else if (bleed_threshold >= Math.RandomFloat01())
				{
					AddBulletHit();
				}
				else
				{
					AttemptAddBleedingSource(component);
				}
				
				if (!isBulletStopped)
				{
					if (Math.RandomFloat01() < GetSyberiaConfig().m_sepsisBulletHitChance)
					{
						SetBloodInfection(true);
					}
				}
			}
			else if (ammoType == "FragGrenade")
			{
				SetConcussionHit(true);
			}
		}
	};
};