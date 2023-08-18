modded class InjectorBase
{
    override void OnApply(PlayerBase player)
	{
		player.ApplyAdvMedicineItem("CfgVehicles " + GetType(), 1.0);
        
        PlayerBase operator = PlayerBase.Cast( GetHierarchyRootPlayer() ); 
        if (operator) operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineInjector);
	}
};