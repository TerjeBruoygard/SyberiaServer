modded class ActionSleep
{
	override void OnEndServer( ActionData action_data ) 
	{
		Object target = Object.Cast(action_data.m_Target.GetObject());
		if (target)
		{
			vector pos = target.GetPosition();
			string sleepingSound;
			if ( GetGame().ConfigGetText("CfgVehicles " + target.GetType() + " sleepingSound", sleepingSound) )
			{
				SyberiaSoundEmitter.Spawn(sleepingSound, pos);
			}
		}
	}
};