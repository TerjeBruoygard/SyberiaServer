modded class ActionPlugStethoscope
{
	override void OnExecuteServer( ActionData action_data )
	{
		Stethoscope itemStatoschope = Stethoscope.Cast(action_data.m_Player.GetHumanInventory().GetEntityInHands());
		if (!itemStatoschope)
			return;
		
		ItemBase itemEyewear = action_data.m_Player.GetItemOnSlot("Eyewear");
		if (itemEyewear)
		{
            action_data.m_Player.GetInventory().DropEntity(InventoryMode.SERVER, action_data.m_Player, itemEyewear);
        }
        
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(DoPlugStethoscope, 500, false, itemStatoschope, action_data.m_Player);
        
	}
	
	void DoPlugStethoscope(Stethoscope itemStatoschope, PlayerBase player)
	{
		player.GetHumanInventory().TakeEntityToInventory(InventoryMode.SERVER, FindInventoryLocationType.ATTACHMENT, itemStatoschope);
	}
};