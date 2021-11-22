modded class PetrolLighter
{
    override void OnIgnitedTarget( EntityAI ignited_item )
	{
        PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
        float value = 1.0;
        if (player)
        {
            value = (float)player.GetPerkIntValue(SyberiaPerkType.SYBPERK_SURVIVAL_TOOLS_DEGRADATION_DEC, 1);
        }
        
        AddQuantity( -1.0 / value );
	}
	
	override void OnIgnitedTargetFailed( EntityAI target_item )
	{
        PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
        float value = 1.0;
        if (player)
        {
            value = (float)player.GetPerkIntValue(SyberiaPerkType.SYBPERK_SURVIVAL_TOOLS_DEGRADATION_DEC, 1);
        }
        
        AddQuantity( -1.0 / value );
	}
};