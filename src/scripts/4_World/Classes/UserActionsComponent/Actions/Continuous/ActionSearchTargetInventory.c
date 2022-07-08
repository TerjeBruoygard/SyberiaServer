modded class ActionSearchTargetInventory
{
    override void OnEndServer( ActionData action_data )
	{
        EntityAI targetEntity = EntityAI.Cast( action_data.m_Target.GetObject() );
		if ( targetEntity != null )
		{
			targetEntity.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		}
		
		ItemBase gloves = action_data.m_Player.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
        }
        else if (Math.RandomFloat01() < GetSyberiaConfig().m_zombieBloodHandsChanceOnLooting)
        {
            action_data.m_Player.SetBloodyHands(true);
        }
    }
};