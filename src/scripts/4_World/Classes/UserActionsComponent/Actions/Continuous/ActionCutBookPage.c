modded class ActionCutBookPage
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		float decreaseHealth = action_data.m_MainItem.GetMaxHealth() / BOOK_PAGES_COUNT_TO_CUT;
		action_data.m_MainItem.AddHealth("", "", decreaseHealth * -1);
		
		ItemBase paper;
		vector playerPos = action_data.m_Player.GetPosition();
		array<Object> nearbyObjects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(playerPos, 1, nearbyObjects, null);
		foreach (ref Object nearbyObject : nearbyObjects)
		{
			if (Paper.CastTo(paper, nearbyObject))
			{
				if (paper.GetQuantity() < paper.GetQuantityMax())
				{
					break;
				}
				else
				{
					paper = null;
				}
			}
		}
		
		if (paper)
		{
			paper.AddQuantity(1, false);
		}
		else
		{
			Class.CastTo(paper, GetGame().CreateObjectEx("Paper", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE) );
			paper.SetQuantity(1, false);
		}
		
		SyberiaSoundEmitter.Spawn("CutPage_SoundEmitter", playerPos);
	}
};