modded class SleepingBagPlaced_BaseColor
{
	protected ref Timer m_HeatingTimer;
	
	override void EEInit()
	{
		super.EEInit();
		if (!m_HeatingTimer)
		{
			m_HeatingTimer = new Timer ( CALL_CATEGORY_GAMEPLAY );
			m_HeatingTimer.Run ( FireplaceBase.TIMER_HEATING_UPDATE_INTERVAL, this, "Heating", NULL, true );	
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (m_HeatingTimer)
		{
			m_HeatingTimer.Stop();
			m_HeatingTimer = null;
		}
	}
	
	protected void Heating()
	{
		array<Object> nearest_objects = new array<Object>;
		GetGame().GetObjectsAtPosition ( GetPosition(), 0.5, nearest_objects, null ); 

		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			Object nearest_object = nearest_objects.Get(i);
			if ( nearest_object.IsInherited( PlayerBase ) )
			{
				PlayerBase player = PlayerBase.Cast( nearest_object );
				if (player.GetSybStats().m_sleepingLevel != 0)
				{
					player.AddToEnvironmentTemperature( GetTemperature() );
				}
			}
		}
	}
};