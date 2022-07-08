modded class CarScript
{
	private static ref array<CarScript> m_allVehicles = new array<CarScript>;
	
	static void GetAllVehicles(ref array<CarScript> vechiles)
	{
		vechiles.Copy( m_allVehicles );
	}
	
	override void EEInit()
	{
		super.EEInit();
		m_allVehicles.Insert(this);
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		m_allVehicles.RemoveItem(this);
	}
};