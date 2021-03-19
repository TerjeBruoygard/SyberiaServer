modded class TabletsBase
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		consumer.ApplyAdvMedicineItem(GetType(), amount);
        
        PlayerBase operator = PlayerBase.Cast( GetHierarchyRootPlayer() ); 
		if (operator) operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineTablets);
	}
};