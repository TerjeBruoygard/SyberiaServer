modded class MissionServer
{
	ref array<ref CharacterMetadata> m_maleCharactersPool;
	ref array<ref CharacterMetadata> m_femaleCharactersPool;
	ref SkillsContainer m_defaultSkillsContainer;
	
	override void OnInit()
	{
		super.OnInit();
		
		m_maleCharactersPool = new array<ref CharacterMetadata>;
		m_femaleCharactersPool = new array<ref CharacterMetadata>;
		m_defaultSkillsContainer = SkillsContainer.Create();		
		
		GetMaleCharactersMetadata(m_maleCharactersPool);
		GetFemaleCharactersMetadata(m_femaleCharactersPool);
		
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_CREATENEWCHAR_REQUEST, this, "OnCreateNewCharRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_STARTGAME_REQUEST, this, "OnStartGameRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_RESPAWN_REQUEST, this, "OnRespawnRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_DELETECHAR_REQUEST, this, "OnDeleteCharRequest");
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_SKILLS_ACTIVATE, this, "OnSkillActivate");
		SybLogSrv("Syberia server mission initialized");
	}
	
	private void OnPlayerStarted(PlayerIdentity identity, PlayerBase player)
	{
		if (!player.IsGhostBody())
		{
			player.m_charProfile = GetSyberiaCharacters().Get(identity);
			if (!player.m_charProfile)
			{
				ForceRespawnPlayer(identity, player);
			}
			else
			{
				player.m_charProfile.m_skills.m_dirty = true;
				GetSyberiaOptions().SendClientOptions(identity, player.m_charProfile);
				PluginLogicPDA.Cast(GetPlugin(PluginLogicPDA)).SendPdaUserState(identity, player.m_charProfile);
			}
		}
	}
	
	override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player)
	{
		GetGame().SelectPlayer(identity, player);
		OnPlayerStarted(identity, player);
	}
	
	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		string classname;
		bool ghostMode = false;
		vector startPos = "0 0 0";
		ref array<string> startEquipClothes = null;
		ref array<string> startEquipGuns = null;
		ref array<string> startEquipItems = null;
		ref array<ref array<string>> allowedEquip = null;
		ref CharProfile profile = GetSyberiaCharacters().Get(identity);
		if (profile)
		{
			if (profile.m_needToForceRespawn)
			{
				classname = profile.m_classname;
				profile.m_needToForceRespawn = false;
				profile.m_respawnCounter = profile.m_respawnCounter + 1;
				
				if (profile.m_startGear)
				{					
					ref PluginSyberiaOptions_GroupFaction faction = GetSyberiaOptions().FindGroupByMember(profile.m_id);
					allowedEquip = GetSyberiaOptions().GetCharacterAllowedEquipment(identity, profile, faction);					
					if (allowedEquip.Count() == profile.m_startGear.Count())
					{
						int itemIndex = -1;
						int spawnPointId = profile.m_startGear.Get(SyberiaScreenEquipPages.SYBSEP_SPAWN_PAGE);
						startPos = GetSyberiaOptions().GetCharacterSpawnpoint(profile, faction, spawnPointId).CalculateSpawnpoint();
						
						startEquipClothes = new array<string>;						
						for (int i = SyberiaScreenEquipPages.SYBSEP_BODY_PAGE; i <= SyberiaScreenEquipPages.SYBSEP_BACKPACK_PAGE; i++)
						{
							itemIndex = profile.m_startGear.Get(i);
							if (itemIndex >= 0 && itemIndex < allowedEquip.Get(i).Count())
							{
								startEquipClothes.Insert(allowedEquip.Get(i).Get(itemIndex));
							}
						}
						
						startEquipGuns = new array<string>;
						itemIndex = profile.m_startGear.Get(SyberiaScreenEquipPages.SYBSEP_WEAPON_PAGE);
						if (itemIndex >= 0 && itemIndex < allowedEquip.Get(SyberiaScreenEquipPages.SYBSEP_WEAPON_PAGE).Count())
						{
							startEquipGuns.Insert(allowedEquip.Get(SyberiaScreenEquipPages.SYBSEP_WEAPON_PAGE).Get(itemIndex));
						}

						itemIndex = profile.m_startGear.Get(SyberiaScreenEquipPages.SYBSEP_ITEMS_PAGE);				
						startEquipItems = GetSyberiaOptions().GetCharacterLoadoutItems(profile, faction, itemIndex);
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
					allowedEquip = GetSyberiaOptions().GetCharacterAllowedEquipment(identity, profile, GetSyberiaOptions().FindGroupByMember(profile.m_id));
					auto equipParams = new Param1<ref array<ref array<string>>>(allowedEquip);
					GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_EQUIP_SCREEN_OPEN, identity, equipParams);
					SybLogSrv("Send SYBRPC_EQUIP_SCREEN_OPEN RPC.");
					
					foreach (ref array<string> equip2 : allowedEquip) delete equip2;
					delete allowedEquip;
					delete equipParams;
					
				}
				else
				{
					auto respParams = new Param4<string, int, int, int>(profile.m_name, profile.m_souls, GetSyberiaOptions().m_main.m_respawnSoulsPrice, GetSyberiaOptions().m_main.m_roleplay_mode);
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
			newcharParams.m_score = GetSyberiaOptions().m_main.m_newchar_points;
			newcharParams.m_maleCharsMetadata = m_maleCharactersPool;
			newcharParams.m_femaleCharsMetadata = m_femaleCharactersPool;
			newcharParams.m_skills = m_defaultSkillsContainer;

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
			else
			{
				if (startEquipClothes)
				{
					foreach (string itemNameClothes : startEquipClothes)
					{
						EquipItemIntoPlayer(player, itemNameClothes, false, -1);
					}
				}
				
				if (startEquipGuns)
				{
					foreach (string itemNameGun : startEquipGuns)
					{
						EquipItemIntoPlayer(player, itemNameGun, true, 0);
					}
				}
				
				if (startEquipItems)
				{
					foreach (string itemNameGear : startEquipItems)
					{
						EquipItemIntoPlayer(player, itemNameGear, false, -1);
					}
				}
			}
		}
		
		if (startEquipClothes) delete startEquipClothes;
		if (startEquipGuns) delete startEquipGuns;
		
		OnPlayerStarted(identity, player);
		
		return player;
	}
	
	private void EquipItemIntoPlayer(PlayerBase player, string data, bool inHands, int quickbarId)
	{
		string itemName, magName;
		float health, quantity;
		TStringArray attachments = new TStringArray();
		EquipItemData.Parse(data, itemName, attachments, health, quantity, magName);
		
		ItemBase entity = null;
		if (inHands)
		{
			entity = ItemBase.Cast( player.GetHumanInventory().CreateInHands(itemName) );
		}
		else
		{
			entity = ItemBase.Cast( player.CreateInInventory(itemName) );
		}
		
		if (!entity)
		{
			return;
		}
		
		foreach (string attachment : attachments)
		{
			entity.GetInventory().CreateAttachment(attachment);
		}
		
		if (health >= 0)
		{
			entity.SetHealth01("", "", health);
		}
		
		if (quantity >= 0)
		{
			entity.SetQuantity(quantity);
		}
		
		if (quickbarId >= 0)
		{
			player.SetQuickBarEntityShortcut( entity, quickbarId );
		}
		
		if (magName != "")
		{
			player.CreateInInventory(magName);
		}
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
		if (player)
		{
			if (player.IsGhostBody())
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
			else
			{
				GetSyberiaCharacters().Save(identity);
			}
		}
		
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
	}
	
	override bool InsertCorpse(Man player)
	{
		PlayerBase pbase = PlayerBase.Cast(player);
		if (pbase && pbase.IsGhostBody())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 10000, false, pbase);
			return true;
		}
		
		PluginAdminTool adminTool = PluginAdminTool.Cast(GetPlugin(PluginAdminTool));
		if (adminTool && !adminTool.m_corpsePtr) adminTool.m_corpsePtr = m_DeadPlayersArray;
		
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
	
	private void ForceRespawnPlayer(ref PlayerIdentity identity, ref PlayerBase player)
	{
		if (player)
		{
			player.SetAllowDamage(true);
			player.SetHealth("", "", 0);
			player.SetSynchDirty();
		}
		else
		{
			GetGame().DisconnectPlayer(identity);
			SybLogSrv("ForceRespawnPlayer PlayerBase not found.");
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
			
			string newFullName = clientData.param1.m_name;
			if (CharacterMetadata.ValidateCharacterNameFull(newFullName))
			{
				profile = new CharProfile();
				profile.m_uid = sender.GetId();
				profile.m_name = newFullName;
				profile.m_souls = GetSyberiaOptions().m_main.m_startSoulsCount;
							
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
				
				int charScore = GetSyberiaOptions().m_main.m_newchar_points;
				if (charScore < clientData.param1.m_skills.GetTotalScore()) 
				{
					GetGame().DisconnectPlayer(sender);
					return;
				}
				
				profile.m_skills = clientData.param1.m_skills;				
				GetSyberiaCharacters().Create(sender, profile, this, "OnCreateNewCharRequest_CreateChar");
				delete profile;	
			}
			else
			{
				OnCreateNewCharRequest_CreateChar(sender, false);
			}		

			delete clientData.param1;
		}
		else
		{
			SybLogSrv("SYBRPC_CREATENEWCHAR_REQUEST Player kicked because already has profile: " + sender);
			GetGame().DisconnectPlayer(sender);
		}
	}
	
	void OnCreateNewCharRequest_CreateChar(ref PlayerIdentity sender, bool success)
	{
		if (success)
		{
			SybLogSrv("Success to create new character");
			ForceRespawnPlayer(sender, GetPlayerByIdentity(sender));
		}
		else
		{
			SybLogSrv("Failed to create new character");
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_CREATENEWCHAR_ERROR, sender, new Param1<int>(0));			
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
			if (selectedIndexes.Count() != GetSyberiaOptions().GetCharacterAllowedEquipmentSize())
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
			int soulsPrice = GetSyberiaOptions().m_main.m_respawnSoulsPrice;
			if ((profile.m_souls - soulsPrice) < 0)
			{
				SybLogSrv("SYBRPC_STARTGAME_REQUEST Player kicked because no souls left to respawn: " + sender);
				GetGame().DisconnectPlayer(sender);
				return;
			}
			
			profile.m_needToConfigureGear = true;
			profile.m_souls = profile.m_souls - soulsPrice;
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
		if (GetSyberiaOptions().m_main.m_roleplay_mode == 1)
		{
			return;
		}
		
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
	
	void OnSkillActivate(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		ref CharProfile profile = GetSyberiaCharacters().Get(sender);
		if (profile && profile.m_skills)
		{
			Param2<int, int> clientData;
       		if ( !ctx.Read( clientData ) ) return;
			
			int perkId = clientData.param1;
			int perkLvl = clientData.param2;
			if ( profile.m_skills.CanActivatePerk(perkId, perkLvl) )
			{
				profile.m_skills.SetPerk(perkId, perkLvl);
			}
		}
	}
};