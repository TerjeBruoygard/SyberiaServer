modded class VomitSymptom
{
    override void OnGetActivatedServer(PlayerBase player)
	{
        super.OnGetActivatedServer(player);
        
        if (player)
        {
            if (player.IsFaceBlocked(true))
            {
                ref ItemBase itemCheck = player.GetItemOnSlot("Mask");
                if (!itemCheck)
                {
                    itemCheck = player.GetItemOnSlot("Headgear");
                }
                
                if (itemCheck)
                {
                    player.GetInventory().DropEntity(InventoryMode.SERVER, player, itemCheck);
                }
            }
        }
    }
};