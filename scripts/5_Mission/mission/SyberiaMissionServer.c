class SyberiaMissionServer : MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_CREATENEWCHAR_REQUEST, this, "OnCreateNewCharRequest");
		SybLogSrv("Syberia server mission initialized");
	}
	
	override void OnEvent(EventType eventTypeId, Param params) 
	{		
		if (eventTypeId == ClientReadyEventTypeID)
		{		
			PlayerIdentity identity;
			PlayerBase player;
				
			ClientReadyEventParams readyParams;
			Class.CastTo(readyParams, params);
			
			identity = readyParams.param1;
			Class.CastTo(player, readyParams.param2);
			
			if (player.IsGhostBody())
			{
				DeleteGhostBody(identity, player);
				return;
			}
		}
		
		super.OnEvent(eventTypeId, params);
		
		if (eventTypeId == ClientNewEventTypeID)
		{
			ClientNewEventParams newParams;
			Class.CastTo(newParams, params);			
			OnNewPlayerConnected(newParams.param1);
		}
	}
	
	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		SybLogSrv("Create ghost body.");
		PlayerBase player = CreateCharacter(identity, "0 0 0", ctx, "Survivor_Ghost");
		player.SetPosition("-100000 0 -100000");
		player.SetAllowDamage(false);
		player.GetInputController().SetDisabled(true);
		return player;
	}
	
	override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
	{
		if (player.IsGhostBody())
		{
			DeleteGhostBody(identity, player);
			return;
		}
		
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
	}
	
	private void DeleteGhostBody(PlayerIdentity identity, PlayerBase player)
	{
		SybLogSrv("Delete ghost body.");
		InvokeOnDisconnect(player);
		
		if (GetHive())
		{
			GetHive().CharacterKill(player);		
		}
		
		player.ReleaseNetworkControls();
		player.SetAllowDamage(true);
		player.Delete();
		
		GetGame().DisconnectPlayer(identity);
		// Send list of players at all clients
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SyncEvents.SendPlayerList, 1000);
	}
	
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;

		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(4);

			player.GetInventory().CreateInInventory("Apple");
            player.GetInventory().CreateInInventory("Apple");
            player.GetInventory().CreateInInventory("M67Grenade");
            player.GetInventory().CreateInInventory("CombatKnife");
            player.GetInventory().CreateAttachment("TortillaBag");
            player.GetInventory().CreateAttachment("Saiga").GetInventory().CreateAttachment("Saiga_Bttstck");
            player.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
		}
	}
	
	override void ControlPersonalLight(PlayerBase player) { }
	
	private void OnNewPlayerConnected(ref PlayerIdentity identity)
	{
		ref CharProfile profile = GetSyberiaCharacters().Get(identity);
		if (profile)
		{
			auto respParams = new Param3<string, int, int>(profile.m_name, profile.m_souls, GetSyberiaOptions().m_respawnSoulsPrice);
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_RESPAWN_SCREEN_OPEN, identity, respParams);
			SybLogSrv("Send SYBRPC_RESPAWN_SCREEN_OPEN RPC.");
		}
		else
		{
			auto newcharParams = new Param2<string, int>(identity.GetName(), GetSyberiaOptions().m_newchar_points);
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_NEWCHAR_SCREEN_OPEN, identity, newcharParams);
			SybLogSrv("Send SYBRPC_NEWCHAR_SCREEN_OPEN RPC.");
		}
	}
	
	void OnCreateNewCharRequest(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST Received from " + sender);
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (!profile)
		{
			Param1<string> clientData;
       		if ( !ctx.Read( clientData ) ) return;	
			
			profile = new CharProfile;
			profile.m_name = clientData.param1;
			
			if (profile.m_name.LengthUtf8() > 32) {
				profile.m_name = profile.m_name.Substring(0, 32);
			}
			else if (profile.m_name.LengthUtf8() < 4)
			{
				profile.m_name = sender.GetName();
			}
			
			profile.m_souls = GetSyberiaOptions().m_startSoulsCount;
			GetSyberiaCharacters().Create(sender, profile);
			
			SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST char name " + profile.m_name);
			
			auto equipParams = new Param1<int>(0);
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_EQUIP_SCREEN_OPEN, sender, equipParams);
		}
		else
		{
			SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST Player kicked because already has profile: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
};