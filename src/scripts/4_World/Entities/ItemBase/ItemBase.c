modded class ItemBase
{
	override void InitItemVariables()
	{
		super.InitItemVariables();
		
		if (CanHaveTemperature())
		{
			Mission mission = GetGame().GetMission();
			if (mission && mission.GetWorldData())
			{
				SetTemperature( mission.GetWorldData().GetBaseEnvTemperature() );
			}
		}
	}
	
	bool CanHaveTemperature_SybInternalAccess()
	{
		return CanHaveTemperature();
	}
	
	override void ProcessItemTemperature( float delta, bool hasParent, bool hasRootAsPlayer, ItemBase refParentIB )
	{
		if (!CanHaveTemperature())
			return;
		
		if ( !hasRootAsPlayer )
		{
			Mission mission = GetGame().GetMission();
			if (!mission || !mission.GetWorldData())
				return;
			
			float envTemperature = mission.GetWorldData().GetBaseEnvTemperature();
			float itemTemperature = GetTemperature();
			if ( !hasParent )
			{
				// cooling on ground
				if ( !IsFireplace() )
				{
					if (itemTemperature > envTemperature) AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_GROUND );
					else AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_GROUND * -1.0 );
				}
			}
			else if ( refParentIB )
			{
				// cooling of an item inside other
				if ( !IsFireplace() )
				{
					if (refParentIB.CanHaveTemperature_SybInternalAccess())
					{
						float parentTemperature = refParentIB.GetTemperature();
						if (itemTemperature > parentTemperature) AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE );
						else AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE * -1.0 );
					}
					else
					{
						if (itemTemperature > envTemperature) AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE );
						else AddTemperature( delta * GameConstants.TEMPERATURE_RATE_COOLING_INSIDE * -1.0 );
					}
				}
			}
		}
	}
};