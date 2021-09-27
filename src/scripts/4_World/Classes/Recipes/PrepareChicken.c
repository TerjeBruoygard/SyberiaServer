modded class PrepareChicken
{	
	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		Edible_Base item = Edible_Base.Cast(ingredients[0]);
		if (item)
		{
			item.DoSkinning(player, ingredients[1]);
		}
	}
};
