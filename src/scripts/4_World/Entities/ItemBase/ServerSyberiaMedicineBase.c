modded class SyberiaMedicineBase
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		consumer.ApplyAdvMedicineItem(GetType(), amount);
	}
};