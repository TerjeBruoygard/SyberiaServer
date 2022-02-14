modded class PlayerBase
{	
	ref CharProfile m_charProfile;
	
	// Stats
	bool m_sybstatsDirty;
	float m_sybstatTimer;
	
	// Sleeping
	float m_sleepingDecTimer;
	float m_sleepingBoostTimer;
	int m_sleepingBoostValue;
	int m_sleepingSoundTimer;
	
	// Adv med
	float m_advMedUpdateTimer;
	float m_painTimer;
	float m_painkillerTime;
	float m_hematomaRegenTimer;
	float m_cuthitRegenTimer;
	float m_stomatchhealTimer;
	bool m_hemologicShock;
	float m_sepsisTime;
	float m_zvirusTimer;
	float m_bullethitRegenTimer;
	float m_knifehitRegenTimer;
	float m_concussionRegenTimer;
	float m_bloodHemostaticTimer;
	float m_hematopoiesisTimer;
	float m_salveEffectTimer;
	float m_adrenalinEffectTimer;
	float m_antibioticsTimer;
	float m_antibioticsStrange;
	float m_influenzaTimer;
	float m_stomatchpoisonTimer;
	float m_radioprotectionTimer;
	float m_antidepresantTimer;
	float m_lastStomatchpoisonHeal;
	float m_overdosedValue;	
	
	// Mind state
	float m_mindDegradationForce;
	float m_mindDegradationTime;
	
	// Admin tool
	bool m_freeCamMode = false;
	
	// Skills
	int m_skillsSaveInterval = 0;
	bool m_skillsSaveDirty = false;
	float m_sprintingTime = 0;
	float m_jogingTime = 0;
	float m_crouchTime = 0;
	vector m_skillsRunLastPos;
	vector m_skillsCrouchLastPos;
	int m_fireplaceLastIgnition = 0;
	
	// Zones
	float m_zoneGasTotalValue = 0;
	float m_zoneRadTotalValue = 0;
	float m_zonePsiTotalValue = 0;
	int m_zoneToxicEffect = 0;
	float m_radiationDose = 0;
	bool m_isAlreadyDead = false;
	float m_lastHealth;
	float m_lastBlood;
	float m_lastShock;
	
	override void Init()
	{
		super.Init();
		
		// Stats
		m_sybstatsDirty = false;
		m_sybstatTimer = -1.5;
		
		// Sleeping
		m_sleepingDecTimer = 0;
		m_sleepingBoostTimer = 0;
		m_sleepingBoostValue = 0;
		m_sleepingSoundTimer = 0;
		
		// Adv Med
		m_advMedUpdateTimer = 0;
		m_painTimer = 0;
		m_hematomaRegenTimer = 0;
		m_cuthitRegenTimer = 0;
		m_painkillerTime = 0;
		m_stomatchhealTimer = 0;
		m_hemologicShock = false;
		m_sepsisTime = 0;
		m_zvirusTimer = 0;
		m_bullethitRegenTimer = 0;
		m_knifehitRegenTimer = 0;
		m_concussionRegenTimer = 0;
		m_bloodHemostaticTimer = 0;
		m_hematopoiesisTimer = 0;
		m_salveEffectTimer = 0;
		m_adrenalinEffectTimer = 0;
		m_antibioticsTimer = 0;
		m_antibioticsStrange = 0;
		m_influenzaTimer = 0;
		m_stomatchpoisonTimer = 0;
		m_radioprotectionTimer = 0;
		m_antidepresantTimer = 0;
		m_lastStomatchpoisonHeal = 0;
		
		
		// Mind state
		m_mindDegradationForce = 0;
		m_mindDegradationTime = 0;
		
		m_skillsRunLastPos = GetPosition();
		m_skillsCrouchLastPos = GetPosition();
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave(ctx);
        
        // VER 0.1
        ctx.Write( SYBERIA_B10_VERSION );
        ctx.Write( m_sleepingValue );
        ctx.Write( m_sleepingBoostTimer );
        ctx.Write( m_sleepingBoostValue );
        ctx.Write( m_sybstats.m_bulletHits );
        ctx.Write( m_sybstats.m_knifeHits );
        ctx.Write( m_sybstats.m_hematomaHits );
        ctx.Write( m_sybstats.m_visceraHit );
        ctx.Write( m_sybstats.m_concussionHit );
        ctx.Write( m_sybstats.m_painLevel );
        ctx.Write( m_painTimer );
        ctx.Write( m_hematomaRegenTimer );
        ctx.Write( m_cuthitRegenTimer );
        ctx.Write( m_sybstats.m_painkillerEffect );
        ctx.Write( m_painkillerTime );
        ctx.Write( m_sybstats.m_stomatchpoisonLevel );
		ctx.Write( m_sybstats.m_stomatchhealLevel );
        ctx.Write( m_stomatchhealTimer );
        ctx.Write( m_hemologicShock );
        ctx.Write( m_sybstats.m_sepsis );
        ctx.Write( m_sepsisTime );
        ctx.Write( m_sybstats.m_zombieVirus );
        ctx.Write( m_zvirusTimer );		
        ctx.Write( m_sybstats.m_bulletBandage1 );
        ctx.Write( m_sybstats.m_bulletBandage2 );
        ctx.Write( m_sybstats.m_knifeBandage1 );
        ctx.Write( m_sybstats.m_knifeBandage2 );
        ctx.Write( m_bullethitRegenTimer );
        ctx.Write( m_knifehitRegenTimer );
        ctx.Write( m_concussionRegenTimer );
        ctx.Write( m_sybstats.m_bloodHemostaticEffect );
        ctx.Write( m_bloodHemostaticTimer );		
        ctx.Write( m_sybstats.m_hematopoiesisEffect );		
        ctx.Write( m_hematopoiesisTimer );
        ctx.Write( m_sybstats.m_salveEffect );
        ctx.Write( m_salveEffectTimer );		
        ctx.Write( m_sybstats.m_adrenalinEffect );
        ctx.Write( m_adrenalinEffectTimer );
        ctx.Write( m_overdosedValue );
		ctx.Write( m_sybstats.m_influenzaLevel );
		ctx.Write( m_influenzaTimer );
		ctx.Write( m_sybstats.m_antibioticsLevel );
		ctx.Write( m_antibioticsTimer );
		ctx.Write( m_antibioticsStrange );
		ctx.Write( m_stomatchpoisonTimer );
		ctx.Write( m_mindStateValue );
		ctx.Write( m_mindDegradationForce );
		ctx.Write( m_mindDegradationTime );
		
		// VER 0.2
        ctx.Write( SYBERIA_B20_VERSION );
		ctx.Write( m_sybstats.m_radiationSickness );
		ctx.Write( m_radiationDose );
		ctx.Write( m_sybstats.m_radioprotectionLevel );
		ctx.Write( m_radioprotectionTimer );
		ctx.Write( m_sybstats.m_antidepresantLevel );
		ctx.Write( m_antidepresantTimer );
		
		// VER 0.49
		ctx.Write( SYBERIA_B49_VERSION );
		ctx.Write( m_sybstats.m_isPveIntruder );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
        // VER 0.1
        int syb_ver;
        if(ctx.Read( syb_ver ) && syb_ver == SYBERIA_B10_VERSION)
		{
			// Sleeping
			if (!ctx.Read( m_sleepingValue )) return false;		
			if (!ctx.Read( m_sleepingBoostTimer )) return false;		
			if (!ctx.Read( m_sleepingBoostValue )) return false;
			
			// Adv medicine
			if (!ctx.Read( m_sybstats.m_bulletHits )) return false;		
			if (!ctx.Read( m_sybstats.m_knifeHits )) return false;		
			if (!ctx.Read( m_sybstats.m_hematomaHits )) return false;		
			if (!ctx.Read( m_sybstats.m_visceraHit )) return false;		
			if (!ctx.Read( m_sybstats.m_concussionHit )) return false;		
			if (!ctx.Read( m_sybstats.m_painLevel )) return false;		
			if (!ctx.Read( m_painTimer )) return false;		
			if (!ctx.Read( m_hematomaRegenTimer )) return false;		
			if (!ctx.Read( m_cuthitRegenTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_painkillerEffect )) return false;		
			if (!ctx.Read( m_painkillerTime )) return false;		
			if (!ctx.Read( m_sybstats.m_stomatchpoisonLevel )) return false;		
			if (!ctx.Read( m_sybstats.m_stomatchhealLevel )) return false;	
			if (!ctx.Read( m_stomatchhealTimer )) return false;		
			if (!ctx.Read( m_hemologicShock )) return false;		
			if (!ctx.Read( m_sybstats.m_sepsis )) return false;		
			if (!ctx.Read( m_sepsisTime )) return false;		
			if (!ctx.Read( m_sybstats.m_zombieVirus )) return false;	
			if (!ctx.Read( m_zvirusTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_bulletBandage1 )) return false;		
			if (!ctx.Read( m_sybstats.m_bulletBandage2 )) return false;		
			if (!ctx.Read( m_sybstats.m_knifeBandage1 )) return false;		
			if (!ctx.Read( m_sybstats.m_knifeBandage2 )) return false;		
			if (!ctx.Read( m_bullethitRegenTimer )) return false;		
			if (!ctx.Read( m_knifehitRegenTimer )) return false;		
			if (!ctx.Read( m_concussionRegenTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_bloodHemostaticEffect )) return false;		
			if (!ctx.Read( m_bloodHemostaticTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_hematopoiesisEffect )) return false;		
			if (!ctx.Read( m_hematopoiesisTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_salveEffect )) return false;		
			if (!ctx.Read( m_salveEffectTimer )) return false;		
			if (!ctx.Read( m_sybstats.m_adrenalinEffect )) return false;
			if (!ctx.Read( m_adrenalinEffectTimer )) return false;
			if (!ctx.Read( m_overdosedValue )) return false;
			if (!ctx.Read( m_sybstats.m_influenzaLevel )) return false;
			if (!ctx.Read( m_influenzaTimer )) return false;
			if (!ctx.Read( m_sybstats.m_antibioticsLevel )) return false;
			if (!ctx.Read( m_antibioticsTimer )) return false;
			if (!ctx.Read( m_antibioticsStrange )) return false;
			if (!ctx.Read( m_stomatchpoisonTimer )) return false;
			
			// Mind state
			if (!ctx.Read( m_mindStateValue )) return false;
			m_mindStateLast = m_mindStateValue;
			if (!ctx.Read( m_mindDegradationForce )) return false;
			if (!ctx.Read( m_mindDegradationTime )) return false;
		}
		else
		{
			return false;
		}
		
		// VER 0.2
		if(ctx.Read( syb_ver ) && syb_ver == SYBERIA_B20_VERSION)
		{
			if (!ctx.Read( m_sybstats.m_radiationSickness )) return false;
			if (!ctx.Read( m_radiationDose )) return false;
			if (!ctx.Read( m_sybstats.m_radioprotectionLevel )) return false;
			if (!ctx.Read( m_radioprotectionTimer )) return false;
			if (!ctx.Read( m_sybstats.m_antidepresantLevel )) return false;
			if (!ctx.Read( m_antidepresantTimer )) return false;
		}
		else
		{
			return false;
		}
		
		// VER 0.49
		if(ctx.Read( syb_ver ) && syb_ver == SYBERIA_B49_VERSION)
		{
			if (!ctx.Read( m_sybstats.m_isPveIntruder )) return false;
		}
		else
		{
			return false;
		}
		
		MarkSybStatsDirty(0);
		return true;
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == SyberiaERPC.SYBERPC_SYNCH_PLAYER_SYBSTATS_REQUEST)
		{
			Param1<ref SyberiaPlayerStats> params = new Param1<ref SyberiaPlayerStats>(m_sybstats);
			RPCSingleParam(SyberiaERPC.SYBERPC_SYNCH_PLAYER_SYBSTATS_RESPONSE, params, true, sender);
		}
	}
	
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		
		if (IsAlive())
		{
			m_advMedUpdateTimer = m_advMedUpdateTimer + deltaTime;
			if (m_advMedUpdateTimer > 0.3)
			{
				OnTickAdvMedicine_Bloodlose(m_advMedUpdateTimer);
				OnTickAdvMedicine_Salve(m_advMedUpdateTimer);
				OnTickAdvMedicine_Regen(m_advMedUpdateTimer);
				OnTickAdvMedicine_Pain(m_advMedUpdateTimer);
				OnTickAdvMedicine_Sepsis(m_advMedUpdateTimer);
				OnTickAdvMedicine_ZVirus(m_advMedUpdateTimer);
				OnTickAdvMedicine_Stomatchheal(m_advMedUpdateTimer);
				OnTickAdvMedicine_Antibiotics(m_advMedUpdateTimer);
				OnTickAdvMedicine_Influenza(m_advMedUpdateTimer);
				//OnTickAdvMedicine_HemorlogicShock(m_advMedUpdateTimer);
				OnTickAdvMedicine_Overdose(m_advMedUpdateTimer);
				OnTickAdvMedicine_HemostatickEffect(m_advMedUpdateTimer);
				OnTickAdvMedicine_HematopoiesisEffect(m_advMedUpdateTimer);
				OnTickAdvMedicine_Radprotector(m_advMedUpdateTimer);
				OnTickAdvMedicine_Antidepresant(m_advMedUpdateTimer);
				OnTickUpdateLastHealthstate();
				m_advMedUpdateTimer = 0;
			}
			
			m_sleepingDecTimer = m_sleepingDecTimer + deltaTime;
			while (m_sleepingDecTimer > 1.0)
			{
				m_sleepingDecTimer = m_sleepingDecTimer - 1.0;
				OnTickLowHealth();
				OnTickSleeping();
				OnTickMindState();
				OnTickSickCheck();
				OnTickStomatchpoison();
				OnTickUnconsition();
				OnTickSkills();
				OnTickExperience();
				OnTickStethoscope();
				OnTickZones();
				OnTickZoneEffect();
				OnTickDisinfectedHands();
				OnTickStamina();
			}
				
			m_sybstatTimer = m_sybstatTimer + deltaTime;
			if (m_sybstatTimer > 0.2)
			{
				m_sybstatTimer = 0;
				if (m_sybstatsDirty)
				{
					m_sybstatsDirty = false;
					Param1<ref SyberiaPlayerStats> params = new Param1<ref SyberiaPlayerStats>(m_sybstats);
					RPCSingleParam(SyberiaERPC.SYBERPC_SYNCH_PLAYER_SYBSTATS_RESPONSE, params, true);
				}
			}
				
			if (m_freeCamMode)
			{
				vector teleportPos = GetPosition();
				teleportPos[1] = GetGame().SurfaceY(teleportPos[0], teleportPos[2]) - 50;
				if ( !GameHelpers.GetPlayerVehicle(this) )
				{
					SetPosition(teleportPos);
				}
			}
		}
	}
	
	private void OnTickUpdateLastHealthstate()
	{
		m_lastHealth = this.GetHealth("GlobalHealth", "Health");
		m_lastBlood = this.GetHealth("GlobalHealth", "Blood");
		m_lastShock = this.GetHealth("GlobalHealth", "Shock");
	}
	
	private void OnTickDisinfectedHands()
	{
		bool lastValue = m_sybstats.m_disinfectedHands;
		
		ItemBase gloves = GetItemOnSlot("Gloves");
		if (gloves && gloves.IsCleanness())
		{			
			m_sybstats.m_disinfectedHands = true;
			
			ItemBase itemInHands = GetItemInHands();
			if (itemInHands && !itemInHands.IsCleanness() && Math.RandomFloat01() < 0.001)
			{
				gloves.SetCleanness(0);
				m_sybstats.m_disinfectedHands = false;
			}
		}
		else if (!gloves && !HasBloodyHands() && GetModifiersManager().IsModifierActive(eModifiers.MDF_DISINFECTION))
		{
			m_sybstats.m_disinfectedHands = true;
		}
		else
		{
			m_sybstats.m_disinfectedHands = false;
		}
		
		if (m_sybstats.m_disinfectedHands != lastValue)
		{
			MarkSybStatsDirty(1);
		}
	}
	
	private void OnTickLowHealth()
	{
		bool criticalBlood = GetHealth("GlobalHealth", "Blood") < PlayerConstants.SL_BLOOD_CRITICAL;
		bool criticalHealth = GetHealth("GlobalHealth", "Health") < PlayerConstants.SL_HEALTH_CRITICAL;
		if (criticalBlood || criticalHealth)
		{
			DecreaseHealth("", "Shock", GetSyberiaConfig().m_shockDecrementOnLowHealthAndBlood);
		}
	}
	
	private void OnTickZones()
	{
		PluginZones pluginZones = PluginZones.Cast(GetPlugin(PluginZones));
		if (!pluginZones)
			return;
		
		ref array<ref ZoneDefinition> currentZones = new array<ref ZoneDefinition>;
		vector pos = GetPosition();
		float landPos = GetGame().SurfaceY(pos[0], pos[2]);
		vector pos2d = pos;
		vector zone2d;
		float landDiff;
		pos2d[1] = 0;
		
		if (pluginZones.m_config.m_customZones)
		{
			foreach (ref ZoneDefinition zone : pluginZones.m_config.m_customZones)
			{
				if (zone.m_height < 0)
				{
					if (vector.Distance(pos, zone.m_position) < zone.m_radius && pos[1] > landPos)
					{
						currentZones.Insert(zone);
					}
				}
				else
				{
					zone2d = zone.m_position;
					zone2d[1] = 0;

					if (vector.Distance(pos2d, zone2d) < zone.m_radius && pos[1] >= zone.m_position[1] && pos[1] < zone.m_position[1] + zone.m_height)
					{
						currentZones.Insert(zone);
					}
				}
			}
		}
		
		if (pluginZones.m_config.m_defaultZone && currentZones.Count() == 0)
		{
			currentZones.Insert(pluginZones.m_config.m_defaultZone);
		}
		
		int nullCheckIndex = 0;
		ref ZoneImplementation zoneImpl;
		while (nullCheckIndex < m_zones.Count())
		{
			zoneImpl = m_zones[nullCheckIndex];
			if (zoneImpl == null || zoneImpl.m_zone == null)
			{
				m_zones.Remove(nullCheckIndex);
			}
			else
			{
				nullCheckIndex = nullCheckIndex + 1;
			}
		}
		
		bool zonesDirty = false;	
		for (int i = 0; i < m_zones.Count(); i++)
		{
			zoneImpl = m_zones[i];
			if (currentZones.Find(zoneImpl.m_zone) == -1)
			{
				if (zoneImpl.m_zoneLeaveTimer < zoneImpl.m_zone.m_leaveTime)
				{
					zoneImpl.m_zoneLeaveTimer = zoneImpl.m_zoneLeaveTimer + 1.0;
				}
				else
				{
					if (zoneImpl.m_zone.m_leaveMessage && zoneImpl.m_zone.m_leaveMessage.LengthUtf8() > 0)
					{
						GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, GetIdentity(), new Param1<string>(zoneImpl.m_zone.m_leaveMessage));
					}
					
					m_zones.Remove(i);
					zonesDirty = true;
					
					if (GetIdentity() != null && m_charProfile != null)
					{
						SybLogSrv("PLAYER " + m_charProfile.m_name + " (" + GetIdentity().GetId() + ") LEAVE FROM ZONE " + zoneImpl.m_zone.m_id + "; ACTIVE ZONES COUNT " + m_zones.Count());
					}
					
					delete zoneImpl;
					zoneImpl = null;
					break;
				}
			}
		}
		
		if (!zonesDirty)
		{
			for (int q = 0; q < currentZones.Count(); q++)
			{
				bool found = false;
				ref ZoneDefinition zoneDef = currentZones[q];
				for (int e = 0; e < m_zones.Count(); e++)
				{
					zoneImpl = m_zones[e];
					if (zoneImpl.m_zone == zoneDef)
					{
						found = true;
						break;
					}
				}
				
				if (!found)
				{				
					if (zoneDef.m_enterMessage && zoneDef.m_enterMessage.LengthUtf8() > 0)
					{
						GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, GetIdentity(), new Param1<string>(zoneDef.m_enterMessage));
					}
					
					m_zones.Insert( new ZoneImplementation(zoneDef) );
					zonesDirty = true;
					
					if (GetIdentity() != null && m_charProfile != null)
					{
						SybLogSrv("PLAYER " + m_charProfile.m_name + " (" + GetIdentity().GetId() + ") ENTER TO ZONE " + zoneDef.m_id + "; ACTIVE ZONES COUNT " + m_zones.Count());
					}
					
					break;
				}
			}
		}
		
		bool enableGodMode = false;
		m_zoneGasTotalValue = 0;
		m_zoneRadTotalValue = 0;
		m_zonePsiTotalValue = 0;
		for (int y = 0; y < m_zones.Count(); y++)
		{
			zoneImpl = m_zones[y];
			if (zoneImpl.m_zone.m_godMode == 1 && !m_sybstats.m_isPveIntruder)
			{
				enableGodMode = true;
			}
			
			m_zoneGasTotalValue = m_zoneGasTotalValue + zoneImpl.m_zone.m_gas;
			m_zoneRadTotalValue = m_zoneRadTotalValue + zoneImpl.m_zone.m_radiation;
			m_zonePsiTotalValue = m_zonePsiTotalValue + zoneImpl.m_zone.m_psi;
		}
		
		if (zonesDirty)
		{
			if (enableGodMode)
			{
				SetAllowDamage(false);
			}
			else
			{
				SetAllowDamage(true);
			}
			
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_CURRENT_ZONE_SYNC, GetIdentity(), new Param1<ref array<ref ZoneImplementation>>(m_zones));
			SetSynchDirty();
		}
		
		currentZones.Clear();
		delete currentZones;
	}

	private void OnTickStamina()
	{
		float staminaDepMod = GetSyberiaConfig().m_defaultStaminaDepletionMultiplier;
		float staminaRecMod = GetSyberiaConfig().m_defaultStaminaRecoveryMultiplier;
		if (m_sybstats.m_adrenalinEffect > 0)
		{
			m_adrenalinEffectTimer = m_adrenalinEffectTimer - 1;
			if (m_adrenalinEffectTimer < 0)
			{
				m_sybstats.m_adrenalinEffect = 0;
				MarkSybStatsDirty(27);
			}
			else
			{
				if (m_sybstats.m_adrenalinEffect >= 1 && m_sybstats.m_adrenalinEffect <= 3)
				{		
					float maxShock = GetMaxHealth("GlobalHealth","Shock");			
					float shockEffectValue = GetSyberiaConfig().m_adrenalinEffectShockUpPerSec[m_sybstats.m_adrenalinEffect - 1]; 
					AddHealth("", "Shock", shockEffectValue * maxShock);
					
					staminaDepMod = staminaDepMod * GetSyberiaConfig().m_adrenalinEffectStaminaDepletionMod[m_sybstats.m_adrenalinEffect - 1];
					staminaRecMod = staminaRecMod * GetSyberiaConfig().m_adrenalinEffectStaminaRecoveryMod[m_sybstats.m_adrenalinEffect - 1];
				}
			}
		}
		else
		{
			m_adrenalinEffectTimer = 0;
		}

		int attCount = GetInventory().AttachmentCount();
		for ( int attIdx = 0; attIdx < attCount; attIdx++ )
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex( attIdx );
			if ( attachment )
			{
				if (attachment.ConfigIsExisting("staminaDepletionMultiplier"))
				{
					staminaDepMod = staminaDepMod * attachment.ConfigGetFloat("staminaDepletionMultiplier");
				}
				
				if (attachment.ConfigIsExisting("staminaRecoveryMultiplier"))
				{
					staminaRecMod = staminaRecMod * attachment.ConfigGetFloat("staminaRecoveryMultiplier");
				}
			}
		}
					
		if (GetStaminaHandler().GetDepletionMultiplier() != staminaDepMod)
		{
			GetStaminaHandler().SetDepletionMultiplier( staminaDepMod );
		}
		
		if (GetStaminaHandler().GetRecoveryMultiplier() != staminaRecMod)
		{
			GetStaminaHandler().SetRecoveryMultiplier( staminaRecMod );
		}
	}
		
	private void OnTickZoneEffect()
	{
		int waterDrain;
		int energyDrain;
		
		if (m_zoneToxicEffect > 0)
		{
			if (m_zoneToxicEffect < 10)
			{
				if (m_zoneToxicEffect % 2 == 0)
				{
					GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
				}
			}
			else
			{			
				if (m_zoneToxicEffect % 10 == 0)
				{
					SymptomBase symptom1 = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);				
					if( symptom1 )
					{
						symptom1.SetDuration(5);
						
						waterDrain = GetSyberiaConfig().m_stomatchpoisonWaterDrainFromVomit[1];
						energyDrain = GetSyberiaConfig().m_stomatchpoisonEnergyDrainFromVomit[1];
		
						if (GetStatWater().Get() > waterDrain)
							GetStatWater().Add(-1 * waterDrain);
						
						if (GetStatEnergy().Get() > energyDrain)
							GetStatEnergy().Add(-1 * energyDrain);
					}
				}
				
				DecreaseHealth("", "Health", 1);
				SetSynchDirty();
			}
		}
		
		bool filterProtection = false;
		ItemBase filter = GetGasMaskFilter();
		if (filter && filter.GetQuantity() > 0)
		{
			filterProtection = true;
			if (m_zoneGasTotalValue > 0)
			{
				filter.AddQuantity(GetSyberiaConfig().m_gasMaskFilterDegradationInToxicZone);				
			}
			else if (m_zoneRadTotalValue > 0)
			{
				filter.AddQuantity(GetSyberiaConfig().m_gasMaskFilterDegradationInRadZone);	
			}
			else
			{
				filter.AddQuantity(GetSyberiaConfig().m_gasMaskFilterDegradationDefault);	
			}
		}
		
		if (m_zoneGasTotalValue > 0 && !filterProtection)
		{
			m_zoneToxicEffect = m_zoneToxicEffect + 1;
		}
		else if (m_zoneToxicEffect > 0)
		{
			m_zoneToxicEffect = m_zoneToxicEffect - 1;
		}
		
		if (m_zoneRadTotalValue > 0)
		{
			float radIncrement = (1.0 - CalculateRadiationProtection()) * m_zoneRadTotalValue;
			if (radIncrement > 0)
			{
				AddRadiationDose(radIncrement);
			}
		}

		if (GetRadiationDose() > 0)
		{
			if (m_sybstats.m_radioprotectionLevel >= 0 && m_sybstats.m_radioprotectionLevel <= 3) {
				AddRadiationDose(GetSyberiaConfig().m_radiationDoseDecrementPerSec[m_sybstats.m_radioprotectionLevel]);
			}
		}
		
		if (m_sybstats.m_radiationSickness > 0)
		{
			DecreaseHealth("", "Health", GetSyberiaConfig().m_radiationHealthDamage[m_sybstats.m_radiationSickness - 1]);
		}
		
		if (m_sybstats.m_radiationSickness >= 2 && Math.RandomFloat01() < 0.05)
		{
			// Vomit
			SymptomBase symptom2 = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);				
			if( symptom2 )
			{
				symptom2.SetDuration(3);
				
				waterDrain = GetSyberiaConfig().m_stomatchpoisonWaterDrainFromVomit[2];
				energyDrain = GetSyberiaConfig().m_stomatchpoisonEnergyDrainFromVomit[2];

				if (GetStatWater().Get() > waterDrain)
					GetStatWater().Add(-1 * waterDrain);
				
				if (GetStatEnergy().Get() > energyDrain)
					GetStatEnergy().Add(-1 * energyDrain);
			}
		}
		
		if (m_sybstats.m_radiationSickness == 3)
		{
			if (Math.RandomFloat01() < 0.1)
			{
				DecreaseHealth("", "Shock", 20);
			}
			
			if (Math.RandomFloat01() < 0.05 && m_BleedingManagerServer.GetBleedingSourcesCount() == 0)
			{
				GetBleedingManagerServer().AttemptAddBleedingSource(0);
			}
		}
	}
	
	private bool IsHeavyItemSkillCheck(ItemBase itemInHands)
	{
		if (!itemInHands)
		{
			return false;
		}
		
		if (itemInHands.IsInherited(MediumTent))
		{
			return false;
		}
		
		if (itemInHands.IsInherited(LargeTent))
		{
			return false;
		}
		
		return itemInHands.IsHeavyBehaviour();
	}
	
	private void OnTickExperience()
	{
		// Increment survival timers
		m_fireplaceLastIgnition = m_fireplaceLastIgnition + 1;
		
		// Calculate overweight perk
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands && IsHeavyItemSkillCheck(itemInHands) && CanDropEntity(itemInHands) && GetPerkBoolValue(SyberiaPerkType.SYBPERK_STRENGTH_HEAVY_ITEMS) == false)
		{
			GetInventory().DropEntity(InventoryMode.SERVER, this, itemInHands);
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, GetIdentity(), new Param1<string>("#syb_skill_overweight_item #syb_perk_name_" + SyberiaPerkType.SYBPERK_STRENGTH_HEAVY_ITEMS));
            SyberiaSoundEmitter.Spawn("JimWow" + Math.RandomIntInclusive(1, 2) + "_SoundEmitter", GetPosition());
		}
		
		// Calculate athletic
		if (!IsSicknesOrInjured())
		{
			float moveDist;
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT)
			{
				moveDist = vector.Distance(m_skillsRunLastPos, GetPosition());
				if (m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT)
				{
					m_sprintingTime = m_sprintingTime + 1;
					if (m_sprintingTime > GetSyberiaConfig().m_skillsExpAthleticsSprintTime && moveDist > 10)
					{
						m_sprintingTime = 0;
						m_skillsRunLastPos = GetPosition();
						AddExperience(SyberiaSkillType.SYBSKILL_ATHLETICS, GetSyberiaConfig().m_skillsExpAthleticsSprintIncrement);
					}
				}
				else if (m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN && moveDist > 5)
				{
					m_jogingTime = m_jogingTime + 1;
					if (m_jogingTime > GetSyberiaConfig().m_skillsExpAthleticsJogTime)
					{
						m_jogingTime = 0;
						m_skillsRunLastPos = GetPosition();
						AddExperience(SyberiaSkillType.SYBSKILL_ATHLETICS, GetSyberiaConfig().m_skillsExpAthleticsJogIncrement);
					}
				}
			}
			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				moveDist = vector.Distance(m_skillsCrouchLastPos, GetPosition());
				m_crouchTime = m_crouchTime + 1;
				if (m_crouchTime > GetSyberiaConfig().m_skillsExpStealthCrouchTime && moveDist > 5)
				{
					m_crouchTime = 0;
					m_skillsCrouchLastPos = GetPosition();
					AddExperience(SyberiaSkillType.SYBSKILL_STEALTH, GetSyberiaConfig().m_skillsExpStealthCrouchIncrement);
				}
			}
		}
	}
	
	private void OnTickStethoscope()
	{
		Stethoscope itemStatoschope = Stethoscope.Cast(GetItemOnSlot("Eyewear"));
		if (!itemStatoschope)
			return;
		
		ItemBase itemInHands = GetItemInHands();
		if (!itemInHands)
			return;
		
		GetInventory().DropEntity(InventoryMode.SERVER, this, itemStatoschope);
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, GetIdentity(), new Param1<string>("#syb_stethoscope_drop"));
	}
	
	private void OnTickSkills()
	{		
		bool dirty = false;
		float skillsStealthStepVolume = 1.0 - Math.Clamp(GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_STEPS_SOUND_DEC, 0, 0), 0, 1);
		float skillsStealthVoiceVolume = 1.0 - Math.Clamp(GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_VOICE_SOUND_DEC, 0, 0), 0, 1);
		float skillsStealthWeaponsVolume = 1.0 - Math.Clamp(GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_WEAPONS_SOUND_DEC, 0, 0), 0, 1);
		float skillsStealthEquipmentVolume = 1.0 - Math.Clamp(GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_EQUIPMENT_SOUND_DEC, 0, 0), 0, 1);
		
		if (m_sybstats.m_skillsStealthStepVolume != skillsStealthStepVolume) { m_sybstats.m_skillsStealthStepVolume = skillsStealthStepVolume; dirty = true; }
		if (m_sybstats.m_skillsStealthVoiceVolume != skillsStealthVoiceVolume) { m_sybstats.m_skillsStealthVoiceVolume = skillsStealthVoiceVolume; dirty = true; }
		if (m_sybstats.m_skillsStealthWeaponsVolume != skillsStealthWeaponsVolume) { m_sybstats.m_skillsStealthWeaponsVolume = skillsStealthWeaponsVolume; dirty = true; }
		if (m_sybstats.m_skillsStealthEquipmentVolume != skillsStealthEquipmentVolume) { m_sybstats.m_skillsStealthEquipmentVolume = skillsStealthEquipmentVolume; dirty = true; }
		
		if (dirty)
		{
			MarkSybStatsDirty(2);
		}
		
		if (m_charProfile && m_charProfile.m_skills && m_charProfile.m_skills.m_dirty)
		{
			m_charProfile.m_skills.m_dirty = false;			
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SKILLS_UPDATE, GetIdentity(), new Param1<ref SkillsContainer>(m_charProfile.m_skills));
			m_skillsSaveDirty = true;
		}
		
		m_skillsSaveInterval = m_skillsSaveInterval + 1;
		if (m_skillsSaveDirty && m_skillsSaveInterval > 60)
		{
			m_skillsSaveDirty = false;
			m_skillsSaveInterval = 0;
			GetSyberiaCharacters().Save( GetIdentity() );
		}
	}
	
	private void OnTickUnconsition()
	{
		if ( GetModifiersManager().IsModifierActive(eModifiers.MDF_UNCONSCIOUSNESS ) )
		{
			m_ShockHandler.SetMultiplier( GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_UNCUNSION_TIME, 1, 1) );
		}
		else
		{
			m_ShockHandler.SetMultiplier( GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_UNCUNSION_CHANCE, 1, 1) );
		}
	}
	
	private void OnTickSleeping()
	{
		int sleepingDiff = 0;		
		sleepingDiff = sleepingDiff - GetSyberiaConfig().m_sleepingDecPerSec;
		
		if (m_sybstats.m_radiationSickness > 0)
		{
			sleepingDiff = sleepingDiff - GetSyberiaConfig().m_radiationSleepingDec;
		}
		
		if (m_sleepingBoostTimer > 0)
		{
			m_sleepingBoostTimer = m_sleepingBoostTimer - 1;
			sleepingDiff = sleepingDiff + m_sleepingBoostValue;
		}

		SyberiaSleepingLevel sleepingLevel = SyberiaSleepingLevel.SYBSL_NONE;		
		if (GetEmoteManager() && GetEmoteManager().IsPlayerSleeping())
		{
			float heatValue = GetStatHeatComfort().Get();
			if (IsSicknesOrInjured())
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_SICK;
			}
			else if (m_sleepingBoostTimer > 0)
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_ENERGED;
			}
			else if (heatValue < PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING)
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_COLD;
			}
			else if (heatValue > PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_CRITICAL)
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_HOT;
			}
			else if (m_HasHeatBuffer)
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_PERFECT;
				sleepingDiff = sleepingDiff + (int)(GetSyberiaConfig().m_sleepingIncPerSleepingLvl2Sec * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_SLEEPING_TIME, 1, 1));
			}
			else
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_COMFORT;
				sleepingDiff = sleepingDiff + (int)(GetSyberiaConfig().m_sleepingIncPerSleepingLvl1Sec * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_SLEEPING_TIME, 1, 1));
			}
		}
		
		int sleepingLvlInt = (int)sleepingLevel;
		if (sleepingLvlInt != m_sybstats.m_sleepingLevel)
		{
			m_sybstats.m_sleepingLevel = sleepingLvlInt;
			MarkSybStatsDirty(3);
		}
		
		if (sleepingLvlInt > 0)
		{
			m_sleepingSoundTimer = m_sleepingSoundTimer + 1;
			if (m_sleepingSoundTimer >= 5)
			{
				m_sleepingSoundTimer = 0;
				if (IsMale()) SyberiaSoundEmitter.Spawn("SleepingMale_SoundEmitter", GetPosition());
				else SyberiaSoundEmitter.Spawn("SleepingFemale_SoundEmitter", GetPosition());
			}
			
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			AddHealth("GlobalHealth", "Health", maxHealth * GetSyberiaConfig().m_sleepingHealPerSec01);						
			if (m_sybstats.m_influenzaLevel <= 1 && GetPerkBoolValue(SyberiaPerkType.SYBPERK_IMMUNITY_INFLUENZA_FIREPLACE_HEAL) && Math.RandomFloat01() < GetSyberiaConfig().m_sleepingHealInfluenzaChance)
			{
				m_sybstats.m_influenzaLevel = 0;
				m_influenzaTimer = 0;
				MarkSybStatsDirty(4);
			}
		}
		else
		{
			m_sleepingSoundTimer = -10;
		}
		
		m_lastSleepingValue = m_sleepingValue;
		m_sleepingValue = m_sleepingValue + sleepingDiff;
		
		if (m_sleepingValue <= 0) 
		{
			if (GetSyberiaConfig().m_sleepingUnconsionEnabled)
			{
				SetHealth("GlobalHealth","Shock",0);
				SetSleepingBoost(GetSyberiaConfig().m_sleepingIncPerUnconsionBoostTime, GetSyberiaConfig().m_sleepingIncPerUnconsionBoostValue);
			}
			
			m_sleepingValue = 0;
		}
		
		if (m_sleepingValue > GetSyberiaConfig().m_sleepingMaxValue)
		{
			m_sleepingValue = GetSyberiaConfig().m_sleepingMaxValue;
		}
	}
	
	private void OnTickSickCheck()
	{
		float perkMod = (1.0 - GetPerkFloatValue(SyberiaPerkType.SYBPERK_SURVIVAL_FROST_RESIST, 0, 0));
		float currHeatComf = GetStatHeatComfort().Get();
		if (currHeatComf < -0.5)
		{
			if (Math.RandomFloat01() < GetSyberiaConfig().m_influenzaApplyOnColdCritChance * perkMod)
			{
				AddInfluenza();
			}
		}
		else if (currHeatComf < -0.9)
		{
			if (Math.RandomFloat01() < GetSyberiaConfig().m_influenzaApplyOnColdWarnChance * perkMod)
			{
				AddInfluenza();
			}
		}
	}
	
	void SetStomatchPoison(int level, float time)
	{
		if (level > m_sybstats.m_stomatchpoisonLevel)
		{
			m_sybstats.m_stomatchpoisonLevel = Math.Clamp(level, 0, 3);
			MarkSybStatsDirty(5);
		}
		
		if (m_sybstats.m_stomatchpoisonLevel > 0)
		{
			float max = GetSyberiaConfig().m_stomatchpoisonDefaultTimes[m_sybstats.m_stomatchpoisonLevel - 1];
			m_stomatchpoisonTimer = Math.Clamp(m_stomatchpoisonTimer + time, 0, max);
			if (m_stomatchpoisonTimer < 60)
			{
				m_stomatchpoisonTimer = 60; 
			}
		}
	}
	
	private void OnTickStomatchpoison()
	{
		int symptomDuration = 0;
		if (m_sybstats.m_stomatchpoisonLevel == 1)
		{
			if (m_sybstats.m_stomatchhealLevel < 1 && Math.RandomFloat01() < GetSyberiaConfig().m_stomatchpoisonVomitChance[0])
			{
				symptomDuration = Math.RandomIntInclusive( 5, 10 );
			}
		}
		if (m_sybstats.m_stomatchpoisonLevel == 2)
		{
			if (m_sybstats.m_stomatchhealLevel < 2 && Math.RandomFloat01() < GetSyberiaConfig().m_stomatchpoisonVomitChance[1])
			{
				symptomDuration = Math.RandomIntInclusive( 2, 8 );
			}
		}
		if (m_sybstats.m_stomatchpoisonLevel == 3)
		{
			if (m_sybstats.m_stomatchhealLevel < 3 && Math.RandomFloat01() < GetSyberiaConfig().m_stomatchpoisonVomitChance[2])
			{
				symptomDuration = Math.RandomIntInclusive( 1, 4 );
			}
		}
				
		m_lastStomatchpoisonHeal = m_lastStomatchpoisonHeal + 1;
		if (m_sybstats.m_stomatchpoisonLevel > 0 && m_sybstats.m_stomatchpoisonLevel <= 3)
		{
			if (symptomDuration > 0)
			{
				SymptomBase symptom = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);				
				if( symptom )
				{
					symptom.SetDuration(symptomDuration);
					
					int waterDrain = GetSyberiaConfig().m_stomatchpoisonWaterDrainFromVomit[m_sybstats.m_stomatchpoisonLevel - 1];
					int energyDrain = GetSyberiaConfig().m_stomatchpoisonEnergyDrainFromVomit[m_sybstats.m_stomatchpoisonLevel - 1];
	
					if (GetStatWater().Get() > waterDrain)
						GetStatWater().Add(-1 * waterDrain);
					
					if (GetStatEnergy().Get() > energyDrain)
						GetStatEnergy().Add(-1 * energyDrain);
				}
			}
			
			m_stomatchpoisonTimer = m_stomatchpoisonTimer - GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_STOMATCHPOISON_TIME, 1, 1);
			if (m_stomatchpoisonTimer < 0)
			{
				m_sybstats.m_stomatchpoisonLevel = m_sybstats.m_stomatchpoisonLevel - 1;
				if (m_sybstats.m_stomatchpoisonLevel > 0 && m_sybstats.m_stomatchpoisonLevel <= 3)
				{
					m_stomatchpoisonTimer = GetSyberiaConfig().m_stomatchpoisonDefaultTimes[m_sybstats.m_stomatchpoisonLevel - 1];
				}
				else
				{
					if (m_lastStomatchpoisonHeal > 600)
					{
						AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityStomatch);
					}
					
					m_stomatchpoisonTimer = 0;
					m_lastStomatchpoisonHeal = 0;
				}
				SetSynchDirty();
			}
		}
	}
	
	private void OnTickMindState()
	{
		m_mindStateLast = m_mindStateValue;
		if (m_mindDegradationTime > 0)
		{
			m_mindDegradationTime = m_mindDegradationTime - GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_MENTAL_TIME, 1, 1);
			m_mindStateValue = m_mindStateValue - m_mindDegradationForce;
		}
		else 
		{
			m_mindDegradationForce = 0;
			m_mindDegradationTime = 0;
			m_mindStateValue = m_mindStateValue + (GetSyberiaConfig().m_mindstateHealPerSec * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_MENTAL_TIME, 1, 1));
		}
		
		if (m_sybstats.m_antidepresantLevel > 0 && m_sybstats.m_antidepresantLevel <= 3)
		{
			m_mindStateValue = m_mindStateValue + GetSyberiaConfig().m_antidepresantMindInc[m_sybstats.m_antidepresantLevel - 1];
		}
		
		if (m_zonePsiTotalValue > 0)
		{
			float perkMod = 1 - GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_MENTAL_TIME, 0, 0);
			m_mindStateValue = m_mindStateValue - (m_zonePsiTotalValue * perkMod);
		}
				
		m_mindStateValue = Math.Clamp(m_mindStateValue, 0, GetSyberiaConfig().m_mindstateMaxValue);
		
		if (m_mindStateValue < GetSyberiaConfig().m_mindstateLevel5)
		{
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			DecreaseHealth("", "Health", maxHealth / GetSyberiaConfig().m_mindstateEmptyDeadtimeSec);
		}
		if (m_mindStateValue < GetSyberiaConfig().m_mindstateLevel3)
		{
			float laughtChange = 1 - (m_mindStateValue / GetSyberiaConfig().m_mindstateLevel3);
			if( Math.RandomFloat01() < laughtChange * 0.1 )
			{
				GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_LAUGHTER);
			}
		}
		
		if (m_mindStateLast != m_mindStateValue)
		{
			SetSynchDirty();
		}
	}
	
	void AddMindDegradation(float force, float time)
	{
		if (m_mindDegradationForce < force)
		{
			m_mindDegradationForce = force;
		}
		
		m_mindDegradationTime = m_mindDegradationTime + time;
	}
	
	void SetSleepingBoost(float time, int value)
	{
		m_sleepingBoostTimer = time;
		m_sleepingBoostValue = value;
		SetSynchDirty();
	}
	
	void AddSleepingBoost(float time, int value)
	{
		if (value == m_sleepingBoostValue)
		{
			m_sleepingBoostTimer = m_sleepingBoostTimer + time;
			SetSynchDirty();
		}
		else
		{
			SetSleepingBoost(time, value);
		}
	}
	
	void AddInfluenza()
	{
		if (m_sybstats.m_antibioticsLevel > 0) return;
		
		if (m_sybstats.m_influenzaLevel == 0 && m_influenzaTimer == 0)
		{
			m_influenzaTimer = GetSyberiaConfig().m_influenzaIncubatePeriodsSec[0];
		}
	}
	
	override bool Consume(ItemBase source, float amount, EConsumeType consume_type )
	{
		bool hasBrainAgents = false;
		if (source)
		{
			ApplyAdvMedicineItem(source.GetType(), amount);
			
			if (source.ContainsAgent(eAgents.BRAIN))
			{
				AddMindDegradation(1, amount);
				source.RemoveAgent(eAgents.BRAIN);
				hasBrainAgents = true;
			}
			else if (source.IsInherited(HumanSteakMeat))
			{
				AddMindDegradation(1, amount);
			}
			
			Edible_Base edibleBaseSource = Edible_Base.Cast(source);
			if (edibleBaseSource)
			{
				if (source.IsMeat())
				{
					if (edibleBaseSource.IsFoodRaw()) SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonRawMeat[0], GetSyberiaConfig().m_stomatchpoisonRawMeat[1] * amount);
					if (edibleBaseSource.IsFoodBurned()) SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonBurnedMeat[0], GetSyberiaConfig().m_stomatchpoisonBurnedMeat[1] * amount);
					if (edibleBaseSource.IsFoodRotten()) SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonRottenMeat[0], GetSyberiaConfig().m_stomatchpoisonRottenMeat[1] * amount);
				}
				else
				{
					if (edibleBaseSource.IsFoodRotten()) SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonRottenFood[0], GetSyberiaConfig().m_stomatchpoisonRottenFood[1] * amount);					
					if (edibleBaseSource.IsFoodBurned()) SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonBurnedFood[0], GetSyberiaConfig().m_stomatchpoisonBurnedFood[1] * amount);
				}
			}
			
			if (source.ContainsAgent(eAgents.SALMONELLA) || source.ContainsAgent(eAgents.CHOLERA) || source.ContainsAgent(eAgents.FOOD_POISON))
			{
				SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonInfection[0], GetSyberiaConfig().m_stomatchpoisonInfection[1] * amount);
			}
			
			if (source.IsLiquidContainer())
			{
				int liquid_type = source.GetLiquidType();
				string liquidClassName = Liquid.GetLiquidClassname(liquid_type);
				if (liquidClassName == "Gasoline" || liquidClassName == "Diesel")
				{
					SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonGasoline[0], GetSyberiaConfig().m_stomatchpoisonGasoline[1] * amount);
				}
			}
			
			if (source.IsTemperatureVisible())
			{
				float temperature = source.GetTemperature();
				if (temperature > 40)
				{
					temperature = (Math.Clamp(temperature, 40.0, 80.0) - 40.0) / 40.0;
					GetStatHeatBuffer().Add(temperature * amount * GetSyberiaConfig().m_temperatureHighConsumeItemsMod);
				}
				else if (temperature < 20)
				{
					temperature = (20.0 - Math.Clamp(temperature, -20.0, 20.0)) / 40.0;
					GetStatHeatBuffer().Add(temperature * amount * GetSyberiaConfig().m_temperatureLowConsumeItemsMod);
				}
			}
		}
		
		if (HasDirtyHands())
		{
			SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonDirtyHands[0], GetSyberiaConfig().m_stomatchpoisonDirtyHands[1] * amount);
		}
		
		if (consume_type == EConsumeType.ENVIRO_POND)
		{
			GetStatHeatBuffer().Add(temperature * amount * GetSyberiaConfig().m_temperatureDrinkPondPenalty);
			
			if (Math.RandomFloat01() < GetSyberiaConfig().m_stomatchpoisonChanceDrinkPond)
			{
				SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonDrinkPond[0], GetSyberiaConfig().m_stomatchpoisonDrinkPond[1] * amount);
			}
		}
		
		if (consume_type == EConsumeType.ENVIRO_WELL)
		{
			GetStatHeatBuffer().Add(temperature * amount * GetSyberiaConfig().m_temperatureDrinkWellPenalty);
			
			if (Math.RandomFloat01() < GetSyberiaConfig().m_stomatchpoisonChanceDrinkWell)
			{
				SetStomatchPoison(GetSyberiaConfig().m_stomatchpoisonDrinkWell[0], GetSyberiaConfig().m_stomatchpoisonDrinkWell[1] * amount);
			}
		}
	
		bool result = super.Consume(source, amount, consume_type);
				
		if (result)
		{
			SodaCan_EnergyDrink edible_item = SodaCan_EnergyDrink.Cast(source);
			if (edible_item)
			{
				AddSleepingBoost(amount, 10);
			}
		}
		
		if (hasBrainAgents)
		{
			source.InsertAgent(eAgents.BRAIN);
		}
		
		ItemBase gloves = GetItemOnSlot("Gloves");
		if (gloves)
		{
			gloves.SetCleanness(0);
		}
		else if ( GetModifiersManager().IsModifierActive(eModifiers.MDF_DISINFECTION))
		{
			GetModifiersManager().DeactivateModifier( eModifiers.MDF_DISINFECTION );
		}
		
		return result;
	}
	
	private float ProcessOverdosedPostinc(float time, float overdoseInc)
	{
		if (time > 3000) return overdoseInc * 5.0;
		if (time > 1000) return overdoseInc * 3.0;
		if (time > 500) return overdoseInc * 2.5;
		if (time > 300) return overdoseInc * 2.0;
		if (time > 100) return overdoseInc * 1.5;
		return overdoseInc;
	}
	
	// medPainkillerLevel, medPainkillerTimeSec
	// medStomatchhealLevel, medStomatchhealTimeSec
	// medAntibioticLevel, medAntibioticsTimeSec, medAntibioticsStrength
	// medHemologicShock
	// medRemoveSepsis
	// medRemoveZVirus
	// medBloodStop
	// medConcussionHeal
	// medBloodHemostatic, medBloodHemostaticTimeSec
	// medBloodHematopoiesis, medBloodHematopoiesisTimeSec
	// medHematomaHeal, medHematomaHealTimeSec
	// medAdrenalinLevel, medAdrenalinTimeSec
	void ApplyAdvMedicineItem(string classname, float amount)
	{
		float overdosedIncrement = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " overdosedIncrement" );
			
		int medPainkillerLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medPainkillerLevel" );
		if (medPainkillerLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_painkillerTime, overdosedIncrement);
			float medPainkillerTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medPainkillerTimeSec" );
			if (medPainkillerLevel >= m_sybstats.m_painkillerEffect && medPainkillerTimeSec > 0)
			{				
				m_sybstats.m_painkillerEffect = medPainkillerLevel;
				m_painkillerTime = m_painkillerTime + (medPainkillerTimeSec * amount);
			}
		}
		
		int medStomatchhealLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medStomatchhealLevel" );
		if (medStomatchhealLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_stomatchhealTimer, overdosedIncrement);
			float medStomatchhealTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medStomatchhealTimeSec" );
			if (medStomatchhealLevel >= m_sybstats.m_stomatchhealLevel && medStomatchhealTimeSec > 0)
			{
				m_sybstats.m_stomatchhealLevel = medStomatchhealLevel;
				m_stomatchhealTimer = m_stomatchhealTimer + (medStomatchhealTimeSec * amount);
			}
		}
		
		int medAntibioticLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAntibioticLevel" );
		if (medAntibioticLevel > 0)
		{			
			overdosedIncrement = ProcessOverdosedPostinc(m_antibioticsTimer, overdosedIncrement);
			float medAntibioticsTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsTimeSec" );
			float medAntibioticsStrength = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsStrength" );
			if (medAntibioticLevel >= m_sybstats.m_antibioticsLevel && medAntibioticsTimeSec > 0 && medAntibioticsStrength > 0)
			{
				m_sybstats.m_antibioticsLevel = medAntibioticLevel;
				m_antibioticsTimer = m_antibioticsTimer + (medAntibioticsTimeSec * amount);
				m_antibioticsStrange = medAntibioticsStrength;
			}			
		}
		
		/*int medHemologicShock = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medHemologicShock" );
		if (medHemologicShock > 0)
		{
			m_hemologicShock = true;
		}*/
		
		int medRemoveSepsis = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medRemoveSepsis" );
		if (medRemoveSepsis > 0)
		{
			m_BleedingManagerServer.SetBloodInfection(false);
		}
		
		int medRemoveZVirus = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medRemoveZVirus" );
		if (medRemoveZVirus > 0)
		{			
			if (medRemoveZVirus <= 1)
			{
				float maxBlood = GetMaxHealth("GlobalHealth","Blood");
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("", "Blood", maxBlood * GetSyberiaConfig().m_zvirusAntidotBloodDmg01);
				DecreaseHealth("", "Health", maxHealth * GetSyberiaConfig().m_zvirusAntidotHealthDmg01);
			}
			
			if (medRemoveZVirus <= 2)
			{
				m_UnconsciousEndTime = -60;
				SetHealth("","Shock",0);
			}
			
			m_BleedingManagerServer.SetZVirus(false);
		}
		
		int medBloodStop = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medBloodStop" );
		if (medBloodStop > 0)
		{
			m_BleedingManagerServer.ApplyBandage(medBloodStop);
		}
		
		int medConcussionHeal = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medConcussionHeal" );
		if (medConcussionHeal > 0)
		{
			m_BleedingManagerServer.SetConcussionHit(false);
		}
		
		int medBloodHemostatic = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medBloodHemostatic" );
		if (medBloodHemostatic > 0)
		{
			m_sybstats.m_bloodHemostaticEffect = true;
			float medBloodHemostaticTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medBloodHemostaticTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_bloodHemostaticTimer, overdosedIncrement);
			m_bloodHemostaticTimer = m_bloodHemostaticTimer + (medBloodHemostaticTimeSec * amount);
		}
		
		int medBloodHematopoiesis = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medBloodHematopoiesis" );
		if (medBloodHematopoiesis > 0)
		{
			m_sybstats.m_hematopoiesisEffect = true;
			float medBloodHematopoiesisTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medBloodHematopoiesisTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_hematopoiesisTimer, overdosedIncrement);
			m_hematopoiesisTimer = m_hematopoiesisTimer + (medBloodHematopoiesisTimeSec * amount);
		}
		
		int medHematomaHeal = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medHematomaHeal" );
		if (medHematomaHeal > 0)
		{
			m_sybstats.m_salveEffect = true;
			float medHematomaHealTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medHematomaHealTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_salveEffectTimer, overdosedIncrement);
			m_salveEffectTimer = m_salveEffectTimer + (medHematomaHealTimeSec * amount);
		}
		
		int medAdrenalinLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAdrenalinLevel" );
		if (medAdrenalinLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_adrenalinEffectTimer, overdosedIncrement);
			if (m_sybstats.m_adrenalinEffect <= medAdrenalinLevel)
			{
				m_sybstats.m_adrenalinEffect = medAdrenalinLevel;
				float medAdrenalinTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAdrenalinTimeSec" );
				m_adrenalinEffectTimer = m_adrenalinEffectTimer + (medAdrenalinTimeSec * amount);
			}
			
			GetStaminaHandler().SetStamina( GetStaminaHandler().GetStaminaMax() );
		}
		
		int medRadprotectLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medRadioprotectionLevel" );
		if (medRadprotectLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_radioprotectionTimer, overdosedIncrement);
			float medRadprotectTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medRadioprotectionTimer" );
			if (medRadprotectLevel >= m_sybstats.m_radioprotectionLevel && medRadprotectTimeSec > 0)
			{
				m_sybstats.m_radioprotectionLevel = medRadprotectLevel;
				m_radioprotectionTimer = m_radioprotectionTimer + (medRadprotectTimeSec * amount);
			}
		}
		
		int medAntidepLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAntidepresantLevel" );
		if (medAntidepLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_antidepresantTimer, overdosedIncrement);
			float medAntidepTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntidepresantTimer" );
			if (medAntidepLevel >= m_sybstats.m_antidepresantLevel && medAntidepTimeSec > 0)
			{
				m_sybstats.m_antidepresantLevel = medAntidepLevel;
				m_antidepresantTimer = m_antidepresantTimer + (medAntidepTimeSec * amount);
			}
		}
		
		float medRadiationIncrement = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medRadiationIncrement" );
		if (medRadiationIncrement != 0)
		{
			m_radiationDose = Math.Max(0, m_radiationDose + medRadiationIncrement);
		}
		
		float medMindDegradationForce = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medMindDegradationForce" );
		if (medMindDegradationForce > 0)
		{
			float medMindDegradationTime = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medMindDegradationTime" );
			if (medMindDegradationTime > 0)
			{
				AddMindDegradation(medMindDegradationForce, medMindDegradationTime);
			}
		}
		
		m_overdosedValue = m_overdosedValue + overdosedIncrement;		
		MarkSybStatsDirty(7);
	}
	
	protected void OnTickAdvMedicine_Bloodlose(float deltaTime)
	{		
		while (m_sybstats.m_bulletBandage1 + m_sybstats.m_bulletBandage2 > m_sybstats.m_bulletHits)
		{
			if (m_sybstats.m_bulletBandage1 > 0) m_sybstats.m_bulletBandage1 = m_sybstats.m_bulletBandage1 - 1;
			else if (m_sybstats.m_bulletBandage2 > 0) m_sybstats.m_bulletBandage2 = m_sybstats.m_bulletBandage2 - 1;
			MarkSybStatsDirty(8);
		}
		
		while (m_sybstats.m_knifeBandage1 + m_sybstats.m_knifeBandage2 > m_sybstats.m_knifeHits)
		{
			if (m_sybstats.m_knifeBandage1 > 0) m_sybstats.m_knifeBandage1 = m_sybstats.m_knifeBandage1 - 1;
			else if (m_sybstats.m_knifeBandage2 > 0) m_sybstats.m_knifeBandage2 = m_sybstats.m_knifeBandage2 - 1;
			MarkSybStatsDirty(9);
		}
		
		int bh_opened = m_sybstats.m_bulletHits - (m_sybstats.m_bulletBandage1 + m_sybstats.m_bulletBandage2);
		int kh_opened = m_sybstats.m_knifeHits - (m_sybstats.m_knifeBandage1 + m_sybstats.m_knifeBandage2);
		
		float bloodlose = 0;
		bloodlose = bloodlose + (bh_opened * GetSyberiaConfig().m_bleedingBullethitOpenPerSecond);
		bloodlose = bloodlose + (m_sybstats.m_bulletBandage1 * GetSyberiaConfig().m_bleedingBullethitCupd1PerSecond);
		bloodlose = bloodlose + (m_sybstats.m_bulletBandage2 * GetSyberiaConfig().m_bleedingBullethitCupd2PerSecond);
		bloodlose = bloodlose + (kh_opened * GetSyberiaConfig().m_bleedingKnifehitOpenPerSecond);
		bloodlose = bloodlose + (m_sybstats.m_knifeBandage1 * GetSyberiaConfig().m_bleedingKnifehitCupd1PerSecond);
		bloodlose = bloodlose + (m_sybstats.m_knifeBandage2 * GetSyberiaConfig().m_bleedingKnifehitCupd2PerSecond);
		bloodlose = bloodlose + (m_sybstats.m_visceraHit * GetSyberiaConfig().m_bleedingVisceraBloodlosePerSecond);
		bloodlose = bloodlose + (m_sybstats.m_hematomaHits * GetSyberiaConfig().m_bleedingHematomaBloodlosePerSecond);
		if (bloodlose > 0.01)
		{
			if (m_sybstats.m_bloodHemostaticEffect)
			{
				bloodlose = bloodlose * GetSyberiaConfig().m_hemostaticEffectModifier;
			}
			DecreaseHealth("", "Blood", bloodlose * deltaTime);
		}
		
		float healthlose = 0;
		healthlose = healthlose + (bh_opened * GetSyberiaConfig().m_bleedingBullethitHealthlosePerSecond);
		healthlose = healthlose + (kh_opened * GetSyberiaConfig().m_bleedingKnifehitHealthlosePerSecond);
		healthlose = healthlose + (m_sybstats.m_visceraHit * GetSyberiaConfig().m_bleedingVisceraHealthlosePerSecond);
		healthlose = healthlose + (m_sybstats.m_hematomaHits * GetSyberiaConfig().m_bleedingHematomaHealthlosePerSecond);
		if (healthlose > 0.01)
		{
			DecreaseHealth("", "Health", healthlose * deltaTime);
		}
	}
	
	protected void OnTickAdvMedicine_Salve(float deltaTime)
	{
		if (m_sybstats.m_salveEffect)
		{
			m_salveEffectTimer = m_salveEffectTimer - deltaTime;
			if (m_salveEffectTimer < 0)
			{
				m_sybstats.m_salveEffect = false;
				MarkSybStatsDirty(10);
			}
		}
		else
		{
			m_salveEffectTimer = 0;
		}
	}
	
	protected void OnTickAdvMedicine_Regen(float deltaTime)
	{
		if (m_sybstats.m_hematomaHits > 0)
		{
			float hematomaRegenOffset = deltaTime * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_HEMATOMA_TIME, 1, 1); 
			if (m_sybstats.m_salveEffect)
			{
				hematomaRegenOffset = hematomaRegenOffset * GetSyberiaConfig().m_hematomaRegenTimeBoostOnSalve;
			}
			
			m_hematomaRegenTimer = m_hematomaRegenTimer + hematomaRegenOffset;
			if (m_hematomaRegenTimer > GetSyberiaConfig().m_hematomaRegenTimerSec)
			{
				m_hematomaRegenTimer = 0;
				m_sybstats.m_hematomaHits = m_sybstats.m_hematomaHits - 1;
				MarkSybStatsDirty(11);
				
				AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityHematoma);
			}
		}
		else
		{
			m_hematomaRegenTimer = 0;
		}
		
		if (GetBleedingBits() != 0)
		{
			m_cuthitRegenTimer = m_cuthitRegenTimer + (deltaTime * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_CUTHIT_TIME, 1, 1));
			if (m_cuthitRegenTimer > GetSyberiaConfig().m_cuthitRegenTimerSec)
			{
				m_cuthitRegenTimer = 0;
				if (m_BleedingManagerServer)
				{
					m_BleedingManagerServer.RemoveMostSignificantBleedingSource();
				}
			}
		}
		else
		{
			m_cuthitRegenTimer = 0;
		}
		
		if (GetSyberiaConfig().m_bleedingBullethitRemoveBandagedTimeSec > 0 && m_sybstats.m_bulletHits > 0 && (m_sybstats.m_bulletBandage1 + m_sybstats.m_bulletBandage2) > 0)
		{
			m_bullethitRegenTimer = m_bullethitRegenTimer + deltaTime;
			if (m_bullethitRegenTimer > GetSyberiaConfig().m_bleedingBullethitRemoveBandagedTimeSec)
			{
				m_bullethitRegenTimer = 0;
				m_BleedingManagerServer.RemoveBulletHit(true);
			}
		}
		else
		{
			m_bullethitRegenTimer = 0;
		}
		
		if (GetSyberiaConfig().m_bleedingKnifehitRemoveBandagedTimeSec > 0 && m_sybstats.m_knifeHits > 0 && (m_sybstats.m_knifeBandage1 + m_sybstats.m_knifeBandage2) > 0)
		{
			m_knifehitRegenTimer = m_knifehitRegenTimer + deltaTime;
			if (m_knifehitRegenTimer > GetSyberiaConfig().m_bleedingKnifehitRemoveBandagedTimeSec)
			{
				m_knifehitRegenTimer = 0;
				m_BleedingManagerServer.RemoveKnifeHit(true);
			}
		}
		else
		{
			m_knifehitRegenTimer = 0;
		}
		
		if (m_sybstats.m_concussionHit)
		{
			m_concussionRegenTimer = m_concussionRegenTimer + deltaTime;
			if (m_concussionRegenTimer > GetSyberiaConfig().m_concussionRegenTimeSec)
			{
				m_concussionRegenTimer = 0;
				m_BleedingManagerServer.SetConcussionHit(false);
			}
		}
		else
		{
			m_concussionRegenTimer = 0;
		}
	}
	
	protected void OnTickAdvMedicine_Pain(float deltaTime)
	{
		m_painTimer = Math.Clamp(m_painTimer - deltaTime, 0, GetSyberiaConfig().m_painMaxDurationSec);
		if (m_sybstats.m_painLevel > 0 && m_painTimer < 0.1)
		{
			AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityPain * m_sybstats.m_painLevel);
			
			m_sybstats.m_painLevel = m_sybstats.m_painLevel - 1;
			if (m_sybstats.m_painLevel == 2) m_painTimer = GetSyberiaConfig().m_painLvl2TimeSec;
			else if (m_sybstats.m_painLevel == 1) m_painTimer = GetSyberiaConfig().m_painLvl1TimeSec;
			MarkSybStatsDirty(12);
		}
		
		if (m_sybstats.m_painLevel < 3 && m_sybstats.m_visceraHit)
		{
			m_sybstats.m_painLevel = 3;
			if (m_painTimer < GetSyberiaConfig().m_painLvl3TimeSec) 
			{
				m_painTimer = GetSyberiaConfig().m_painLvl3TimeSec;
			}
			MarkSybStatsDirty(13);
		}
		else if (m_sybstats.m_painLevel < 2 && (m_sybstats.m_knifeHits > 0 || m_sybstats.m_bulletHits > 0))
		{
			m_sybstats.m_painLevel = 2;
			if (m_painTimer < GetSyberiaConfig().m_painLvl2TimeSec) 
			{
				m_painTimer = GetSyberiaConfig().m_painLvl2TimeSec;
			}
			MarkSybStatsDirty(14);
		}
		
		if (m_sybstats.m_painkillerEffect != 0)
		{
			m_painkillerTime = m_painkillerTime - deltaTime;
			if (m_painkillerTime < 0)
			{
				m_sybstats.m_painkillerEffect = 0;
				m_painkillerTime = 0;
				MarkSybStatsDirty(15);
			}
		}
		
		int currentPainLvl = GetCurrentPainLevel();
		int currentShock = GetHealth("","Shock");
		if (currentPainLvl == 3 && currentShock > 40)
		{
			if (Math.RandomFloat01() <= deltaTime * 0.05)
			{
				DecreaseHealth("", "Shock", 20);
			}
		}
		else if ((m_sybstats.m_concussionHit || currentPainLvl == 2) && currentShock > 60)
		{
			if (Math.RandomFloat01() <= deltaTime)
			{
				DecreaseHealth("","Shock",10);
			}
		}
	}
	
	protected void OnTickAdvMedicine_Sepsis(float deltaTime)
	{
		if (m_sybstats.m_sepsis > 0)
		{
			m_sepsisTime = m_sepsisTime + (deltaTime * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_SEPSIS_TIME, 1, 1));
						
			if (m_sepsisTime > GetSyberiaConfig().m_sepsisStage1TimeSec)
			{
				if (m_sybstats.m_sepsis == 1)
				{
					m_sybstats.m_sepsis = 2;
					MarkSybStatsDirty(16);
				}
				
				AddToEnvironmentTemperature(GetSyberiaConfig().m_sepsisHighTemperatureValue);
			}
			
			if (m_sepsisTime > GetSyberiaConfig().m_sepsisStage2TimeSec)
			{
				if (m_sybstats.m_sepsis == 2)
				{
					m_sybstats.m_sepsis = 3;
					MarkSybStatsDirty(17);
				}
				
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth","Health", (maxHealth / GetSyberiaConfig().m_sepsisDeathTimeSec) * deltaTime);
			}
		}
		else
		{
			m_sepsisTime = 0;
		}
	}
	
	protected void OnTickAdvMedicine_ZVirus(float deltaTime)
	{
		if (m_sybstats.m_zombieVirus > 0)
		{
			m_zvirusTimer = m_zvirusTimer + (deltaTime * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_ZVIRUS_TIME, 1, 1));
			
			if (m_zvirusTimer > GetSyberiaConfig().m_zvirusStage1TimeSec)
			{
				if (m_sybstats.m_zombieVirus == 1) 
				{
					m_sybstats.m_zombieVirus = 2;
					MarkSybStatsDirty(18);
				}
				
				DecreaseHealth("GlobalHealth", "Blood", GetSyberiaConfig().m_zvirusBloodLosePerSec * deltaTime);
				DecreaseHealth("GlobalHealth", "Health", GetSyberiaConfig().m_zvirusHealthLosePerSec * deltaTime);

				if (Math.RandomFloat01() < deltaTime * GetSyberiaConfig().m_zvirusCuthitSpawnChance)
				{
					GetBleedingManagerServer().AttemptAddBleedingSource( Math.RandomIntInclusive(0, 5) );
					RequestSoundEvent(EPlayerSoundEventID.INJURED_LIGHT);
				}
				else if (Math.RandomFloat01() < deltaTime * GetSyberiaConfig().m_zvirusPainSpawnChance)
				{
					if (GetBleedingManagerServer().GetPainLevel() == 1 && Math.RandomFloat01() < 0.1)
					{
						GetBleedingManagerServer().SetPainLevel(2);
					}
					else
					{
						GetBleedingManagerServer().SetPainLevel(1);
					}
					
					RequestSoundEvent(EPlayerSoundEventID.INJURED_LIGHT);
				}
				else if (Math.RandomFloat01() < deltaTime * GetSyberiaConfig().m_zvirusVommitSpawnChance)
				{
					SymptomBase symptom1 = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);				
					if ( symptom1 ) symptom1.SetDuration(3);
					
					int waterDrain = GetSyberiaConfig().m_stomatchpoisonWaterDrainFromVomit[0];
					int energyDrain = GetSyberiaConfig().m_stomatchpoisonEnergyDrainFromVomit[0];
	
					if (GetStatWater().Get() > waterDrain)
						GetStatWater().Add(-1 * waterDrain);
					
					if (GetStatEnergy().Get() > energyDrain)
						GetStatEnergy().Add(-1 * energyDrain);
				}
				/*else if (Math.RandomFloat01() < deltaTime * GetSyberiaConfig().m_zvirusBlindnessSpawnChance)
				{
					SymptomBase symptom2 = GetSymptomManager().QueueUpSecondarySymptomEx(SymptomIDs.SYMPTOM_BLINDNESS);				
					if ( symptom2 ) symptom2.SetDuration(15);
				}
				else if (Math.RandomFloat01() < deltaTime * GetSyberiaConfig().m_zvirusFeverblurSpawnChance)
				{
					SymptomBase symptom3 = GetSymptomManager().QueueUpSecondarySymptomEx(SymptomIDs.SYMPTOM_FEVERBLUR);				
					if ( symptom3 ) symptom3.SetDuration(15);
				}*/
			}
			
			if (m_zvirusTimer > GetSyberiaConfig().m_zvirusStage2TimeSec)
			{
				if (m_sybstats.m_zombieVirus == 2)
				{
					m_sybstats.m_zombieVirus = 3;
					MarkSybStatsDirty(19);
				}
				
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth", "Health", (maxHealth / GetSyberiaConfig().m_zvirusDeathTimeSec) * deltaTime);
			}
		}
		else
		{
			m_zvirusTimer = 0;
		}
	}
	
	protected void OnTickAdvMedicine_Stomatchheal(float deltaTime)
	{
		if (m_sybstats.m_stomatchhealLevel == 0)
			return;

		if (m_sybstats.m_stomatchpoisonLevel > 0)
		{
			m_stomatchpoisonTimer = m_stomatchpoisonTimer - (deltaTime * m_sybstats.m_stomatchhealLevel * GetSyberiaConfig().m_stomatchhealModifier);
		}
		
		m_stomatchhealTimer = Math.Clamp(m_stomatchhealTimer - deltaTime, 0, 999999);
		if (m_stomatchhealTimer < 0.1)
		{
			m_sybstats.m_stomatchhealLevel = 0;
			m_stomatchhealTimer = 0;			
			MarkSybStatsDirty(20);
		}		
	}
	
	protected void OnTickAdvMedicine_Antibiotics(float deltaTime)
	{
		if (m_sybstats.m_antibioticsLevel > 0)
		{
			if (m_antibioticsTimer > 0)
			{
				m_antibioticsTimer = m_antibioticsTimer - deltaTime;
				if (m_sybstats.m_influenzaLevel > 0 && m_sybstats.m_influenzaLevel <= m_sybstats.m_antibioticsLevel)
				{
					float antibioticsMdfrLvl = Math.Clamp((m_sybstats.m_antibioticsLevel - m_sybstats.m_influenzaLevel) + 1, 1, 4);
					float antibioticsMdfrMod = m_antibioticsStrange * antibioticsMdfrLvl * deltaTime * GetSyberiaConfig().m_antibioticsGlobalEffectivityModifier;
					if (Math.RandomFloat01() <= antibioticsMdfrMod)
					{
						m_sybstats.m_influenzaLevel = m_sybstats.m_influenzaLevel - 1;
						if (m_sybstats.m_influenzaLevel > 0 && m_sybstats.m_influenzaLevel < 3)
						{
							m_influenzaTimer = GetSyberiaConfig().m_influenzaIncubatePeriodsSec[m_sybstats.m_influenzaLevel];
						}
						else
						{
							m_influenzaTimer = 0;
							AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityInfluenza);
						}
						
						MarkSybStatsDirty(21);
					}
				}
			}
			else
			{
				m_antibioticsTimer = 0;
				m_sybstats.m_antibioticsLevel = 0;
				m_antibioticsStrange = 0;
				MarkSybStatsDirty(22);
			}
		}
		
		AntibioticsAttack( deltaTime );
	}
	
	protected void OnTickAdvMedicine_Influenza(float deltaTime)
	{
		if (m_influenzaTimer > 0)
		{
			m_influenzaTimer = m_influenzaTimer - (deltaTime * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_INFLUENZA_TIME, 1, 1));
			if (m_influenzaTimer <= 0)
			{
				if (m_sybstats.m_influenzaLevel > 0 && m_sybstats.m_antibioticsLevel >= m_sybstats.m_influenzaLevel)
				{
					m_influenzaTimer = GetSyberiaConfig().m_influenzaIncubatePeriodsSec[m_sybstats.m_influenzaLevel];
				}
				else if (m_sybstats.m_influenzaLevel < 3)
				{
					m_sybstats.m_influenzaLevel = m_sybstats.m_influenzaLevel + 1;
					m_influenzaTimer = GetSyberiaConfig().m_influenzaIncubatePeriodsSec[m_sybstats.m_influenzaLevel];
					MarkSybStatsDirty(23);
				}
				else
				{
					m_influenzaTimer = 0;
				}
			}
		}
		
		if (m_sybstats.m_influenzaLevel > 0 && m_sybstats.m_influenzaLevel <= 3)
		{
			int influenzaLevelIndex = m_sybstats.m_influenzaLevel - 1;	
			if (GetSyberiaConfig().m_influenzaTemperatureLevels[influenzaLevelIndex] > 0)
			{
				AddToEnvironmentTemperature(GetSyberiaConfig().m_influenzaTemperatureLevels[influenzaLevelIndex]);
			}
			
			if ( Math.RandomFloat01() < GetSyberiaConfig().m_influenzaSympthomChance[influenzaLevelIndex] * deltaTime )
			{
				if (influenzaLevelIndex == 0)
				{
					GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_SNEEZE);
				}
				else
				{
					GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
				}
			}
			
			float healthloseTime = GetSyberiaConfig().m_influenzaDeathTime[influenzaLevelIndex];
			if (healthloseTime > 0)
			{
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth","Health", (maxHealth / healthloseTime) * deltaTime);
			}
		}
	}
	
	protected void OnTickAdvMedicine_Overdose(float deltaTime)
	{
		int lastOverdoseInt = m_sybstats.m_overdosedLevel;
		if (m_overdosedValue > 2)
		{
			float curShock = GetHealth("", "Shock");
			if (curShock > 50)
			{
				if (Math.RandomFloat01() < GetSyberiaConfig().m_overdoseUnconChangePerSec * deltaTime)
				{
					m_UnconsciousEndTime = -60;
					SetHealth("","Shock",0);
				}
			}
		}
		
		if (m_overdosedValue > 3)
		{
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			DecreaseHealth("GlobalHealth","Health", (maxHealth / 100) * (m_overdosedValue - 3) * deltaTime);
		}
		
        if (m_overdosedValue > 0)
        {
            m_overdosedValue = m_overdosedValue - (GetSyberiaConfig().m_overdoseDecrementPerSec * GetPerkFloatValue(SyberiaPerkType.SYBPERK_IMMUNITY_OVERDOSE_STRONG, 1, 1) * deltaTime);
 			m_overdosedValue = Math.Max(0, m_overdosedValue);
		}
		
		m_sybstats.m_overdosedLevel = (int)Math.Clamp(m_overdosedValue, 0, 3);
		if (lastOverdoseInt != m_sybstats.m_overdosedLevel)
		{
			MarkSybStatsDirty(24);
		}
		
		if (lastOverdoseInt > 0 && m_sybstats.m_overdosedLevel == 0)
		{
			AddExperience(SyberiaSkillType.SYBSKILL_IMMUNITY, GetSyberiaConfig().m_skillsExpImmunityOverdose);
		}
	}
	
	/*protected void OnTickAdvMedicine_HemorlogicShock(float deltaTime)
	{
		if (m_hemologicShock)
		{
			m_UnconsciousEndTime = -60;
			SetHealth("","Shock",0);
			
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			DecreaseHealth("GlobalHealth","Health", (maxHealth / HEMOLOGIC_SHOCK_DIETIME_SEC) * deltaTime);
			SetSynchDirty();
		}
	}*/
	
	protected void OnTickAdvMedicine_HemostatickEffect(float deltaTime)
	{
		if (m_sybstats.m_bloodHemostaticEffect)
		{
			m_bloodHemostaticTimer = m_bloodHemostaticTimer - deltaTime;
			if (m_bloodHemostaticTimer < 0)
			{
				m_sybstats.m_bloodHemostaticEffect = false;
				MarkSybStatsDirty(25);
			}
		}
	}
	
	protected void OnTickAdvMedicine_HematopoiesisEffect(float deltaTime)
	{
		if (m_sybstats.m_hematopoiesisEffect)
		{
			m_hematopoiesisTimer = m_hematopoiesisTimer - deltaTime;
			if (m_hematopoiesisTimer < 0)
			{
				m_sybstats.m_hematopoiesisEffect = false;
				MarkSybStatsDirty(26);
			}
			else
			{
				AddHealth("", "Blood", GetSyberiaConfig().m_hematopoiesisEffectBloodPerSec * deltaTime);
			}
		}
	}

	protected void OnTickAdvMedicine_Antidepresant(float deltaTime)
	{
		if (m_sybstats.m_antidepresantLevel > 0)
		{
			m_antidepresantTimer = m_antidepresantTimer - deltaTime;
			if (m_antidepresantTimer < 0)
			{
				m_sybstats.m_antidepresantLevel = 0;
				MarkSybStatsDirty(28);
			}
		}
		else
		{
			m_antidepresantTimer = 0;
		}
	}
		
	protected void OnTickAdvMedicine_Radprotector(float deltaTime)
	{
		if (m_sybstats.m_radioprotectionLevel > 0)
		{
			m_radioprotectionTimer = m_radioprotectionTimer - deltaTime;
			if (m_radioprotectionTimer < 0)
			{
				m_sybstats.m_radioprotectionLevel = 0;
				MarkSybStatsDirty(29);
			}
		}
		else
		{
			m_radioprotectionTimer = 0;
		}
	}
	
	bool HasDirtyHands()
	{
		ItemBase gloves = GetItemOnSlot("Gloves");
		if (!gloves && HasBloodyHands())
		{
			return true;
		}
		
		return false;
	}
	
	void AddExperience(int id, float value)
	{
		if (IsAlive() && m_charProfile && m_charProfile.m_skills)
		{
			m_charProfile.m_skills.AddExpirience(id, value);
		}
	}
	
	override int GetSkillValue(int skillId)
	{
		if (IsAlive() && m_charProfile && m_charProfile.m_skills)
		{
			int result = m_charProfile.m_skills.GetSkillValueInt(skillId);
			if (result != -1)
			{
				return result;
			}
		}
		
		return 0;
	}
	
	override int GetPerkIntValue(int perkId, int defaultValue = 0)
	{
		if (IsAlive() && m_charProfile && m_charProfile.m_skills)
		{
			int result = m_charProfile.m_skills.GetPerkValue(perkId);
			if (result != -1)
			{
				return result;
			}
		}
		
		return defaultValue;
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		PlayerBase otherPlayer = PlayerBase.Cast(killer);
		if (otherPlayer && otherPlayer != this && !m_isAlreadyDead && !this.m_sybstats.m_isPveIntruder)
		{
			if (otherPlayer.IsInversedDammageEnabled() || this.IsInversedDammageEnabled())
			{
				otherPlayer.SetAllowDamage(true);
				otherPlayer.SetHealth("", "", 0);
				otherPlayer.m_sybstats.m_isPveIntruder = true;
				otherPlayer.SetSynchDirty();
				otherPlayer.MarkSybStatsDirty(30);
			}
		}
		
		m_isAlreadyDead = true;
	}
	
	private void PostEEHitByAction(PlayerBase sourcePlayer, string dmgZone, float healthLose, float bloodLose, float shockLose, float healthDiff, float bloodDiff, float shockDiff)
	{
		this.AddHealth(dmgZone, "Health", healthLose);
		this.AddHealth(dmgZone, "Blood", bloodLose);
		this.AddHealth(dmgZone, "Shock", shockLose);
		
		this.AddHealth("GlobalHealth", "Health", healthDiff);
		this.AddHealth("GlobalHealth", "Blood", bloodDiff);
		this.AddHealth("GlobalHealth", "Shock", shockDiff);
		
		sourcePlayer.DecreaseHealth(dmgZone, "Health", healthLose * 2);
		sourcePlayer.DecreaseHealth(dmgZone, "Blood", bloodLose * 2);
		sourcePlayer.DecreaseHealth(dmgZone, "Shock", shockLose * 2);
		
		sourcePlayer.DecreaseHealth("GlobalHealth", "Health", healthDiff * 2);
		sourcePlayer.DecreaseHealth("GlobalHealth", "Blood", bloodDiff * 2);
		sourcePlayer.DecreaseHealth("GlobalHealth", "Shock", shockDiff * 2);	
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool inverseDammage = false;
		PlayerBase sourcePlayer = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
		if (sourcePlayer)
		{
			if ( sourcePlayer != this && !m_isAlreadyDead && (sourcePlayer.IsInversedDammageEnabled() || this.IsInversedDammageEnabled()) )
			{
				if (!this.m_sybstats.m_isPveIntruder)
				{
					float healthLose = damageResult.GetDamage(dmgZone, "Health");
					float bloodLose = damageResult.GetDamage(dmgZone, "Blood");
					float shockLose = damageResult.GetDamage(dmgZone, "Shock");
					
					float healthDiff = Math.Max(0, this.m_lastHealth - this.GetHealth("GlobalHealth", "Health"));
					float bloodDiff = Math.Max(0, this.m_lastBlood - this.GetHealth("GlobalHealth", "Blood"));
					float shockDiff = Math.Max(0, this.m_lastShock - this.GetHealth("GlobalHealth", "Shock"));
					this.OnTickUpdateLastHealthstate();	
					
					sourcePlayer.SetAllowDamage(true);
					sourcePlayer.m_sybstats.m_isPveIntruder = true;
					sourcePlayer.SetSynchDirty();
					sourcePlayer.MarkSybStatsDirty(33);
					inverseDammage = true;
					
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.PostEEHitByAction, 10, false, sourcePlayer, dmgZone, healthLose, bloodLose, shockLose, healthDiff, bloodDiff, shockDiff);
				}
			}
			else if (damageResult != null && source != null)
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
							SetHealth("", "Shock", 0);
							GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(KnockoutStateReset, 1000, false);
						}
						
						if (Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthHeavyAttackChance)
						{
							sourcePlayer.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthHeavyAttackValue);
						}
					}
					else
					{
						additionalDmg = additionalDmg * sourcePlayer.GetPerkFloatValue(SyberiaPerkType.SYBPERK_STRENGTH_FAST_ATTACK_STRENGTH, 0, 0);
						
						if (Math.RandomFloat01() < GetSyberiaConfig().m_skillsExpStrengthLightAttackChance)
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
		
		if (inverseDammage && sourcePlayer)
		{
			sourcePlayer.EEHitBy(damageResult, damageType, this, component, dmgZone, ammo, modelPos, speedCoef);
		}
		else
		{
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
	}
	
	override void SetBloodyHands( bool show )
	{
		super.SetBloodyHands(show);
		if (show)
		{
			if ( GetModifiersManager().IsModifierActive(eModifiers.MDF_DISINFECTION))
			{
				GetModifiersManager().DeactivateModifier( eModifiers.MDF_DISINFECTION );
			}
		}
	}
	
	void KnockoutStateReset()
	{
		if (!IsAlive()) return;
		SetHealth("", "Shock", 50);
	}
	
	ItemBase GetGasMaskFilter()
	{
		ItemBase itemMask = GetItemOnSlot("Mask");
		if (!itemMask) return null;
		if (itemMask.IsInherited(GasMask)) return itemMask;
		
		return ItemBase.Cast( itemMask.GetInventory().FindAttachmentByName("GasMaskFilter") );
	}
	
	void OnRadiationDoseChanged()
	{
		int newRadLevel = 0;
		if (m_radiationDose > GetSyberiaConfig().m_radiationLevels[2])
		{
			newRadLevel = 3;
		}
		else if (m_radiationDose > GetSyberiaConfig().m_radiationLevels[1])
		{
			newRadLevel = 2;
		}
		else if (m_radiationDose > GetSyberiaConfig().m_radiationLevels[0])
		{
			newRadLevel = 1;
		}
		
		if (newRadLevel != m_sybstats.m_radiationSickness)
		{
			m_sybstats.m_radiationSickness = newRadLevel;
			MarkSybStatsDirty(31);
		}
	}
	
	void SetRadiationDose(float dose)
	{
		m_radiationDose = Math.Max(0, dose);
		OnRadiationDoseChanged();
	}
	
	void AddRadiationDose(float dose)
	{
		m_radiationDose = Math.Max(0, m_radiationDose + dose);
		OnRadiationDoseChanged();
	}
	
	float GetRadiationDose()
	{
		return m_radiationDose;
	}
	
	// 0 - no protection
	// 1 - full protection
	float CalculateRadiationProtection()
	{
		float value = 0;
		float state = 0;
		float damageByRadiation = GetSyberiaConfig().m_damageClothingInRadiationZone;
		EntityAI attachment;
		ItemBase itemMask = GetItemOnSlot("Mask");
		int attCount = GetInventory().AttachmentCount();
		for ( int attIdx = 0; attIdx < attCount; attIdx++ )
		{
			attachment = GetInventory().GetAttachmentFromIndex( attIdx );
			if ( attachment && attachment.IsClothing() && attachment != itemMask )
			{
				state = Math.Clamp(attachment.GetHealth01("", "") * 2, 0, 1);
				value = value + (GetGame().ConfigGetFloat( "CfgVehicles " + attachment.GetType() + " radiationProtection" ) * state);
				
				if (damageByRadiation > 0)
				{
					attachment.DecreaseHealth("", "", damageByRadiation);
				}
			}
		}
		
		ItemBase gasMaskFilter = GetGasMaskFilter();
		if (gasMaskFilter && gasMaskFilter.GetQuantity() > 0)
		{
			value = value + GetGame().ConfigGetFloat( "CfgVehicles " + gasMaskFilter.GetType() + " radiationProtection" );
		}
		
		return Math.Clamp(value, 0, 1);
	}
	
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		if (!IsDamageDestroyed())
		{
			if (GetSyberiaConfig().m_zombieAttackPlayersInUnconscious)
			{
				return true;
			}
			else
			{
				return !IsUnconscious();
			}
		}
		
		return false;
	}
	
	override void SetVisibilityCoef(float pVisibility)
	{
		float visibilitySkillMod = 1.0 - Math.Clamp(GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_ZOMBIE_AGRO_DEC, 0, 0), 0, 1);
		m_VisibilityCoef = Math.Clamp(pVisibility * (visibilitySkillMod * 2.0), 0, 1.5) * GetSyberiaConfig().m_zombieDetectPlayerVisibilityMultiplier;
	}
	
	void AddExperienceOnIgniteFireplace(int state)
	{
		if (state == 1)
		{
			if (m_fireplaceLastIgnition > 300)
			{
				AddExperience(SyberiaSkillType.SYBSKILL_SURVIVAL, GetSyberiaConfig().m_skillsExpSurvivalIgniteFireSuccess);
				m_fireplaceLastIgnition = 0;
			}
		}
		else if (state == -1)
		{
			AddExperience(SyberiaSkillType.SYBSKILL_SURVIVAL, GetSyberiaConfig().m_skillsExpSurvivalIgniteFireFailed);
		}
	}
	
	void MarkAsNPC()
	{
		m_sybstats.m_isNPC = true;
		MarkSybStatsDirty(32);
	}
	
	void MarkSybStatsDirty(int synchIndexDebug)
	{
		m_sybstatsDirty = true;
		SybLogSrv("SYBSTATS mark dirty for player " + this + " with debug index " + synchIndexDebug);
	}
};