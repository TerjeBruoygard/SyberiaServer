modded class MushroomBase
{
    override void EEOnCECreate()
	{
        super.EEOnCECreate();
        
        if (GetFoodStageType() == FoodStageType.RAW)
		{
			InsertAgent(eAgents.FOOD_POISON, 1);
		}
    }
};