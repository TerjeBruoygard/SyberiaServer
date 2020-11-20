modded class PlayerBase
{	
	float m_painTimer;
	float m_painkillerTime;
	float m_hematomaRegenTimer;
	float m_cuthitRegenTimer;
	float m_stomatchhealTimer;
	bool m_hemologicShock;
	float m_sepsisTime;
	float m_zvirusTimer;
	
	override void Init()
	{
		super.Init();
		m_painTimer = 0;
		m_hematomaRegenTimer = 0;
		m_cuthitRegenTimer = 0;
		m_painkillerTime = 0;
		m_stomatchhealTimer = 0;
		m_hemologicShock = false;
		m_sepsisTime = 0;
		m_zvirusTimer = 0;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave(ctx);
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
		ctx.Write( m_stomatchhealEffect );
		ctx.Write( m_stomatchhealTimer );
		ctx.Write( m_hemologicShock );
		ctx.Write( m_sepsis );
		ctx.Write( m_sepsisTime );
		ctx.Write( m_zombieVirus );
		ctx.Write( m_zvirusTimer );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
		// Sleeping
		if(!ctx.Read( m_sleepingValue ))
			return false;
		
		if(!ctx.Read( m_sleepingBoostTimer ))
			return false;
		
		if(!ctx.Read( m_sleepingBoostValue ))
			return false;
		
		// Adv medicine
		if(!ctx.Read( m_bulletHits ))
			return false;
		
		if(!ctx.Read( m_knifeHits ))
			return false;
		
		if(!ctx.Read( m_hematomaHits ))
			return false;
		
		if(!ctx.Read( m_visceraHit ))
			return false;
		
		if(!ctx.Read( m_concussionHit ))
			return false;
		
		if(!ctx.Read( m_painLevel ))
			return false;
		
		if(!ctx.Read( m_painTimer ))
			return false;
		
		if(!ctx.Read( m_hematomaRegenTimer ))
			return false;
		
		if(!ctx.Read( m_cuthitRegenTimer ))
			return false;
		
		if(!ctx.Read( m_painkillerEffect ))
			return false;
		
		if(!ctx.Read( m_painkillerTime ))
			return false;
		
		if(!ctx.Read( m_stomatchhealEffect ))
			return false;
		
		if(!ctx.Read( m_stomatchhealTimer ))
			return false;
		
		if(!ctx.Read( m_hemologicShock ))
			return false;
		
		if(!ctx.Read( m_sepsis ))
			return false;
		
		if(!ctx.Read( m_sepsisTime ))
			return false;
		
		if(!ctx.Read( m_zombieVirus ))
			return false;
		
		if(!ctx.Read( m_zvirusTimer ))
			return false;
		
		return true;
	}
	
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		OnTickAdvMedicine_Bloodlose(deltaTime);
		OnTickAdvMedicine_Regen(deltaTime);
		OnTickAdvMedicine_Pain(deltaTime);
		OnTickAdvMedicine_Sepsis(deltaTime);
		OnTickAdvMedicine_ZVirus(deltaTime);
		OnTickAdvMedicine_Stomatchheal(deltaTime);
		OnTickAdvMedicine_Antibiotics(deltaTime);
		OnTickAdvMedicine_HemorlogicShock(deltaTime);
	}
	
	override bool Consume(ItemBase source, float amount, EConsumeType consume_type )
	{
		bool result = super.Consume(source, amount, consume_type);
				
		if (result)
		{
			SodaCan_EnergyDrink edible_item = SodaCan_EnergyDrink.Cast(source);
			if (edible_item)
			{
				AddSleepingBoost(amount, 10);
			}
		}
		
		return result;
	}
	
	void ApplyAdvMedicineItem(string classname, float amount)
	{
		int medPainkillerLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medPainkillerLevel" );
		if (medPainkillerLevel > 0)
		{
			if (m_painkillerEffect <= medPainkillerLevel)
			{
				float medPainkillerTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medPainkillerTimeSec" );
				m_painkillerEffect = medPainkillerLevel;
				m_painkillerTime = m_painkillerTime + (medPainkillerTimeSec * amount);
				SetSynchDirty();
			}
		}
		
		int medStomatchhealLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medStomatchhealLevel" );
		if (medStomatchhealLevel > 0)
		{
			if (!m_stomatchhealEffect)
			{
				m_stomatchhealEffect = true;
				m_stomatchhealTimer = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medStomatchhealTimeSec" );
				SetSynchDirty();
			}
		}
		
		int medAntibioticLevel = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medAntibioticLevel" );
		if (medAntibioticLevel > 0)
		{
			if( GetModifiersManager().IsModifierActive(eModifiers.MDF_ANTIBIOTICS))
			{
				GetModifiersManager().DeactivateModifier( eModifiers.MDF_ANTIBIOTICS );
			}
			GetModifiersManager().ActivateModifier( eModifiers.MDF_ANTIBIOTICS );
			AntibioticsMdfr antibioticsMdfr = AntibioticsMdfr.Cast(GetModifiersManager().GetModifier(eModifiers.MDF_ANTIBIOTICS));
			float medAntibioticsTimeSec = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsTimeSec" );
			float medAntibioticsStrength = GetGame().ConfigGetFloat( "CfgVehicles " + classname + " medAntibioticsStrength" );
			if (antibioticsMdfr && medAntibioticsTimeSec > 0 && medAntibioticsStrength > 0)
			{
				antibioticsMdfr.SetAntibioticsConfig(medAntibioticsTimeSec, medAntibioticsStrength);
			}
		}
		
		int medHemologicShock = GetGame().ConfigGetInt( "CfgVehicles " + classname + " medHemologicShock" );
		if (medHemologicShock > 0)
		{
			m_hemologicShock = true;
		}
	}
	
	protected void OnTickAdvMedicine_Bloodlose(float deltaTime)
	{		
		float bloodlose = 0;
		bloodlose = bloodlose + (m_bulletHits * BLEEDING_BULLETHIT_OPEN_PER_SECOND);
		bloodlose = bloodlose + (m_knifeHits * BLEEDING_KNIFEHIT_OPEN_PER_SECOND);
		bloodlose = bloodlose + (m_visceraHit * BLEEDING_VISCERA_BLOODLOSE_PER_SECOND);
		bloodlose = bloodlose + (m_hematomaHits * BLEEDING_HEMATOMA_BLOODLOSE_PER_SECOND);
		if (bloodlose > 0.01)
		{
			DecreaseHealth("", "Blood", bloodlose * deltaTime);
		}
		
		float healthlose = 0;
		healthlose = healthlose + (m_bulletHits * BLEEDING_BULLETHIT_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (m_knifeHits * BLEEDING_KNIFEHIT_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (m_visceraHit * BLEEDING_VISCERA_HEALTHLOSE_PER_SECOND);
		healthlose = healthlose + (m_hematomaHits * BLEEDING_HEMATOMA_HEALTHLOSE_PER_SECOND);
		if (healthlose > 0.01)
		{
			DecreaseHealth("", "Health", healthlose * deltaTime);
		}
	}
	
	protected void OnTickAdvMedicine_Regen(float deltaTime)
	{
		if (m_hematomaHits > 0)
		{
			m_hematomaRegenTimer = m_hematomaRegenTimer + deltaTime;
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
	}
	
	protected void OnTickAdvMedicine_Pain(float deltaTime)
	{
		m_painTimer = Math.Clamp(m_painTimer - deltaTime, 0, PAIN_MAX_DURATION_SEC);
		if (m_painLevel != 0 && m_painTimer < 0.1)
		{
			m_painLevel = 0;
			SetSynchDirty();
		}
		
		m_painkillerTime = Math.Clamp(m_painkillerTime - deltaTime, 0, PAINKILLER_MAX_DURATION_SEC);
		if (m_painkillerEffect != 0 && m_painkillerTime < 0.1)
		{
			m_painkillerEffect = 0;
			SetSynchDirty();
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
				
				// TODO: make high temperature symphtom
				/*PlayerStat<float> heatStat = GetStatHeatComfort();
				float tempValue = heatStat.Get();
				float tempMax = heatStat.GetMax();// * SEPSIS_TEMPERATURE_MAX;
				if (tempValue < tempMax)
				{
					tempValue = tempValue + (tempMax * deltaTime);
					tempValue = Math.Clamp(tempValue, heatStat.GetMin(), tempMax);
					heatStat.Set(tempValue);
				}*/
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
		if (!m_stomatchhealEffect)
			return;
		
		m_stomatchhealTimer = Math.Clamp(m_stomatchhealTimer - deltaTime, 0, 999999);
		if (m_stomatchhealTimer < 0.1)
		{
			m_stomatchhealEffect = false;
			m_stomatchhealTimer = 0;
			if( GetModifiersManager().IsModifierActive(eModifiers.MDF_POISONING))
			{
				GetModifiersManager().DeactivateModifier(eModifiers.MDF_POISONING);
			}
			SetSynchDirty();
		}		
	}
	
	protected void OnTickAdvMedicine_Antibiotics(float deltaTime)
	{
		bool lastAntibioticsEffect = m_antibioticsEffect;
		m_antibioticsEffect = GetModifiersManager().IsModifierActive(eModifiers.MDF_ANTIBIOTICS);
		if (m_antibioticsEffect != lastAntibioticsEffect)
		{
			SetSynchDirty();
		}
	}
	
	protected void OnTickAdvMedicine_HemorlogicShock(float deltaTime)
	{
		if (m_hemologicShock)
		{
			m_UnconsciousEndTime = -60;
			SetHealth("","Shock",0);
			
			float maxHealth = GetMaxHealth("GlobalHealth","Health");
			DecreaseHealth("GlobalHealth","Health", (maxHealth / HEMOLOGIC_SHOCK_DIETIME_SEC) * deltaTime);
			SetSynchDirty();
		}
	}
};