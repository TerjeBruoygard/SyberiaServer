modded class InjectorBase
{
    override void OnApply(PlayerBase player)
	{
		player.ApplyAdvMedicineItem(GetType(), 1.0);
	}
};