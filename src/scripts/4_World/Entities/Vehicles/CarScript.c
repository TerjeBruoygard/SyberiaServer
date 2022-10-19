modded class CarScript
{
	private static ref array<CarScript> m_sybAdm_allVehicles = new array<CarScript>;
	
	static void SybAdm_GetAllVehicles(ref array<CarScript> vechiles)
	{
		vechiles.Copy( m_sybAdm_allVehicles );
	}
	
	override void EEInit()
	{
		super.EEInit();
		m_sybAdm_allVehicles.Insert(this);
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		m_sybAdm_allVehicles.RemoveItem(this);
	}
};