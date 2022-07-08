modded class TurnAmpouleRefillLambda
{
    override void OnSuccess (EntityAI new_item)
    {
        super.OnSuccess(new_item);
        
		ItemBase ampouleTarget = ItemBase.Cast(new_item);
		if (!ampouleTarget) return;
		
		float sourceQuantity = m_ampouleSource.GetQuantity();
		float targetEmptyQuantity = ampouleTarget.GetQuantityMax() - ampouleTarget.GetQuantity();
		float transitQuantity = Math.Min(sourceQuantity, targetEmptyQuantity);
		
		m_ampouleSource.AddQuantity(-1 * transitQuantity, true);
		ampouleTarget.AddQuantity(transitQuantity);
    }
};