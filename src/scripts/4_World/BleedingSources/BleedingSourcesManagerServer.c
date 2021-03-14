modded class BleedingSourcesManagerServer
{
	void SetZVirus(bool value)
	{
		if (value)
		{
			if (m_Player.m_zombieVirus == 0)
			{
				m_Player.m_zombieVirus = 1;
			}
		}
		else
		{
			if (m_Player.m_zombieVirus > 0)
			{
				m_Player.AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityZVirus);
			}
			
			m_Player.m_zombieVirus = 0;
		}
		m_Player.SetSynchDirty();
	}
		
	void SetBloodInfection(bool value)
	{
		if (value)
		{
			if (m_Player.m_sepsis == 0)
			{
				m_Player.m_sepsis = 1;
			}
		}
		else
		{
			if (m_Player.m_sepsis > 0)
			{
				m_Player.AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunitySepsis);
			}
			
			m_Player.m_sepsis = 0;
		}
		m_Player.SetSynchDirty();
	}
	
	void ApplyBandage(int level)
	{
		if (m_Player.m_bulletHits > (m_Player.m_bulletBandage1 + m_Player.m_bulletBandage2))
		{
			if (level == 2)
			{
				if (!m_Player.HasGloves()) 
					m_Player.SetBloodyHands(true);
				
				m_Player.m_bulletBandage1 = m_Player.m_bulletBandage1 + 1;
				m_Player.SetSynchDirty();
				return;
			}
			if (level == 3)
			{
				if (!m_Player.HasGloves()) 
					m_Player.SetBloodyHands(true);
				
				m_Player.m_bulletBandage2 = m_Player.m_bulletBandage2 + 1;
				m_Player.SetSynchDirty();
				return;
			}
		}
		
		if (m_Player.m_knifeHits > (m_Player.m_knifeBandage1 + m_Player.m_knifeBandage2))
		{
			if (level == 2)
			{
				if (!m_Player.HasGloves()) 
					m_Player.SetBloodyHands(true);
				
				m_Player.m_knifeBandage1 = m_Player.m_knifeBandage1 + 1;
				m_Player.SetSynchDirty();
				return;
			}
			if (level == 3)
			{
				if (!m_Player.HasGloves()) 
					m_Player.SetBloodyHands(true);
				
				m_Player.m_knifeBandage2 = m_Player.m_knifeBandage2 + 1;
				m_Player.SetSynchDirty();
				return;
			}
		}
		
		if (level > 0)
		{
			RemoveMostSignificantBleedingSource();
			return;
		}
	}
	
	void AddBulletHit()
	{
		m_Player.m_bulletHits = m_Player.m_bulletHits + 1;
		SetPainLevel(2);
		m_Player.SetSynchDirty();
	}
	
	void RemoveBulletHit(bool removeBandage)
	{
		if (m_Player.m_bulletHits > 0)
		{
			m_Player.m_bulletHits = m_Player.m_bulletHits - 1;
			if (removeBandage)
			{
				if (m_Player.m_bulletBandage1 > 0) m_Player.m_bulletBandage1 = m_Player.m_bulletBandage1 - 1;
				else if (m_Player.m_bulletBandage2 > 0) m_Player.m_bulletBandage2 = m_Player.m_bulletBandage2 - 1;
			}
			m_Player.SetSynchDirty();
		}
	}
	
	void AddKnifeHit()
	{
		m_Player.m_knifeHits = m_Player.m_knifeHits + 1;
		SetPainLevel(2);
		m_Player.SetSynchDirty();
	}
	
	void RemoveKnifeHit(bool removeBandage)
	{
		if (m_Player.m_knifeHits > 0)
		{
			m_Player.m_knifeHits = m_Player.m_knifeHits - 1;
			if (removeBandage)
			{
				if (m_Player.m_knifeBandage1 > 0) m_Player.m_knifeBandage1 = m_Player.m_knifeBandage1 - 1;
				else if (m_Player.m_knifeBandage2 > 0) m_Player.m_knifeBandage2 = m_Player.m_knifeBandage2 - 1;
			}
			m_Player.SetSynchDirty();
		}
	}
	
	void AddHematomaHit()
	{
		if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_HEMATOMA_CHANCE, 0, 0) )
			return;
		
		m_Player.m_hematomaHits = m_Player.m_hematomaHits + 1;
		
		if (Math.RandomFloat01() < GetSyberiaConfig().m_hematomaPainChance)
		{
			SetPainLevel(1);
		}
		
		m_Player.SetSynchDirty();
	}
	
	void RemoveHematomaHit()
	{
		if (m_Player.m_hematomaHits > 0)
		{
			m_Player.m_hematomaHits = m_Player.m_hematomaHits - 1;
			m_Player.SetSynchDirty();
		}
	}
	
	void AddVisceraHit()
	{
		m_Player.m_visceraHit = m_Player.m_visceraHit + 1;
		SetPainLevel(3);
		m_Player.SetSynchDirty();
	}
	
	void RemoveVisceraHit()
	{
		if (m_Player.m_visceraHit > 0)
		{
			m_Player.m_visceraHit = m_Player.m_visceraHit - 1;
			m_Player.SetSynchDirty();
		}
	}
	
	void SetConcussionHit(bool value)
	{
		m_Player.m_concussionHit = value;
		if (value)
		{
			m_Player.m_UnconsciousEndTime = -60;
			m_Player.SetHealth("","Shock",0);
			SetPainLevel(1);
		}
		m_Player.SetSynchDirty();
	}
	
	void SetPainLevel(int value)
	{
		if (m_Player.GetPerkBoolValue(SyberiaPerkType.SYBPERK_IMMUNITY_PAIN_STRONG))
		{
			value = value - 1;
		}
		
		if (value == 1) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl1TimeSec;
		else if (value == 2) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl2TimeSec;
		else if (value == 3) m_Player.m_painTimer = m_Player.m_painTimer + GetSyberiaConfig().m_painLvl3TimeSec;
		
		if (m_Player.m_painLevel < value)
		{
			m_Player.m_painLevel = value;
			m_Player.SetSynchDirty();
		}
	}
	
	override bool AttemptAddBleedingSource(int component)
	{
		if ( Math.RandomFloat01() < m_Player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_CUTHIT_CHANCE, 0, 0) )
			return false;
		
		return super.AttemptAddBleedingSource(component);
	}
	
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		//SybLogSrv("ProcessHit " + m_Player.GetIdentity().GetName() + "; Damage: " + damage + "; Source: " + source.GetType() + "; Component: " + component + "; Zone: " + zone + "; Ammo: " + ammo);
		
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );		
		string ammoType = GetGame().ConfigGetTextOut( "CfgAmmo " + ammo + " DamageApplied " + "type" );
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		
		if (source.IsZombie())
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
				if (zone == "Head" && Math.RandomFloat01() < 0.3)
				{
					SetConcussionHit(true);
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
		}
		else if ( ammo.ToType().IsInherited(Nonlethal_Base) )
		{
			AddHematomaHit();
		}
		else if ( ammo == "BearTrapHit" )
		{
			AddKnifeHit();
		}
		else if (ammoType == "Melee")
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
				if (zone == "Head")
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
		else if (ammoType == "Projectile")
		{
			if (zone == "Head" || zone == "Brain")
			{
				SetConcussionHit(true);
			}
			
			bool isBulletStopped = false;
			if (zone == "Torso")
			{
				ItemBase itemCheck = m_Player.GetItemOnSlot("Vest");
				if (itemCheck && !itemCheck.IsRuined())
				{
					float bulletSpeed = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " typicalSpeed" );
					float bulletCaliber = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " caliber" );
					float plateArmor = GetGame().ConfigGetFloat( "CfgVehicles " + itemCheck.GetType() + " DamageSystem GlobalArmor Projectile Health damage" );
					float injectModifier = (bulletSpeed * bulletCaliber) * 0.001;
					if (plateArmor > injectModifier)
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
	};
};