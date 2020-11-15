modded class PlayerBase
{	
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
		
		return true;
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
}