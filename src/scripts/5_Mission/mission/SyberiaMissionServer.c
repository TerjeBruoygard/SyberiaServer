class SyberiaMissionServer : MissionServer
{
	ref array<ref CharacterMetadata> m_maleCharactersPool;
	ref array<ref CharacterMetadata> m_femaleCharactersPool;
	ref SkillsContainer m_defaultSkillsContainer;
	ref array<ref PerkDescription> m_allPerksDesc;
	
	void SyberiaMissionServer()
	{
		m_maleCharactersPool = new array<ref CharacterMetadata>;
		m_femaleCharactersPool = new array<ref CharacterMetadata>;
		m_defaultSkillsContainer = CreateDefaultSkillsContainer();
		
		m_allPerksDesc = new array<ref PerkDescription>;
		GetPerkDescriptions(m_allPerksDesc);
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
	
	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		string classname;
		bool ghostMode = false;
		vector startPos = "0 0 0";
		ref array<string> startEquip = null;
		ref array<ref array<string>> allowedEquip = null;
		ref CharProfile profile = GetSyberiaCharacters().Get(identity);
		if (profile)
		{
			if (profile.m_needToForceRespawn)
			{
				classname = profile.m_classname;
				profile.m_needToForceRespawn = false;
				
				if (profile.m_startGear)
				{					
					allowedEquip = GetCharacterAllowedEquipment(identity);					
					if (allowedEquip.Count() == profile.m_startGear.Count())
					{
						int spawnPointId = profile.m_startGear.Get(0);
						if (spawnPointId >= 0 && spawnPointId < GetSyberiaOptions().m_spawnpoints.Count())
						{
							startPos = GetSyberiaOptions().m_spawnpoints.Get(spawnPointId).m_pos;
						}
						
						startEquip = new array<string>;
						for (int i = 1; i < profile.m_startGear.Count(); i++)
						{
							int itemIndex = profile.m_startGear.Get(i);
							if (itemIndex >= 0 && itemIndex < allowedEquip.Get(i).Count())
							{
								startEquip.Insert(allowedEquip.Get(i).Get(itemIndex));
							}
						}
					}
					
					foreach (ref array<string> equip1 : allowedEquip) delete equip1;
					delete allowedEquip;
					
					delete profile.m_startGear;
					profile.m_startGear = null;
				}
				
				GetSyberiaCharacters().Save(identity);				
			}
			else
			{
				ghostMode = true;
				classname = profile.m_classname + "_Ghost";
				
				if (profile.m_needToConfigureGear)
				{
					allowedEquip = GetCharacterAllowedEquipment(identity);
					auto equipParams = new Param1<ref array<ref array<string>>>(allowedEquip);
					GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_EQUIP_SCREEN_OPEN, identity, equipParams);
					SybLogSrv("Send SYBRPC_EQUIP_SCREEN_OPEN RPC.");
					
					foreach (ref array<string> equip2 : allowedEquip) delete equip2;
					delete allowedEquip;
					delete equipParams;
					
				}
				else
				{
					auto respParams = new Param3<string, int, int>(profile.m_name, profile.m_souls, GetSyberiaOptions().m_respawnSoulsPrice);
					GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_RESPAWN_SCREEN_OPEN, identity, respParams);
					SybLogSrv("Send SYBRPC_RESPAWN_SCREEN_OPEN RPC.");
					delete respParams;
				}
			}
		}
		else
		{
			classname = "SurvivorM_Mirek_Ghost";
			ghostMode = true;
			
			ref RpcNewCharContainer newcharParams = new RpcNewCharContainer();
			newcharParams.m_name = identity.GetName();
			newcharParams.m_score = GetSyberiaOptions().m_newchar_points;
			newcharParams.m_maleCharsMetadata = m_maleCharactersPool;
			newcharParams.m_femaleCharsMetadata = m_femaleCharactersPool;
			newcharParams.m_skillsContainer = m_defaultSkillsContainer;

			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_NEWCHAR_SCREEN_OPEN, identity, new Param1<ref RpcNewCharContainer>(newcharParams));
			delete newcharParams;
			SybLogSrv("Send SYBRPC_NEWCHAR_SCREEN_OPEN RPC.");
		}
		
		PlayerBase player = CreateCharacter(identity, startPos, ctx, classname);
		if (player)
		{
			if (ghostMode)
			{
				player.SetPosition("-100000 -100 -100000");
				player.GetInputController().SetDisabled(true);
				player.SetAllowDamage(false);
			}
			else if (startEquip)
			{
				foreach (string itemName : startEquip)
				{
					player.CreateInInventory(itemName);
				}
			}
		}
		
		if (startEquip) delete startEquip;
		
		return player;
	}
	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		super.OnEvent(eventTypeId, params);
		
		if (eventTypeId == ClientReadyEventTypeID)
		{
			ClientReadyEventParams readyParams;
			Class.CastTo(readyParams, params);
			
			PlayerIdentity identity = readyParams.param1;
			PlayerBase player = GetPlayerByIdentity(identity);
			if (player && player.IsGhostBody())
			{
				ForceRespawnPlayer(identity, player);
			}
		}
	}
	
	override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
	{
		if (player && player.IsGhostBody())
		{
			InvokeOnDisconnect(player);
		
			player.SetAllowDamage(true);
			player.SetHealth("", "", 0);			
			if (GetHive())
			{
				GetHive().CharacterKill(player);		
			}
			
			player.ReleaseNetworkControls();
			player.Delete();
			SybLogSrv("Delete ghost player");
			
			GetGame().DisconnectPlayer(identity);
			// Send list of players at all clients
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SyncEvents.SendPlayerList, 1000);
			return;
		}
		
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
	}
	
	override bool InsertCorpse(Man player)
	{
		PlayerBase pbase = PlayerBase.Cast(player);
		if (pbase && pbase.IsGhostBody())
		{
			SybLogSrv("Delete ghost player");
			pbase.Delete();
			return false;
		}
		
		return super.InsertCorpse(player);
	}
	
	override void ControlPersonalLight(PlayerBase player) 
	{
		// DO NOTHING
	}
	
		
	override void EquipCharacter(MenuDefaultCharacterData char_data) 
	{
		// DO NOTHING
	}
	
	private ref PlayerBase GetPlayerByIdentity(ref PlayerIdentity identity)
	{
		ref PlayerBase result = null;
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (ref Man man : players)
		{
			ref PlayerBase player = PlayerBase.Cast(man);
			if (player && player.GetIdentity().GetPlayerId() == identity.GetPlayerId())
			{
				result = player;
				break;
			}
		}
		
		delete players;
		return result;
	}
	
	private void ForceRespawnPlayer(ref PlayerIdentity identity, ref PlayerBase player)
	{
		if (player)
		{
			player.SetAllowDamage(true);
			player.SetHealth("", "", 0);
		}
		else
		{
			GetGame().DisconnectPlayer(identity);
			SybLogSrv("ForceRespawnPlayer PlayerBase not found.");
		}
	}
	
	private int GetCharacterAllowedEquipmentSize()
	{
		return 8;
	}
	
	private ref array<ref array<string>> GetCharacterAllowedEquipment(ref PlayerIdentity identity)
	{
		ref array<ref array<string>> result = new array<ref array<string>>;
		
		// SPAWNPOINTS
		ref array<string> spawnPoints = new array<string>;
		foreach (ref SpawnpointInfo sp : GetSyberiaOptions().m_spawnpoints)
		{
			spawnPoints.Insert(sp.m_name);
		}		
		result.Insert(spawnPoints);
		
		// GEAR BODY
		ref array<string> bodyGear = new array<string>;
		bodyGear.Copy(GetSyberiaOptions().m_defaultGearBody);
		result.Insert(bodyGear);
		
		// GEAR PANTS
		ref array<string> pantsGear = new array<string>;
		pantsGear.Copy(GetSyberiaOptions().m_defaultGearPants);
		result.Insert(pantsGear);
		
		// GEAR FOOT
		ref array<string> footGear = new array<string>;
		footGear.Copy(GetSyberiaOptions().m_defaultGearFoot);
		result.Insert(footGear);
		
		// GEAR HEAD
		ref array<string> headGear = new array<string>;
		headGear.Copy(GetSyberiaOptions().m_defaultGearHead);
		result.Insert(headGear);
		
		// GEAR WEAPON
		ref array<string> weapGear = new array<string>;
		weapGear.Copy(GetSyberiaOptions().m_defaultGearWeapon);
		result.Insert(weapGear);
		
		// GEAR ITEMS
		ref array<string> itemsGear = new array<string>;
		itemsGear.Copy(GetSyberiaOptions().m_defaultGearItems);
		result.Insert(itemsGear);
		
		// GEAR SPECIAL
		ref array<string> specGear = new array<string>;
		result.Insert(specGear);
		
		return result;
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
			
			int charScore = GetSyberiaOptions().m_newchar_points;
			foreach (int perkId : clientData.param1.m_perks)
			{
				if (perkId < 0 || perkId >= m_allPerksDesc.Count())
				{
					GetGame().DisconnectPlayer(sender);
					return;
				}
				
				ref PerkDescription perkDesc = m_allPerksDesc.Get(perkId);
				charScore = charScore - perkDesc.m_cost;
			}
			if (charScore < 0) 
			{
				GetGame().DisconnectPlayer(sender);
				return;
			}
			
			profile.m_skills = CreateDefaultSkillsContainer();
			profile.m_skills.m_perks.Copy(clientData.param1.m_perks);
			
			GetSyberiaCharacters().Create(sender, profile);			
			ForceRespawnPlayer(sender, GetPlayerByIdentity(sender));

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
		if (profile && profile.m_needToConfigureGear)
		{
			Param1<ref array<int>> clientData;
       		if ( !ctx.Read( clientData ) ) return;	
			
			ref array<int> selectedIndexes = clientData.param1;
			if (selectedIndexes.Count() != GetCharacterAllowedEquipmentSize())
			{
				GetGame().DisconnectPlayer(sender);
				return;
			}
			
			if (profile.m_startGear) delete profile.m_startGear;
			profile.m_startGear = new ref array<int>;
			profile.m_startGear.Copy(selectedIndexes);
						
			delete selectedIndexes;
			delete clientData;
			
			profile.m_needToConfigureGear = false;
			profile.m_needToForceRespawn = true;
			GetSyberiaCharacters().Save(sender);			
			ForceRespawnPlayer(sender, GetPlayerByIdentity(sender));
		}
		else
		{
			SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because profile not found or invalid: " + sender);
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
			ForceRespawnPlayer(sender, GetPlayerByIdentity(sender));
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
			ForceRespawnPlayer(sender, GetPlayerByIdentity(sender));
		}
		else
		{
			SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because profile not found: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
};