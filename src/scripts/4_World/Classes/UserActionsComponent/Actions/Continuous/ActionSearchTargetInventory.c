modded class ActionSearchTargetInventory
{
    override void OnFinishProgressServer( ActionData action_data )
	{
        ZombieBase targetEntity = ZombieBase.Cast( action_data.m_Target.GetObject() );
		if ( targetEntity != null )
		{
			targetEntity.UnlockInventory();
		}
		
		ItemBase gloves = action_data.m_Player.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
			gloves.DecreaseHealth(0.01, false);
        }
        else if (Math.RandomFloat01() < GetSyberiaConfig().m_zombieBloodHandsChanceOnLooting)
        {
            action_data.m_Player.SetBloodyHands(true);
        }
    }
};