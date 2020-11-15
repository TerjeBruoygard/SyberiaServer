modded class BleedingSourcesManagerServer
{
	void AddBulletHit()
	{
		m_Player.m_bulletHits = m_Player.m_bulletHits + 1;
		m_Player.SetSynchDirty();
	}
	
	void RemoveBulletHit()
	{
		if (m_Player.m_bulletHits > 0)
		{
			m_Player.m_bulletHits = m_Player.m_bulletHits - 1;
			m_Player.SetSynchDirty();
		}
	}
	
	void AddKnifeHit()
	{
		m_Player.m_knifeHits = m_Player.m_knifeHits + 1;
		m_Player.SetSynchDirty();
	}
	
	void RemoveKnifeHit()
	{
		if (m_Player.m_knifeHits > 0)
		{
			m_Player.m_knifeHits = m_Player.m_knifeHits - 1;
			m_Player.SetSynchDirty();
		}
	}
	
	void AddHematomaHit()
	{
		m_Player.m_hematomaHits = m_Player.m_hematomaHits + 1;
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
	
	void SetVisceraHit(bool value)
	{
		m_Player.m_visceraHit = value;
		m_Player.SetSynchDirty();
	}
	
	void SetConcussionHit(bool value)
	{
		m_Player.m_concussionHit = value;
		m_Player.SetSynchDirty();
	}
	
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		SybLogSrv("ProcessHit " + m_Player.GetIdentity().GetName() + "; Damage: " + damage + "; Source: " + source.GetType() + "; Component: " + component + "; Zone: " + zone + "; Ammo: " + ammo);
		
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
			if (zone == "Head")
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
					SetVisceraHit(true);
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
		else
		{
			SybLogSrv("Unknown ammo type detected: " + ammoType + " (" + ammo + ")");
		}
	};
};