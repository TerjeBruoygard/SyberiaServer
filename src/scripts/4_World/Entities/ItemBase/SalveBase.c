modded class SalveBase : Inventory_Base
{
    override void OnApply(PlayerBase player)
	{
		player.ApplyAdvMedicineItem(GetType(), 1.0);
	}
};