modded class Cooking
{
	override void ProcessItemToCook(notnull ItemBase pItem, ItemBase cookingEquip, Param2<CookingMethodType, float> pCookingMethod, out Param2<bool, bool> pStateFlags)
	{
		
	Inventory_Base item = Inventory_Base.Cast(pItem);
	
	bool hasWater = cookingEquip.IsLiquidPresent() && (cookingEquip.GetQuantity() / cookingEquip.GetQuantityMax()) > 0.1;
	if ( item && item.CanBeDisinfected() )
			{
				
				float cooking_equipment_temp = cookingEquip.GetTemperature();
				
				if (hasWater && cooking_equipment_temp >= 100)
				{
					item.SetCleanness(1);
					item.RemoveAllAgents();
					return;
				}
			}
	
		super.ProcessItemToCook(pItem,  cookingEquip, pCookingMethod, pStateFlags);
	}
}
