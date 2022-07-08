modded class SyberiaActionPlacementApply
{
	override void OnEndServer( ActionData action_data )
	{
		if (action_data.m_State != UA_FINISHED)
			return;
		
		SyberiaActionPlacementData poActionData;
		poActionData = SyberiaActionPlacementData.Cast(action_data);
		if ( !poActionData || !action_data.m_MainItem ) return;
		
		vector pos = poActionData.m_Position;
		vector rot = poActionData.m_Orientation;
		
		if ( action_data.m_Player.IsPlacingServer() )
		{
			action_data.m_Player.PlacingCompleteServer();
		}
		
		float health = action_data.m_MainItem.GetHealth01("", "");
		string itemName = action_data.m_MainItem.GetType();
		action_data.m_MainItem.Delete();
		
		string placedName;
		if ( GetGame().ConfigGetText("CfgVehicles " + itemName + " placedName", placedName) )
		{
			EntityAI placedObj = EntityAI.Cast( GetGame().CreateObjectEx( placedName, pos, ECE_PLACE_ON_SURFACE ) );
			if (placedObj)
			{
				placedObj.SetPosition( pos );
				placedObj.SetOrientation( rot );
				placedObj.SetHealth01("", "", health);
			}
		}
		
		string placedSound;
		if ( GetGame().ConfigGetText("CfgVehicles " + placedName + " placedSound", placedSound) )
		{
			SyberiaSoundEmitter.Spawn(placedSound, pos);
		}
	}
};