modded class PlayerBase
{	
	ref CharProfile m_charProfile;
	
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
	
	// Mind state
	float m_mindDegradationForce;
	float m_mindDegradationTime;
	
	// Admin tool
	bool m_freeCamMode = false;
	
	// Skills
	int m_skillsSaveInterval = 0;
	bool m_skillsSaveDirty = false;
	
	override void Init()
	{
		super.Init();
		
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
		
		// Mind state
		m_mindDegradationForce = 0;
		m_mindDegradationTime = 0;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave(ctx);
        
        // VER 100
        ctx.Write( SYBERIA_V100_VERSION );

        // Sleeping
        ctx.Write( m_sleepingValue );
        ctx.Write( m_sleepingBoostTimer );
        ctx.Write( m_sleepingBoostValue );
        
        // Adv medicine
        ctx.Write( m_bulletHits );
        ctx.Write( m_knifeHits );
        ctx.Write( m_hematomaHits );
        ctx.Write( m_visceraHit );
        ctx.Write( m_concussionHit );
        ctx.Write( m_painLevel );
        ctx.Write( m_painTimer );
        ctx.Write( m_hematomaRegenTimer );
        ctx.Write( m_cuthitRegenTimer );
        ctx.Write( m_painkillerEffect );
        ctx.Write( m_painkillerTime );
        ctx.Write( m_stomatchpoisonLevel );
		ctx.Write( m_stomatchhealLevel );
        ctx.Write( m_stomatchhealTimer );
        ctx.Write( m_hemologicShock );
        ctx.Write( m_sepsis );
        ctx.Write( m_sepsisTime );
        ctx.Write( m_zombieVirus );
        ctx.Write( m_zvirusTimer );		
        ctx.Write( m_bulletBandage1 );
        ctx.Write( m_bulletBandage2 );
        ctx.Write( m_knifeBandage1 );
        ctx.Write( m_knifeBandage2 );
        ctx.Write( m_bullethitRegenTimer );
        ctx.Write( m_knifehitRegenTimer );
        ctx.Write( m_concussionRegenTimer );
        ctx.Write( m_bloodHemostaticEffect );
        ctx.Write( m_bloodHemostaticTimer );		
        ctx.Write( m_hematopoiesisEffect );		
        ctx.Write( m_hematopoiesisTimer );
        ctx.Write( m_salveEffect );
        ctx.Write( m_salveEffectTimer );		
        ctx.Write( m_adrenalinEffect );
        ctx.Write( m_adrenalinEffectTimer );
        ctx.Write( m_overdosedValue );
		ctx.Write( m_influenzaLevel );
		ctx.Write( m_influenzaTimer );
		ctx.Write( m_antibioticsLevel );
		ctx.Write( m_antibioticsTimer );
		ctx.Write( m_antibioticsStrange );
		ctx.Write( m_stomatchpoisonTimer );
		
		// Mind state
		ctx.Write( m_mindStateValue );
		ctx.Write( m_mindDegradationForce );
		ctx.Write( m_mindDegradationTime );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
        // VER 100
        int syb_ver_100;
        if(ctx.Read( syb_ver_100 ) && syb_ver_100 == SYBERIA_V100_VERSION)
		{
			// Sleeping
			if(!ctx.Read( m_sleepingValue )) return false;		
			if(!ctx.Read( m_sleepingBoostTimer )) return false;		
			if(!ctx.Read( m_sleepingBoostValue )) return false;
			
			// Adv medicine
			if(!ctx.Read( m_bulletHits )) return false;		
			if(!ctx.Read( m_knifeHits )) return false;		
			if(!ctx.Read( m_hematomaHits )) return false;		
			if(!ctx.Read( m_visceraHit )) return false;		
			if(!ctx.Read( m_concussionHit )) return false;		
			if(!ctx.Read( m_painLevel )) return false;		
			if(!ctx.Read( m_painTimer )) return false;		
			if(!ctx.Read( m_hematomaRegenTimer )) return false;		
			if(!ctx.Read( m_cuthitRegenTimer )) return false;		
			if(!ctx.Read( m_painkillerEffect )) return false;		
			if(!ctx.Read( m_painkillerTime )) return false;		
			if(!ctx.Read( m_stomatchpoisonLevel )) return false;		
			if(!ctx.Read( m_stomatchhealLevel )) return false;	
			if(!ctx.Read( m_stomatchhealTimer )) return false;		
			if(!ctx.Read( m_hemologicShock )) return false;		
			if(!ctx.Read( m_sepsis )) return false;		
			if(!ctx.Read( m_sepsisTime )) return false;		
			if(!ctx.Read( m_zombieVirus )) return false;	
			if(!ctx.Read( m_zvirusTimer )) return false;		
			if(!ctx.Read( m_bulletBandage1 )) return false;		
			if(!ctx.Read( m_bulletBandage2 )) return false;		
			if(!ctx.Read( m_knifeBandage1 )) return false;		
			if(!ctx.Read( m_knifeBandage2 )) return false;		
			if(!ctx.Read( m_bullethitRegenTimer )) return false;		
			if(!ctx.Read( m_knifehitRegenTimer )) return false;		
			if(!ctx.Read( m_concussionRegenTimer )) return false;		
			if(!ctx.Read( m_bloodHemostaticEffect )) return false;		
			if(!ctx.Read( m_bloodHemostaticTimer )) return false;		
			if(!ctx.Read( m_hematopoiesisEffect )) return false;		
			if(!ctx.Read( m_hematopoiesisTimer )) return false;		
			if(!ctx.Read( m_salveEffect )) return false;		
			if(!ctx.Read( m_salveEffectTimer )) return false;		
			if(!ctx.Read( m_adrenalinEffect )) return false;
			if(!ctx.Read( m_adrenalinEffectTimer )) return false;
			if(!ctx.Read( m_overdosedValue )) return false;
			if(!ctx.Read( m_influenzaLevel )) return false;
			if(!ctx.Read( m_influenzaTimer )) return false;
			if(!ctx.Read( m_antibioticsLevel )) return false;
			if(!ctx.Read( m_antibioticsTimer )) return false;
			if(!ctx.Read( m_antibioticsStrange )) return false;
			if(!ctx.Read( m_stomatchpoisonTimer )) return false;
			
			// Mind state
			if(!ctx.Read( m_mindStateValue )) return false;
			m_mindStateLast = m_mindStateValue;
			if(!ctx.Read( m_mindDegradationForce )) return false;
			if(!ctx.Read( m_mindDegradationTime )) return false;
		}
		else
		{
			return false;
		}
		
		return true;
	}
	
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		
		if (IsAlive())
		{
			m_advMedUpdateTimer = m_advMedUpdateTimer + deltaTime;
			if (m_advMedUpdateTimer > 0.2)
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
				OnTickAdvMedicine_Adrenalin(m_advMedUpdateTimer);
				m_advMedUpdateTimer = 0;
			}
			
			m_sleepingDecTimer = m_sleepingDecTimer + deltaTime;
			while (m_sleepingDecTimer > 1.0)
			{
				m_sleepingDecTimer = m_sleepingDecTimer - 1.0;
				OnTickSleeping();
				OnTickMindState();
				OnTickSickCheck();
				OnTickStomatchpoison();
				OnTickSkills();
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
	
	private void OnTickSkills()
	{		
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
	
	private void OnTickSleeping()
	{
		int sleepingDiff = 0;		
		sleepingDiff = sleepingDiff - SLEEPING_DEC_PER_SEC;
		
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
				sleepingDiff = sleepingDiff + SLEEPING_INC_PER_SLEEPING_LVL2_SEC;
			}
			else
			{
				sleepingLevel = SyberiaSleepingLevel.SYBSL_COMFORT;
				sleepingDiff = sleepingDiff + SLEEPING_INC_PER_SLEEPING_LVL1_SEC;
			}
		}
		
		int sleepingLvlInt = (int)sleepingLevel;
		if (sleepingLvlInt != m_sleepingLevel)
		{
			m_sleepingLevel = sleepingLvlInt;
			SetSynchDirty();
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
			AddHealth("GlobalHealth", "Health", maxHealth * SLEEPING_HEAL_PER_SEC_01);						
			if (m_influenzaLevel <= 1 && Math.RandomFloat01() < SLEEPING_HEAL_INFLUENZA_CHANCE)
			{
				m_influenzaLevel = 0;
				m_influenzaTimer = 0;
				SetSynchDirty();
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
			if (SLEEPING_UNCONSION_ENABLED)
			{
				m_UnconsciousEndTime = -60;
				SetHealth("","Shock",0);
				SetSleepingBoost(SLEEPING_INC_PER_UNCONSION_BOOST_TIME, SLEEPING_INC_PER_UNCONSION_BOOST_VALUE);
			}
			
			m_sleepingValue = 0;
		}
		
		if (m_sleepingValue > SLEEPING_MAX_VALUE)
		{
			m_sleepingValue = SLEEPING_MAX_VALUE;
		}
				
		SetSynchDirty();
	}
	
	private void OnTickSickCheck()
	{
		float currHeatComf = GetStatHeatComfort().Get();
		if (currHeatComf < -0.5)
		{
			if (Math.RandomFloat01() < INFLUENZA_APPLY_ON_COLD_CRIT_CHANCE)
			{
				AddInfluenza();
			}
		}
		else if (currHeatComf < -0.9)
		{
			if (Math.RandomFloat01() < INFLUENZA_APPLY_ON_COLD_WARN_CHANCE)
			{
				AddInfluenza();
			}
		}
	}
	
	void SetStomatchPoison(int level, float time)
	{
		if (level > m_stomatchpoisonLevel)
		{
			m_stomatchpoisonLevel = Math.Clamp(level, 0, 3);
			SetSynchDirty();
		}
		
		if (m_stomatchpoisonLevel > 0)
		{
			m_stomatchpoisonTimer = Math.Clamp(m_stomatchpoisonTimer + time, 0, STOMATCHPOISON_DEFAULT_TIMES[m_stomatchpoisonLevel - 1]);
		}
	}
	
	private void OnTickStomatchpoison()
	{
		int symptomDuration = 0;
		if (m_stomatchpoisonLevel == 1)
		{
			if (m_stomatchhealLevel < 1 && Math.RandomFloat01() < STOMATCHPOISON_VOMIT_CHANCE[0])
			{
				symptomDuration = Math.RandomIntInclusive( 5, 10 );
			}
		}
		if (m_stomatchpoisonLevel == 2)
		{
			if (m_stomatchhealLevel < 2 && Math.RandomFloat01() < STOMATCHPOISON_VOMIT_CHANCE[1])
			{
				symptomDuration = Math.RandomIntInclusive( 2, 8 );
			}
		}
		if (m_stomatchpoisonLevel == 3)
		{
			if (m_stomatchhealLevel < 3 && Math.RandomFloat01() < STOMATCHPOISON_VOMIT_CHANCE[2])
			{
				symptomDuration = Math.RandomIntInclusive( 1, 4 );
			}
		}
				
		if (m_stomatchpoisonLevel > 0 && m_stomatchpoisonLevel <= 3)
		{
			if (symptomDuration > 0)
			{
				SymptomBase symptom = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);				
				if( symptom )
				{
					symptom.SetDuration(symptomDuration);
					
					int waterDrain = STOMATCHPOISON_WATER_DRAIN_FROM_VOMIT[m_stomatchpoisonLevel - 1];
					int energyDrain = STOMATCHPOISON_ENERGY_DRAIN_FROM_VOMIT[m_stomatchpoisonLevel - 1];
	
					if (GetStatWater().Get() > waterDrain)
						GetStatWater().Add(-1 * waterDrain);
					
					if (GetStatEnergy().Get() > energyDrain)
						GetStatEnergy().Add(-1 * energyDrain);
				}
			}
			
			m_stomatchpoisonTimer = m_stomatchpoisonTimer - 1.0;
			if (m_stomatchpoisonTimer < 0)
			{
				m_stomatchpoisonLevel = m_stomatchpoisonLevel - 1;
				if (m_stomatchpoisonLevel > 0 && m_stomatchpoisonLevel <= 3)
				{
					m_stomatchpoisonTimer = STOMATCHPOISON_DEFAULT_TIMES[m_stomatchpoisonLevel - 1];
				}
				else
				{
					m_stomatchpoisonTimer = 0;
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
			m_mindDegradationTime = m_mindDegradationTime - 1;
			m_mindStateValue = m_mindStateValue - m_mindDegradationForce;
		}
		else 
		{
			m_mindDegradationForce = 0;
			m_mindDegradationTime = 0;
			m_mindStateValue = m_mindStateValue + MINDSTATE_HEAL_PER_SEC;
		}
				
		m_mindStateValue = Math.Clamp(m_mindStateValue, 0, MINDSTATE_MAX_VALUE);
		
		if (m_mindStateValue < MINDSTATE_LEVEL_5)
		{
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			DecreaseHealth("", "Health", maxHealth / MINDSTATE_EMPTY_DEADTIME_SEC);
		}
		if (m_mindStateValue < MINDSTATE_LEVEL_3)
		{
			float laughtChange = 1 - (m_mindStateValue / MINDSTATE_LEVEL_3);
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
		if (m_antibioticsLevel > 0) return;
		
		if (m_influenzaLevel == 0 && m_influenzaTimer == 0)
		{
			m_influenzaTimer = INFLUENZA_INCUBATE_PERIODS_SEC[0];
		}
	}
	
	override bool Consume(ItemBase source, float amount, EConsumeType consume_type )
	{
		bool hasBrainAgents = false;
		if (source)
		{
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
					if (edibleBaseSource.IsFoodRaw()) SetStomatchPoison(STOMATCHPOISON_RAW_MEAT[0], STOMATCHPOISON_RAW_MEAT[1] * amount);
					if (edibleBaseSource.IsFoodBurned()) SetStomatchPoison(STOMATCHPOISON_BURNED_MEAT[0], STOMATCHPOISON_BURNED_MEAT[1] * amount);
					if (edibleBaseSource.IsFoodRotten()) SetStomatchPoison(STOMATCHPOISON_ROTTEN_MEAT[0], STOMATCHPOISON_ROTTEN_MEAT[1] * amount);
				}
				else
				{
					if (edibleBaseSource.IsFoodRotten()) SetStomatchPoison(STOMATCHPOISON_ROTTEN_FOOD[0], STOMATCHPOISON_ROTTEN_FOOD[1] * amount);					
					if (edibleBaseSource.IsFoodBurned()) SetStomatchPoison(STOMATCHPOISON_BURNED_FOOD[0], STOMATCHPOISON_BURNED_FOOD[1] * amount);
				}
			}
			
			if (source.ContainsAgent(eAgents.SALMONELLA) || source.ContainsAgent(eAgents.CHOLERA))
			{
				SetStomatchPoison(STOMATCHPOISON_INFECTION[0], STOMATCHPOISON_INFECTION[1] * amount);
			}
			
			if (source.IsTemperatureVisible())
			{
				float temperature = source.GetTemperature();
				if (temperature > 40)
				{
					temperature = (Math.Clamp(temperature, 40.0, 80.0) - 40.0) / 40.0;
					GetStatHeatBuffer().Add(temperature * amount * TEMPERATURE_HIGH_CONSUME_ITEMS_MOD);
				}
				else if (temperature < 20)
				{
					temperature = (20.0 - Math.Clamp(temperature, -20.0, 20.0)) / 40.0;
					GetStatHeatBuffer().Add(temperature * amount * TEMPERATURE_LOW_CONSUME_ITEMS_MOD);
				}
			}
		}
		
		if (HasBloodyHands() && !HasMedicalWellGloves())
		{
			SetStomatchPoison(STOMATCHPOISON_DIRTY_HANDS[0], STOMATCHPOISON_DIRTY_HANDS[1] * amount);
		}
		
		if (consume_type == EConsumeType.ENVIRO_POND)
		{
			GetStatHeatBuffer().Add(temperature * amount * TEMPERATURE_DRINK_POND_PENALTY);
			
			if (Math.RandomFloat01() < STOMATCHPOISON_CHANCE_DRINK_POND)
			{
				SetStomatchPoison(STOMATCHPOISON_DRINK_POND[0], STOMATCHPOISON_DRINK_POND[1] * amount);
			}
		}
		
		if (consume_type == EConsumeType.ENVIRO_WELL)
		{
			GetStatHeatBuffer().Add(temperature * amount * TEMPERATURE_DRINK_WELL_PENALTY);
			
			if (Math.RandomFloat01() < STOMATCHPOISON_CHANCE_DRINK_WELL)
			{
				SetStomatchPoison(STOMATCHPOISON_DRINK_WELL[0], STOMATCHPOISON_DRINK_WELL[1] * amount);
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
			if (medPainkillerLevel >= m_painkillerEffect && medPainkillerTimeSec > 0)
			{				
				m_painkillerEffect = medPainkillerLevel;
				m_painkillerTime = m_painkillerTime + (medPainkillerTimeSec * amount);
			}
		}
		
		int medStomatchhealLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medStomatchhealLevel" );
		if (medStomatchhealLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_stomatchhealTimer, overdosedIncrement);
			float medStomatchhealTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medStomatchhealTimeSec" );
			if (medStomatchhealLevel >= m_stomatchhealLevel && medStomatchhealTimeSec > 0)
			{
				m_stomatchhealLevel = medStomatchhealLevel;
				m_stomatchhealTimer = m_stomatchhealTimer + (medStomatchhealTimeSec * amount);
			}
		}
		
		int medAntibioticLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAntibioticLevel" );
		if (medAntibioticLevel > 0)
		{			
			overdosedIncrement = ProcessOverdosedPostinc(m_antibioticsTimer, overdosedIncrement);
			float medAntibioticsTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsTimeSec" );
			float medAntibioticsStrength = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsStrength" );
			if (medAntibioticLevel >= m_antibioticsLevel && medAntibioticsTimeSec > 0 && medAntibioticsStrength > 0)
			{
				m_antibioticsLevel = medAntibioticLevel;
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
				DecreaseHealth("", "Blood", maxBlood * ZVIRUS_ANTIDOT_BLOOD_DMG_01);
				DecreaseHealth("", "Health", maxHealth * ZVIRUS_ANTIDOT_HEALTH_DMG_01);
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
			m_bloodHemostaticEffect = true;
			float medBloodHemostaticTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medBloodHemostaticTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_bloodHemostaticTimer, overdosedIncrement);
			m_bloodHemostaticTimer = m_bloodHemostaticTimer + (medBloodHemostaticTimeSec * amount);
		}
		
		int medBloodHematopoiesis = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medBloodHematopoiesis" );
		if (medBloodHematopoiesis > 0)
		{
			m_hematopoiesisEffect = true;
			float medBloodHematopoiesisTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medBloodHematopoiesisTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_hematopoiesisTimer, overdosedIncrement);
			m_hematopoiesisTimer = m_hematopoiesisTimer + (medBloodHematopoiesisTimeSec * amount);
		}
		
		int medHematomaHeal = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medHematomaHeal" );
		if (medHematomaHeal > 0)
		{
			m_salveEffect = true;
			float medHematomaHealTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medHematomaHealTimeSec" );
			overdosedIncrement = ProcessOverdosedPostinc(m_salveEffectTimer, overdosedIncrement);
			m_salveEffectTimer = m_salveEffectTimer + (medHematomaHealTimeSec * amount);
		}
		
		int medAdrenalinLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAdrenalinLevel" );
		if (medAdrenalinLevel > 0)
		{
			overdosedIncrement = ProcessOverdosedPostinc(m_adrenalinEffectTimer, overdosedIncrement);
			if (m_adrenalinEffect <= medAdrenalinLevel)
			{
				m_adrenalinEffect = medAdrenalinLevel;
				float medAdrenalinTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAdrenalinTimeSec" );
				m_adrenalinEffectTimer = m_adrenalinEffectTimer + (medAdrenalinTimeSec * amount);
			}
		}
		
		m_overdosedValue = m_overdosedValue + overdosedIncrement;		
		SetSynchDirty();
	}
	
	protected void OnTickAdvMedicine_Bloodlose(float deltaTime)
	{		
		while (m_bulletBandage1 + m_bulletBandage2 > m_bulletHits)
		{
			if (m_bulletBandage1 > 0) m_bulletBandage1 = m_bulletBandage1 - 1;
			else if (m_bulletBandage2 > 0) m_bulletBandage2 = m_bulletBandage2 - 1;
			SetSynchDirty();
		}
		
		while (m_knifeBandage1 + m_knifeBandage2 > m_knifeHits)
		{
			if (m_knifeBandage1 > 0) m_knifeBandage1 = m_knifeBandage1 - 1;
			else if (m_knifeBandage2 > 0) m_knifeBandage2 = m_knifeBandage2 - 1;
			SetSynchDirty();
		}
		
		int bh_opened = m_bulletHits - (m_bulletBandage1 + m_bulletBandage2);
		int kh_opened = m_knifeHits - (m_knifeBandage1 + m_knifeBandage2);
		
		float bloodlose = 0;
		bloodlose = bloodlose + (bh_opened * BLEEDING_BULLETHIT_OPEN_PER_SECOND);
		bloodlose = bloodlose + (m_bulletBandage1 * BLEEDING_BULLETHIT_CUPD1_PER_SECOND);
		bloodlose = bloodlose + (m_bulletBandage2 * BLEEDING_BULLETHIT_CUPD2_PER_SECOND);
		bloodlose = bloodlose + (kh_opened * BLEEDING_KNIFEHIT_OPEN_PER_SECOND);
		bloodlose = bloodlose + (m_knifeBandage1 * BLEEDING_KNIFEHIT_CUPD1_PER_SECOND);
		bloodlose = bloodlose + (m_knifeBandage2 * BLEEDING_KNIFEHIT_CUPD2_PER_SECOND);
		bloodlose = bloodlose + (m_visceraHit * BLEEDING_VISCERA_BLOODLOSE_PER_SECOND);
		bloodlose = bloodlose + (m_hematomaHits * BLEEDING_HEMATOMA_BLOODLOSE_PER_SECOND);
		if (bloodlose > 0.01)
		{
			if (m_bloodHemostaticEffect)
			{
				bloodlose = bloodlose * HEMOSTATIC_EFFECT_MODIFIER;
			}
			DecreaseHealth("", "Blood", bloodlose * deltaTime);
		}
		
		float healthlose = 0;
		healthlose = healthlose + (bh_opened * BLEEDING_BULLETHIT_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (kh_opened * BLEEDING_KNIFEHIT_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (m_visceraHit * BLEEDING_VISCERA_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (m_hematomaHits * BLEEDING_HEMATOMA_HEALTHLOSE_PER_SECOND);
		if (healthlose > 0.01)
		{
			DecreaseHealth("", "Health", healthlose * deltaTime);
		}
	}
	
	protected void OnTickAdvMedicine_Salve(float deltaTime)
	{
		if (m_salveEffect)
		{
			m_salveEffectTimer = m_salveEffectTimer - deltaTime;
			if (m_salveEffectTimer < 0)
			{
				m_salveEffect = false;
				SetSynchDirty();
			}
		}
		else
		{
			m_salveEffectTimer = 0;
		}
	}
	
	protected void OnTickAdvMedicine_Regen(float deltaTime)
	{
		if (m_hematomaHits > 0)
		{
			float hematomaRegenOffset = deltaTime; 
			if (m_salveEffect)
			{
				hematomaRegenOffset = hematomaRegenOffset * HEMATOMA_REGEN_TIME_BOOST_ON_SALVE;
			}
			
			m_hematomaRegenTimer = m_hematomaRegenTimer + hematomaRegenOffset;
			if (m_hematomaRegenTimer > HEMATOMA_REGEN_TIMER_SEC)
			{
				m_hematomaRegenTimer = 0;
				m_hematomaHits = m_hematomaHits - 1;
			}
		}
		else
		{
			m_hematomaRegenTimer = 0;
		}
		
		if (GetBleedingBits() != 0)
		{
			m_cuthitRegenTimer = m_cuthitRegenTimer + deltaTime;
			if (m_cuthitRegenTimer > CUTHIT_REGEN_TIMER_SEC)
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
		
		if (BLEEDING_BULLETHIT_REMOVE_BANDAGED_TIME_SEC > 0 && m_bulletHits > 0 && (m_bulletBandage1 + m_bulletBandage2) > 0)
		{
			m_bullethitRegenTimer = m_bullethitRegenTimer + deltaTime;
			if (m_bullethitRegenTimer > BLEEDING_BULLETHIT_REMOVE_BANDAGED_TIME_SEC)
			{
				m_bullethitRegenTimer = 0;
				m_BleedingManagerServer.RemoveBulletHit(true);
			}
		}
		else
		{
			m_bullethitRegenTimer = 0;
		}
		
		if (BLEEDING_KNIFEHIT_REMOVE_BANDAGED_TIME_SEC > 0 && m_knifeHits > 0 && (m_knifeBandage1 + m_knifeBandage2) > 0)
		{
			m_knifehitRegenTimer = m_knifehitRegenTimer + deltaTime;
			if (m_knifehitRegenTimer > BLEEDING_KNIFEHIT_REMOVE_BANDAGED_TIME_SEC)
			{
				m_knifehitRegenTimer = 0;
				m_BleedingManagerServer.RemoveKnifeHit(true);
			}
		}
		else
		{
			m_knifehitRegenTimer = 0;
		}
		
		if (m_concussionHit)
		{
			m_concussionRegenTimer = m_concussionRegenTimer + deltaTime;
			if (m_concussionRegenTimer > CONCUSSION_REGEN_TIME_SEC)
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
		m_painTimer = Math.Clamp(m_painTimer - deltaTime, 0, PAIN_MAX_DURATION_SEC);
		if (m_painLevel != 0 && m_painTimer < 0.1)
		{
			m_painLevel = 0;
			SetSynchDirty();
		}
		
		if (m_painkillerEffect != 0)
		{
			m_painkillerTime = m_painkillerTime - deltaTime;
			if (m_painkillerTime < 0)
			{
				m_painkillerEffect = 0;
				m_painkillerTime = 0;
				SetSynchDirty();
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
		else if ((m_concussionHit || currentPainLvl == 2) && currentShock > 60)
		{
			if (Math.RandomFloat01() <= deltaTime)
			{
				DecreaseHealth("","Shock",10);
			}
		}
	}
	
	protected void OnTickAdvMedicine_Sepsis(float deltaTime)
	{
		if (m_sepsis > 0)
		{
			m_sepsisTime = m_sepsisTime + deltaTime;
						
			if (m_sepsisTime > SEPSIS_STAGE1_TIME_SEC)
			{
				if (m_sepsis == 1)
				{
					m_sepsis = 2;
					SetSynchDirty();
				}
				
				AddToEnvironmentTemperature(SEPSIS_HIGH_TEMPERATURE_VALUE);
			}
			
			if (m_sepsisTime > SEPSIS_STAGE2_TIME_SEC)
			{
				if (m_sepsis == 2)
				{
					m_sepsis = 3;
					SetSynchDirty();
				}
				
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth","Health", (maxHealth / SEPSIS_DEATH_TIME_SEC) * deltaTime);
			}
		}
		else
		{
			m_sepsisTime = 0;
		}
	}
	
	protected void OnTickAdvMedicine_ZVirus(float deltaTime)
	{
		if (m_zombieVirus)
		{
			m_zvirusTimer = m_zvirusTimer + deltaTime;
			
			if (m_zvirusTimer > ZVIRUS_STAGE1_TIME_SEC)
			{
				if (m_zombieVirus == 1) 
				{
					m_zombieVirus = 2;
					SetSynchDirty();
				}
				
				AddHealth("GlobalHealth","Blood", ZVIRUS_BLOOD_REGEN_PER_SEC * deltaTime);
			}
			
			if (m_zvirusTimer > ZVIRUS_STAGE2_TIME_SEC)
			{
				if (m_zombieVirus == 2)
				{
					m_zombieVirus = 3;
					SetSynchDirty();
				}
				
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth","Health", (maxHealth / ZVIRUS_DEATH_TIME_SEC) * deltaTime);
			}
		}
		else
		{
			m_zvirusTimer = 0;
		}
	}
	
	protected void OnTickAdvMedicine_Stomatchheal(float deltaTime)
	{
		if (m_stomatchhealLevel == 0)
			return;

		if (m_stomatchpoisonLevel > 0)
		{
			m_stomatchpoisonTimer = m_stomatchpoisonTimer - (deltaTime * m_stomatchhealLevel * STOMATCHHEAL_MODIFIER);
		}
		
		m_stomatchhealTimer = Math.Clamp(m_stomatchhealTimer - deltaTime, 0, 999999);
		if (m_stomatchhealTimer < 0.1)
		{
			m_stomatchhealLevel = 0;
			m_stomatchhealTimer = 0;			
			SetSynchDirty();
		}		
	}
	
	protected void OnTickAdvMedicine_Antibiotics(float deltaTime)
	{
		if (m_antibioticsLevel > 0)
		{
			if (m_antibioticsTimer > 0)
			{
				m_antibioticsTimer = m_antibioticsTimer - deltaTime;
				if (m_influenzaLevel > 0 && m_influenzaLevel <= m_antibioticsLevel)
				{
					float antibioticsMdfrLvl = Math.Clamp((m_antibioticsLevel - m_influenzaLevel) + 1, 1, 4);
					float antibioticsMdfrMod = m_antibioticsStrange * antibioticsMdfrLvl * deltaTime * ANTIBIOTICS_GLOBAL_EFFECTIVITY_MODIFIER;
					if (Math.RandomFloat01() <= antibioticsMdfrMod)
					{
						m_influenzaLevel = m_influenzaLevel - 1;
						if (m_influenzaLevel > 0 && m_influenzaLevel < 3)
						{
							m_influenzaTimer = INFLUENZA_INCUBATE_PERIODS_SEC[m_influenzaLevel];;
						}
						else
						{
							m_influenzaTimer = 0;
						}
						SetSynchDirty();
					}
				}
			}
			else
			{
				m_antibioticsTimer = 0;
				m_antibioticsLevel = 0;
				m_antibioticsStrange = 0;
				SetSynchDirty();
			}
		}
		
		AntibioticsAttack( deltaTime );
	}
	
	protected void OnTickAdvMedicine_Influenza(float deltaTime)
	{
		if (m_influenzaTimer > 0)
		{
			m_influenzaTimer = m_influenzaTimer - deltaTime;
			if (m_influenzaTimer <= 0)
			{
				if (m_influenzaLevel > 0 && m_antibioticsLevel >= m_influenzaLevel)
				{
					m_influenzaTimer = INFLUENZA_INCUBATE_PERIODS_SEC[m_influenzaLevel];
				}
				else if (m_influenzaLevel < 3)
				{
					m_influenzaLevel = m_influenzaLevel + 1;
					m_influenzaTimer = INFLUENZA_INCUBATE_PERIODS_SEC[m_influenzaLevel];
					SetSynchDirty();
				}
				else
				{
					m_influenzaTimer = 0;
				}
			}
		}
		
		if (m_influenzaLevel > 0 && m_influenzaLevel <= 3)
		{
			int influenzaLevelIndex = m_influenzaLevel - 1;	
			if (INFLUENZA_TEMPERATURE_LEVELS[influenzaLevelIndex] > 0)
			{
				AddToEnvironmentTemperature(INFLUENZA_TEMPERATURE_LEVELS[influenzaLevelIndex]);
			}
			
			if ( Math.RandomFloat01() < INFLUENZA_SYMPTHOM_CHANCE[influenzaLevelIndex] * deltaTime )
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
			
			float healthloseTime = INFLUENZA_DEATH_TIME[influenzaLevelIndex];
			if (healthloseTime > 0)
			{
				float maxHealth = GetMaxHealth("GlobalHealth","Health");
				DecreaseHealth("GlobalHealth","Health", (maxHealth / healthloseTime) * deltaTime);
			}
		}
	}
	
	protected void OnTickAdvMedicine_Overdose(float deltaTime)
	{
		if (m_overdosedValue > 2)
		{
			float curShock = GetHealth("", "Shock");
			if (curShock > 50)
			{
				if (Math.RandomFloat01() < OVERDOSE_UNCON_CHANGE_PER_SEC * deltaTime)
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
			int lastOverdoseInt = (int)m_overdosedValue;
            m_overdosedValue = m_overdosedValue - (OVERDOSE_DECREMENT_PER_SEC * deltaTime);
			int newOverdoseInt = (int)m_overdosedValue;
			if (lastOverdoseInt != newOverdoseInt)
			{
				SetSynchDirty();
			}
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
		if (m_bloodHemostaticEffect)
		{
			m_bloodHemostaticTimer = m_bloodHemostaticTimer - deltaTime;
			if (m_bloodHemostaticTimer < 0)
			{
				m_bloodHemostaticEffect = false;
				SetSynchDirty();
			}
		}
	}
	
	protected void OnTickAdvMedicine_HematopoiesisEffect(float deltaTime)
	{
		if (m_hematopoiesisEffect)
		{
			m_hematopoiesisTimer = m_hematopoiesisTimer - deltaTime;
			if (m_hematopoiesisTimer < 0)
			{
				m_hematopoiesisEffect = false;
				SetSynchDirty();
			}
			else
			{
				AddHealth("", "Blood", HEMATOPOIESIS_EFFECT_BLOOD_PER_SEC * deltaTime);
			}
		}
	}
	
	protected void OnTickAdvMedicine_Adrenalin(float deltaTime)
	{
		if (m_adrenalinEffect > 0)
		{
			m_adrenalinEffectTimer = m_adrenalinEffectTimer - deltaTime;
			if (m_adrenalinEffectTimer < 0)
			{
				m_adrenalinEffect = 0;
				SetSynchDirty();
			}
			else
			{
				float shockEffectValue = 0;
				float maxShock = GetMaxHealth("GlobalHealth","Shock");
				if (m_adrenalinEffect == 1) shockEffectValue = ADRENALIN_EFFECT_SHOCK_01_LVL1_PER_SEC; 
				else if (m_adrenalinEffect == 2) shockEffectValue = ADRENALIN_EFFECT_SHOCK_01_LVL2_PER_SEC;
				else if (m_adrenalinEffect == 3) shockEffectValue = ADRENALIN_EFFECT_SHOCK_01_LVL3_PER_SEC;
				AddHealth("", "Shock", shockEffectValue * maxShock * deltaTime);
			}
		}
		else
		{
			m_adrenalinEffectTimer = 0;
		}
	}
	
	bool HasMedicalWellGloves()
	{
		ItemBase gloves = GetItemOnSlot("Gloves");
		if (!gloves) return false;
		if (gloves.IsRuined()) return false;		
		return GetGame().ConfigGetInt("CfgVehicles " + gloves.GetType() + " medGloves") == 1;
	}
};