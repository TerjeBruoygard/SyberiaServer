modded class PlayerBase
{	
	float m_painTimer;
	float m_painkillerTime;
	float m_hematomaRegenTimer;
	float m_cuthitRegenTimer;
	
	override void Init()
	{
		super.Init();
		m_painTimer = 0;
		m_hematomaRegenTimer = 0;
		m_cuthitRegenTimer = 0;
		m_painkillerTime = 0;
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
		
		return true;
	}
	
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		OnTickAdvMedicine_Bloodlose(deltaTime);
		OnTickAdvMedicine_Regen(deltaTime);
		OnTickAdvMedicine_Pain(deltaTime);
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
			}
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
};