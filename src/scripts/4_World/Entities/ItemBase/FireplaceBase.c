modded class FireplaceBase
{
	private float m_fuelSkillBasedModifier = 1.0;
	
	void SetSkillBasedFuelModifier(float value)
	{
		m_fuelSkillBasedModifier = value;
	}
	
	protected override float GetFuelBurnRateMP()
	{
		return super.GetFuelBurnRateMP() * m_fuelSkillBasedModifier;
	}
};