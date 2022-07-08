modded class CraftSyringeFill extends RecipeBase
{
	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		if (results.Count() > 0)
		{
			SyringeFull syringeFull = SyringeFull.Cast(results[0]);
			if (syringeFull)
			{
				string emptySyringeClassname = ingredients[0].GetType();
				string ampuleClassname = ingredients[1].GetType();
				bool syringeIsDirty = false;
				if (emptySyringeClassname == "SyringeDirty")
				{
					syringeIsDirty = true;
				}
				
				syringeFull.UpdateSyringeData(ampuleClassname, syringeIsDirty);
			}
		}
	}
};
