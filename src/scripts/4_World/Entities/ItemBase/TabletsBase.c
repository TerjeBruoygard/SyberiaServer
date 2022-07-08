modded class TabletsBase
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		consumer.ApplyAdvMedicineItem(GetType(), amount);
	}
};