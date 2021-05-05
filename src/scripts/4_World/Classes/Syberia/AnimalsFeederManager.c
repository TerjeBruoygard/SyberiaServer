class AnimalsFeederManager
{	
	static bool IsFeedShack(Object obj)
	{
		return obj.GetType() == "Land_Misc_FeedShack";
	}
	
	static bool IsFeedRack(Object obj)
	{
		if (!obj.IsPlainObject())
			return false;
		
		if (!obj.MemoryPointExists("sound_rainobjectinner1spotipa_1"))
			return false;
		
		if (obj.GetMemoryPointPos("sound_rainobjectinner1spotipa_1") != "0.077831 0.943713 0.014770")
			return false;
		
		return true;
	}
};