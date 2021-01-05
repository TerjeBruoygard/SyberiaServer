modded class SyringeFull extends Inventory_Base
{
	private string m_medSolution;
	private bool m_medDirty;
	
	override void InitItemVariables()
	{
		super.InitItemVariables();
		m_medSolution = "";
		m_medDirty = false;
	}
	
	override void SaveVariables(ParamsWriteContext ctx)
	{
		super.SaveVariables(ctx);
		
		ctx.Write(m_medSolution);
		ctx.Write(m_medDirty);
	}
	
	override bool LoadVariables(ParamsReadContext ctx, int version = -1)
	{
		bool result = super.LoadVariables(ctx, version);
		
		if (!ctx.Read(m_medSolution))
			return false;
		
		if (!ctx.Read(m_medDirty))
			return false;
		
		return result;
	}
	
	string GetMedSolutionClassname()
	{
		return m_medSolution;
	}
	
	bool IsSyringeDirty()
	{
		return m_medDirty;
	}
	
	void UpdateSyringeData(string solutionClassname, bool dirty)
	{
		m_medSolution = solutionClassname;
		m_medDirty = dirty;
	}
	
	override void OnApply(PlayerBase player)
	{
		if (m_medSolution != "")
		{
			if (m_medDirty && Math.RandomFloat01() < SEPSIS_DIRTY_SYRINGE_CHANCE)
			{
				player.m_BleedingManagerServer.SetBloodInfection(true);
			}
			
			player.ApplyAdvMedicineItem(m_medSolution, 1.0);
		}
	}
};