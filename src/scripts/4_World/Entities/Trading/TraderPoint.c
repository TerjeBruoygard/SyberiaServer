modded class TraderPoint
{
	Object m_traderObject;
	PlayerBase m_traderActiveUser;
	
	void InitTraderPoint(int id, Object traderObj)
	{
		m_traderId = id;
		m_traderObject = traderObj;
		m_ready = true;
	}
	
	void SetActiveUser(PlayerBase player)
	{
		m_traderActiveUser = player;
	}
	
	bool HasActiveUser()
	{
		return m_traderActiveUser != null;
	}
	
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
        if (rpc_type == SyberiaERPC.SYBERPC_SYNCH_TRADER_POINT_SERVER)
		{
			if (m_ready)
			{
				int objNetId1, objNetId2;
				m_traderObject.GetNetworkID(objNetId1, objNetId2);
				RPCSingleParam(SyberiaERPC.SYBERPC_SYNCH_TRADER_POINT_CLIENT, new Param3<int, int, int>(m_traderId, objNetId1, objNetId2), true, sender);
			}
		}
	}

    override Object GetTraderObject()
	{
		if (!m_ready) return null;
		return m_traderObject;
	}
	
	void OnTick()
	{
		if (!m_ready)
			return;
		
		if (m_traderActiveUser)
		{
			if (!m_traderActiveUser.IsAlive())
			{
				m_traderActiveUser = null;
			}
			else if (vector.Distance( m_traderActiveUser.GetPosition(), this.GetPosition() ) > 10)
			{
				m_traderActiveUser = null;
			}
		}
		
		if (m_traderObject)
		{
			m_traderObject.SetPosition( this.GetPosition() );
			m_traderObject.SetOrientation( this.GetOrientation() );
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		if (m_traderObject)
		{
			GetGame().ObjectDelete(m_traderObject);
			m_traderObject = null;
		}
	}
}