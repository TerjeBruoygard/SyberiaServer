modded class EffectAreaLoader
{
    static void CreateZones()
	{
        
    }
    
    static JsonDataContaminatedAreas GetData()
	{
        ref JsonDataContaminatedAreas data = new JsonDataContaminatedAreas();
        data.Areas = new array<ref JsonDataContaminatedArea>;
        data.SafePositions = new array<ref array<float>>;
		return data;
    }
};