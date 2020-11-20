modded class TurnSyringeFillLambda
{
    override void OnSuccess (EntityAI new_item)
    {
        super.OnSuccess(new_item);
        
        SyringeFull syringeFull = SyringeFull.Cast(new_item);
        if (syringeFull)
        {
            syringeFull.UpdateSyringeData(m_medSolutionClassname, m_dirty);
        }
    }
};