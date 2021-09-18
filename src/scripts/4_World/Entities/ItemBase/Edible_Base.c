modded class Edible_Base
{
	override void EEOnCECreate()
    {
        super.EEOnCECreate();
        if (IsFruit() && Math.RandomFloat01() < 0.25)
        {
            InsertAgent(eAgents.FOOD_POISON, 1);
        }
    }
};