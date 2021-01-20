modded class TurnCombineSybLambda
{
    override void OnSuccess (EntityAI new_item)
    {
        super.OnSuccess(new_item);
        
		ItemBase itemTarget = ItemBase.Cast(new_item);
		if (!itemTarget) return;
		
		float sourceQuantity = m_ampouleSource.GetQuantity();
		float targetEmptyQuantity = itemTarget.GetQuantityMax() - itemTarget.GetQuantity();
		float transitQuantity = Math.Min(sourceQuantity, targetEmptyQuantity);
		
		m_ampouleSource.AddQuantity(-1 * transitQuantity, true);
		itemTarget.AddQuantity(transitQuantity);
    }
};