modded class ActionCheckName
{
	override void OnEndServer( ActionData action_data )
	{
		string message;
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		if (ntarget && ntarget.m_charProfile)
		{
			if (ntarget.IsMale())
			{
				message = "#syb_inspect_msg1 " + ntarget.m_charProfile.m_name;
			}
			else
			{
				message = "#syb_inspect_msg2 " + ntarget.m_charProfile.m_name;
			}
		}
		else
		{
			message = "#syb_inspect_msg3";
		}
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, action_data.m_Player.GetIdentity(), new Param1<string>(message));		
	}
};

modded class ActionSayName
{
	override void OnEndServer( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		if (ntarget && ntarget.IsAlive() && action_data.m_Player.m_charProfile)
		{
			string message = "#syb_ur_greeted " + action_data.m_Player.m_charProfile.m_name;
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, ntarget.GetIdentity(), new Param1<string>(message));
		}	
	}
};