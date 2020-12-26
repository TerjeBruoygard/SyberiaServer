modded class ActionFoldBaseBuildingObject
{
    override void OnFinishProgressServer( ActionData action_data ) 
	{
		Object target = Object.Cast(action_data.m_Target.GetObject());
		if (target)
		{
			vector pos = target.GetPosition();
			string foldingSound;
			if ( GetGame().ConfigGetText("CfgVehicles " + target.GetType() + " foldingSound", foldingSound) )
			{
				SyberiaSoundEmitter.Spawn(foldingSound, pos);
			}
		}
        
        super.OnFinishProgressServer(action_data);
	}
};