class SyberiaMissionServer : MissionServer
{
	ref array<ref CharacterMetadata> m_maleCharactersPool;
	ref array<ref CharacterMetadata> m_femaleCharactersPool;
	
	void SyberiaMissionServer()
	{
		m_maleCharactersPool = new array<ref CharacterMetadata>;
		m_femaleCharactersPool = new array<ref CharacterMetadata>;
	}
	
	void ~SyberiaMissionServer()
	{
		foreach (ref CharacterMetadata mmtd : m_maleCharactersPool)
		{
			delete mmtd;
		}
		delete m_maleCharactersPool;
		
		foreach (ref CharacterMetadata fmtd : m_femaleCharactersPool)
		{
			delete fmtd;
		}
		delete m_femaleCharactersPool;
	}
	
	override void OnInit()
	{
		super.OnInit();
		
		GetMaleCharactersMetadata(m_maleCharactersPool);
		GetFemaleCharactersMetadata(m_femaleCharactersPool);
		
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_CREATENEWCHAR_REQUEST, this, "OnCreateNewCharRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_STARTGAME_REQUEST, this, "OnStartGameRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_RESPAWN_REQUEST, this, "OnRespawnRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_DELETECHAR_REQUEST, this, "OnDeleteCharRequest");
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
		string classname;
		bool ghostMode = false;
		ref CharProfile profile = GetSyberiaCharacters().Get(identity);
		if (profile)
		{
			if (profile.m_needToForceRespawn)
			{
				classname = profile.m_classname;
				profile.m_needToForceRespawn = false;
				GetSyberiaCharacters().Save(identity);
				
				auto requestParams = new Param1<int>(0);
				GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_CLOSE_LOGIN_SCREEN, identity, requestParams);
				SybLogSrv("Send SYBRPC_CLOSE_LOGIN_SCREEN RPC.");				
			}
			else
			{
				ghostMode = true;
				classname = profile.m_classname + "_Ghost";
				profile.m_needToConfigureGear = true;
				GetSyberiaCharacters().Save(identity);
			}
		}
		else
		{
			classname = "SurvivorM_Mirek_Ghost";
			ghostMode = true;
		}
		
		PlayerBase player = CreateCharacter(identity, "0 0 0", ctx, classname);
		if (player)
		{
			if (ghostMode)
			{
				player.SetPosition("-100000 -100 -100000");
				player.SetAllowDamage(false);
				player.GetInputController().SetDisabled(true);
			}
		}
		
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
	
	PlayerBase GetPlayerByIdentity(ref PlayerIdentity identity)
	{
		ref PlayerBase result = null;
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		
		foreach (Man man : players)
		{
			ref PlayerBase player = PlayerBase.Cast(man);
			if (player && player.GetIdentity() == identity)
			{
				result = player;
				break;
			}
		}
		
		delete players;
		return result;
	}
	
	ref PlayerBase RecreatePlayer(ref PlayerIdentity identity)
	{
		ref PlayerBase player = GetPlayerByIdentity(identity);
		if (player)
		{
			if (GetHive())
			{
				GetHive().CharacterKill(player);
			}
			
			GetGame().ObjectDelete(player);
		}
		
		player = OnClientNewEvent(identity, "0 0 0", null);
		if (player && GetHive())
		{
			GetHive().CharacterSave(player);
		}
		
		return player;
	}
	
	private void DeleteGhostBody(PlayerIdentity identity, PlayerBase player)
	{
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
		
	override void ControlPersonalLight(PlayerBase player) { }
	
	private void OnNewPlayerConnected(ref PlayerIdentity identity)
	{
		ref CharProfile profile = GetSyberiaCharacters().Get(identity);
		if (profile)
		{
			if (profile.m_needToConfigureGear)
			{
				auto equipParams = new Param1<int>(0);
				GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_EQUIP_SCREEN_OPEN, identity, equipParams);
				SybLogSrv("Send SYBRPC_EQUIP_SCREEN_OPEN RPC.");
			}
			else
			{
				auto respParams = new Param3<string, int, int>(profile.m_name, profile.m_souls, GetSyberiaOptions().m_respawnSoulsPrice);
				GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_RESPAWN_SCREEN_OPEN, identity, respParams);
				SybLogSrv("Send SYBRPC_RESPAWN_SCREEN_OPEN RPC.");
			}
		}
		else
		{
			ref RpcNewCharContainer newcharParams = new RpcNewCharContainer();
			newcharParams.m_name = identity.GetName();
			newcharParams.m_score = GetSyberiaOptions().m_newchar_points;
			newcharParams.m_maleCharsMetadata = m_maleCharactersPool;
			newcharParams.m_femaleCharsMetadata = m_femaleCharactersPool;

			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_NEWCHAR_SCREEN_OPEN, identity, new Param1<ref RpcNewCharContainer>(newcharParams));
			delete newcharParams;
			SybLogSrv("Send SYBRPC_NEWCHAR_SCREEN_OPEN RPC.");
		}
	}
	
	void OnCreateNewCharRequest(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST Received from " + sender);
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (!profile)
		{
			Param1<ref RpcCreateNewCharContainer> clientData;
       		if ( !ctx.Read( clientData ) ) return;	
			
			profile = new CharProfile(clientData.param1.m_name, GetSyberiaOptions().m_startSoulsCount);			
			if (clientData.param1.m_isMale)
			{
				if (clientData.param1.m_faceId >= 0 && clientData.param1.m_faceId < m_maleCharactersPool.Count())
				{
					profile.m_classname = m_maleCharactersPool.Get(clientData.param1.m_faceId).m_classname;
				}
				else
				{
					GetGame().DisconnectPlayer(sender);
					return;
				}
			}
			else
			{
				if (clientData.param1.m_faceId >= 0 && clientData.param1.m_faceId < m_femaleCharactersPool.Count())
				{
					profile.m_classname = m_femaleCharactersPool.Get(clientData.param1.m_faceId).m_classname;
				}
				else
				{
					GetGame().DisconnectPlayer(sender);
					return;
				}
			}

			if (profile.m_name.LengthUtf8() > 32) {
				profile.m_name = profile.m_name.Substring(0, 32);
			}
			else if (profile.m_name.LengthUtf8() < 4)
			{
				profile.m_name = sender.GetName();
			}
			
			GetSyberiaCharacters().Create(sender, profile);			
			RecreatePlayer(sender);
			OnNewPlayerConnected(sender);
				
			delete clientData.param1;
		}
		else
		{
			SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST Player kicked because already has profile: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
	
	void OnStartGameRequest(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		SybLogSrv("SYBRPC_STARTGAME_REQUEST Received from " + sender);
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (profile)
		{
			profile.m_needToConfigureGear = false;
			profile.m_needToForceRespawn = true;
			GetSyberiaCharacters().Save(sender);			
			RecreatePlayer(sender);
		}
		else
		{
			SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because profile not found: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
	
	void OnRespawnRequest(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		SybLogSrv("SYBRPC_RESPAWN_REQUEST Received from " + sender);
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (profile)
		{
			if (profile.m_souls <= 0)
			{
				SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because no souls left to respawn: " + sender);
				GetGame().DisconnectPlayer(sender);
				return;
			}
			
			profile.m_needToConfigureGear = true;
			profile.m_souls = profile.m_souls - 1;
			GetSyberiaCharacters().Save(sender);			
			RecreatePlayer(sender);
			OnNewPlayerConnected(sender);
		}
		else
		{
			SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because profile not found: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
	
	void OnDeleteCharRequest(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		SybLogSrv("SYBRPC_DELETECHAR_REQUEST Received from " + sender);
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (profile)
		{
			GetSyberiaCharacters().Delete(sender);
			OnNewPlayerConnected(sender);
		}
		else
		{
			SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because profile not found: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
};