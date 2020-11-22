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
			m_Player.m_sepsis = 0;
		}
		m_Player.SetSynchDirty();
	}
	
	void ApplyBandage(int level)
	{		
		if (m_Player.m_bulletHits > 0)
		{
			if (level == 2)
			{
				m_Player.m_bulletBandage1 = m_Player.m_bulletBandage1 + 1;
				m_Player.SetSynchDirty();
				return;
			}
			if (level == 3)
			{
				m_Player.m_bulletBandage2 = m_Player.m_bulletBandage2 + 1;
				m_Player.SetSynchDirty();
				return;
			}
		}
		
		if (m_Player.m_knifeHits > 0)
		{
			if (level == 2)
			{
				m_Player.m_knifeBandage1 = m_Player.m_knifeBandage1 + 1;
				m_Player.SetSynchDirty();
				return;
			}
			if (level == 3)
			{
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
		m_Player.m_hematomaHits = m_Player.m_hematomaHits + 1;
		SetPainLevel(1);
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
		if (value == 1) m_Player.m_painTimer = m_Player.m_painTimer + PAIN_LVL1_TIME_SEC;
		else if (value == 2) m_Player.m_painTimer = m_Player.m_painTimer + PAIN_LVL2_TIME_SEC;
		else if (value == 3) m_Player.m_painTimer = m_Player.m_painTimer + PAIN_LVL3_TIME_SEC;
		
		if (m_Player.m_painLevel < value)
		{
			m_Player.m_painLevel = value;
			m_Player.SetSynchDirty();
		}
	}
	
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		//SybLogSrv("ProcessHit " + m_Player.GetIdentity().GetName() + "; Damage: " + damage + "; Source: " + source.GetType() + "; Component: " + component + "; Zone: " + zone + "; Ammo: " + ammo);
		
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );		
		string ammoType = GetGame().ConfigGetTextOut( "CfgAmmo " + ammo + " DamageApplied " + "type" );
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		
		if (source.IsZombie())
		{
			if (Math.RandomFloat01() < 0.3)
			{
				AttemptAddBleedingSource(component);
			}
			else
			{
				AddHematomaHit();
				if (zone == "Head" && Math.RandomFloat01() < 0.1)
				{
					SetConcussionHit(true);
				}
			}
			
			if (Math.RandomFloat01() < ZVIRUS_ZOMBIE_HIT_CHANCE)
			{
				SetZVirus(true);
			}
		}
		else if (ammoType == "Melee")
		{
			float affectSkeleton = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " affectSkeleton" );
			
			if (affectSkeleton > 1 && !ammo.Contains("Axe"))
			{
				AddHematomaHit();
				if (zone == "Head")
				{
					SetConcussionHit(true);
				}
			}
			else if (bleed_threshold >= Math.RandomFloat01())
			{
				if (ammo.Contains("_Heavy") || Math.RandomFloat01() >= 0.3)
				{
					AddKnifeHit();
					if (zone == "Torso")
					{
						AddVisceraHit();
					}
				}
				else
				{
					AttemptAddBleedingSource(component);
				}
			}
			else
			{
				AttemptAddBleedingSource(component);
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
				
				if (!isBulletStopped)
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
		}
		else if (ammoType == "FragGrenade")
		{
			AttemptAddBleedingSource(component);
			SetConcussionHit(true);
		}
	};
};