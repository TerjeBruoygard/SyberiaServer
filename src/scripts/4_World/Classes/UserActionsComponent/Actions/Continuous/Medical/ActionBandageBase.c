modded class ActionBandageBase: ActionContinuousBase
{
	override void ApplyBandage( ItemBase item, PlayerBase player )
	{	
		player.ApplyAdvMedicineItem(item.GetType(), 1.0);
		
		if (Math.RandomFloat01() < GetSyberiaConfig().m_sepsisDitryHandsBandageChance && player.HasBloodyHands() && !player.HasMedicalWellGloves())
		{
			player.m_BleedingManagerServer.SetBloodInfection(true);
		}
		
		if (item.HasQuantity())
		{
			item.AddQuantity(-1,true);
		}
		else
		{
			item.Delete();
		}
	}
};