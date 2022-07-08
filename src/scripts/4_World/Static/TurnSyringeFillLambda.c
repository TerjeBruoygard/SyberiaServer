modded class TurnSyringeFillLambda
{
    override void OnSuccess (EntityAI new_item)
    {
        super.OnSuccess(new_item);
        
        SyringeFull syringeFull = SyringeFull.Cast(new_item);
        if (syringeFull)
        {
            syringeFull.UpdateSyringeData(m_medSolutionItem.GetType());
			
			if (m_medSolutionItem.HasQuantity())
	        {
	            m_medSolutionItem.AddQuantity(-1,true);
	        }
	        else
	        {
	            m_medSolutionItem.Delete();
	        }
        }
    }
};