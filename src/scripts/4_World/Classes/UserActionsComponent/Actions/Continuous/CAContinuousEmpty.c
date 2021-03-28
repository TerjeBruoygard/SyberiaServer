modded class CAContinuousEmpty
{
	override void Setup( ActionData action_data )
	{
		super.Setup(action_data);
		
		if ( action_data.m_MainItem && !m_WringingClothes )
		{
			action_data.m_MainItem.SetWet( action_data.m_MainItem.GetWetMin() );
		}
	}
};