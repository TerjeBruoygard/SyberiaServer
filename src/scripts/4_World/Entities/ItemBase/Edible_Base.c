modded class Edible_Base
{
	override void EEInit()
	{
		super.EEInit();
		if (CanBeSkinned() && !IsSkinned())
		{
			GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		}
	}
	
	override void EEOnCECreate()
    {
        super.EEOnCECreate();
        if (IsFruit() && Math.RandomFloat01() < 0.25)
        {
            InsertAgent(eAgents.FOOD_POISON, 1);
        }
    }
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );

		ctx.Write( m_alreadySkinned );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( !ctx.Read( m_alreadySkinned ) )
		{
			m_alreadySkinned = false;
			return false;
		}
		
		if (CanBeSkinned() && m_alreadySkinned && GetInventory().IsInventoryLocked())
		{
			GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		}
		
		return true;
	}
	
	void DoSkinning(PlayerBase butcher, ItemBase tool)
	{
		if (m_alreadySkinned)
			return;
		
		if (GetInventory().IsInventoryUnlocked())
			return;
		
		GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		
		m_allowCargoManipulation = true;
		
		ItemBase added_item;
		float meatCountMod = butcher.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_MEAT_COUNT, 0, 0);
		float skinningMod = butcher.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_SKINING, 0, 0);
		string skinningCfg = CFG_VEHICLESPATH + " " + GetType() + " Skinning ";
		string itemName;
		float count;
		float quantityMin, quantityMax;
		
		if (GetGame().ConfigIsExisting(skinningCfg + "ObtainedSteaks"))
		{
			itemName = GetGame().ConfigGetTextOut(skinningCfg + "ObtainedSteaks item");
			count = GetGame().ConfigGetFloat(skinningCfg + "ObtainedSteaks count");
			if (count > 0) 
			{
				quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
				quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);			
				count = Math.Max((count * (meatCountMod + 0.2)) + 1, 1);
				
				while (count > 0) {
					count = count - 1.0;
					added_item = ItemBase.Cast(GetInventory().CreateInInventory(itemName));
					if (added_item) {
						added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
						added_item.InsertAgent(eAgents.SALMONELLA, 1);
						added_item.SetTemperature(38);
					}
				}
			}
		}
		
		if (meatCountMod > 0.3 && GetGame().ConfigIsExisting(skinningCfg + "ObtainedLard"))
		{
			itemName = GetGame().ConfigGetTextOut(skinningCfg + "ObtainedLard item");
			count = GetGame().ConfigGetFloat(skinningCfg + "ObtainedLard count");
			if (count > 0) 
			{
				quantityMin = Math.Clamp(0.1 + meatCountMod, 0.1, 0.9);
				quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);
				
				while (count > 0) {
					count = count - 1.0;
					added_item = ItemBase.Cast(GetInventory().CreateInInventory(itemName));
					if (added_item) {
						added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
						added_item.InsertAgent(eAgents.SALMONELLA, 1);
						added_item.SetTemperature(38);
					}
				}
			}
		}
		
		if (GetGame().ConfigIsExisting(skinningCfg + "ObtainedBones"))
		{
			itemName = GetGame().ConfigGetTextOut(skinningCfg + "ObtainedBones item");
			count = GetGame().ConfigGetFloat(skinningCfg + "ObtainedBones count");	
			if (count > 0) 
			{	
				count = Math.Max((count * (meatCountMod + 0.5)), 1);
				quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
				quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);
				
				while (count > 0) {
					count = count - 1.0;
					added_item = ItemBase.Cast(GetInventory().CreateInInventory(itemName));
					if (added_item) {
						added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
						added_item.InsertAgent(eAgents.SALMONELLA, 1);
						added_item.SetTemperature(38);
						added_item.SetHealth01("", "", meatCountMod);
					}
				}
			}
		}
		
		if (GetGame().ConfigIsExisting(skinningCfg + "ObtainedGuts"))
		{
			itemName = GetGame().ConfigGetTextOut(skinningCfg + "ObtainedGuts item");
			count = GetGame().ConfigGetFloat(skinningCfg + "ObtainedGuts count");
			if (count > 0) 
			{
				quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
				quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);

				while (count > 0) {
					count = count - 1.0;
					added_item = ItemBase.Cast(GetInventory().CreateInInventory(itemName));
					if (added_item) {
						added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
						added_item.InsertAgent(eAgents.SALMONELLA, 1);
						added_item.SetTemperature(38);
						added_item.SetHealth01("", "", meatCountMod);
					}
				}
			}
		}
		
		if (skinningMod > 0 && GetGame().ConfigIsExisting(skinningCfg + "ObtainedPelt"))
		{
			itemName = GetGame().ConfigGetTextOut(skinningCfg + "ObtainedPelt item");
			added_item = ItemBase.Cast(GetInventory().CreateInInventory(itemName));
			if (added_item) {
				added_item.SetTemperature(38);
				added_item.SetHealth01("", "", skinningMod);
			}
		}
		
		m_allowCargoManipulation = false;
		m_alreadySkinned = true;		
		butcher.AddExperience(SyberiaSkillType.SYBSKILL_HUNTING, GetSyberiaConfig().m_skillsExpHuntingButchSmall);
		SetHealth01("", "", 0);
		SetSynchDirty();
				
		if (tool)
		{
			float skill = 1 - butcher.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_TOOLS, 0, 0);
			float dmgFactor = GetSyberiaConfig().m_animalsButchingKnifeDamage;
			float itemMaxHealth = tool.GetMaxHealth("", "");
			tool.DecreaseHealth("", "", skill * dmgFactor * itemMaxHealth );
		}
		
		ItemBase gloves = butcher.GetItemOnSlot("Gloves");
        if (gloves)
        {
            gloves.SetCleanness(0);
        }
        else
        {
            butcher.SetBloodyHands(true);
        }
	}
};