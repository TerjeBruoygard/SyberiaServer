modded class ActionTraderInteract
{
    override void OnEndServer( ActionData action_data )
	{
		if (action_data.m_Target && action_data.m_Target.GetObject())
		{
			TraderPoint traderPoint = FindTraderPoint(action_data.m_Target.GetObject().GetPosition());
			if (traderPoint && traderPoint.IsTraderReady() && traderPoint.GetTraderObject() == action_data.m_Target.GetObject() )
			{
				// Logic
			}
		}
	};
};