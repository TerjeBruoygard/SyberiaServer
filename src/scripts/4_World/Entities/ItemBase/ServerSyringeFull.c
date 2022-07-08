modded class SyringeFull extends Inventory_Base
{
	private string m_medSolution;
	
	override void InitItemVariables()
	{
		super.InitItemVariables();
		m_medSolution = "";
	}
	
	override void SaveVariables(ParamsWriteContext ctx)
	{
		super.SaveVariables(ctx);
		
		ctx.Write(m_medSolution);
	}
	
	override bool LoadVariables(ParamsReadContext ctx, int version = -1)
	{
		bool result = super.LoadVariables(ctx, version);
		
		if (!ctx.Read(m_medSolution))
			return false;
		
		return result;
	}
	
	string GetMedSolutionClassname()
	{
		return m_medSolution;
	}
	
	bool IsSyringeDirty()
	{
		return !IsCleanness();
	}
	
	void UpdateSyringeData(string solutionClassname)
	{
		m_medSolution = solutionClassname;
	}
	
	override void OnApply(PlayerBase player)
	{
		if (m_medSolution != "")
		{
			if (IsSyringeDirty() && Math.RandomFloat01() < GetSyberiaConfig().m_sepsisDirtySyringeChance)
			{
				player.m_BleedingManagerServer.SetBloodInfection(true);
			}
			
			player.ApplyAdvMedicineItem(m_medSolution, 1.0);
            
            PlayerBase operator = PlayerBase.Cast( GetHierarchyRootPlayer() ); 
            if (operator) operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineAmpoule);
		}
	}
};