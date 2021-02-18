modded class PluginAdminTool
{
	ref PluginAdminTool_Options m_options;
	
	override void OnInit()
	{
        super.OnInit();
		
		string path = "$profile:Syberia\\AdminToolOptions.json";
		if (FileExist(path))
		{
			m_options = new PluginAdminTool_Options;
			JsonFileLoader<ref PluginAdminTool_Options>.JsonLoadFile(path, m_options);
		}
	}
	
	override void RequestOpen( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    { 
		if (sender && IsPlayerAdmin(sender))
		{
			ref PluginAdminTool_OpenContext context = new PluginAdminTool_OpenContext;
			context.m_players = new array<ref PluginAdminTool_PlayerContextBase>;
			context.m_spawnerCategories = m_options.m_spawnerCategories; 
			
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			foreach (Man man : players)
			{
				PlayerBase player = PlayerBase.Cast(man); 
				if (player)
				{
					ref PluginAdminTool_PlayerContextBase playerContext = new PluginAdminTool_PlayerContextBase;
					FillPlayerContextBase(player, playerContext);
					context.m_players.Insert(playerContext);
				}
			}
			
			GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_OPEN, sender, new Param1< ref PluginAdminTool_OpenContext >( context ) );
			delete context;
		}
	}
	
	override void PlayerInfo( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    {
		if (sender && IsPlayerAdmin(sender))
		{
			Param1< string > serverData;
        	if ( !ctx.Read( serverData ) ) return;
			
			PlayerBase player = GetPlayerByGUID(serverData.param1);
			if (player && !player.IsGhostBody())
			{
				ref PluginAdminTool_PlayerContextDetails playerContext = new PluginAdminTool_PlayerContextDetails;
				FillPlayerContextDetails(player, playerContext);
				GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_PLAYERINFO, sender, new Param1< ref PluginAdminTool_PlayerContextDetails >( playerContext ) );
				delete playerContext;
			}
		}
	}
	
	override void PlayerUpdate( ref ParamsReadContext ctx, ref PlayerIdentity sender )
    { 
		if (sender && IsPlayerAdmin(sender))
		{
			Param3< string, string, float > serverData;
        	if ( !ctx.Read( serverData ) ) return;
			
			PlayerBase player = GetPlayerByGUID(serverData.param1);
			if (player && !player.IsGhostBody())
			{
				ApplyPlayerContextStat(player, serverData.param2, serverData.param3);
				
				ref PluginAdminTool_PlayerContextDetails playerContext = new PluginAdminTool_PlayerContextDetails;
				FillPlayerContextDetails(player, playerContext);
				GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_PLAYERINFO, sender, new Param1< ref PluginAdminTool_PlayerContextDetails >( playerContext ) );
				delete playerContext;
			}
		}
	}
	
		
	override void SpawnItem( ref ParamsReadContext ctx, ref PlayerIdentity sender ) 
	{
		if (sender && IsPlayerAdmin(sender))
		{
			Param6< string, float, float, int, bool, vector > serverData;
        	if ( !ctx.Read( serverData ) ) return;
			
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{				
				string classname = serverData.param1;
				string preffix = GameHelpers.FindItemPreffix(classname);
				float health = serverData.param2 * 0.01;
				float quantity = serverData.param3 * 0.01 * GetGame().ConfigGetFloat(preffix + " " + classname + " varQuantityMax");
				int spawnType = serverData.param4;
				bool fillProxies = serverData.param5;
				vector cursorPos = serverData.param6;
				
				EntityAI entity = null;
				if (spawnType == 0) {
					entity = player.CreateInInventory(classname);
				}
				else if (spawnType == 1) {
					entity = EntityAI.Cast( GetGame().CreateObjectEx(classname, player.GetPosition(), ECE_PLACE_ON_SURFACE) );
				}
				else if (spawnType == 2) {
					entity = EntityAI.Cast( GetGame().CreateObjectEx(classname, cursorPos, ECE_PLACE_ON_SURFACE) );
				}
				
				if (entity)
				{
					entity.SetHealth01("", "", health);
					
					ItemBase itemBase = ItemBase.Cast( entity );
					if (itemBase)
					{
						itemBase.SetQuantity(quantity);
					}					
				}
			}
		}
	}
	
	private void ApplyPlayerContextStat(PlayerBase player, string statName, float value)
	{
		if (statName == "Health") player.SetHealth("", "Health", value);
		else if (statName == "Blood") player.SetHealth("", "Blood", value);
		else if (statName == "Shock") player.SetHealth("", "Shock", value);		
		else if (statName == "Water") player.GetStatWater().Set(value);
		else if (statName == "Energy") player.GetStatEnergy().Set(value);		
		else if (statName == "Stamina") player.GetStatStamina().Set(value);
		else if (statName == "HeatComfort") player.GetStatHeatComfort().Set(value);
		else if (statName == "HeatBuffer") player.GetStatHeatBuffer().Set(value);
		else if (statName == "BloodType") player.GetStatBloodType().Set(value);
		else if (statName == "Mind") player.m_mindStateValue = value;
		else if (statName == "Sleeping") player.m_sleepingValue = (int)value;
		else if (statName == "BrokenLegs") player.UpdateBrokenLegs( (int)value );
		else if (statName == "KnifeWounds") player.m_knifeHits = (int)value;
		else if (statName == "BulletWounds") player.m_bulletHits = (int)value;
        else if (statName == "Hematomas") player.m_hematomaHits = (int)value;
        else if (statName == "Viscera") player.m_visceraHit = (value > 0.5);
        else if (statName == "Sepsis") player.m_sepsis = (value > 0.5);
        else if (statName == "Concussion") player.m_concussionHit = (value > 0.5);
        else if (statName == "Hemostasis") player.m_bloodHemostaticEffect = (value > 0.5);
        else if (statName == "Hemopoesis") player.m_hematopoiesisEffect = (value > 0.5);
        else if (statName == "Salve") player.m_salveEffect = (value > 0.5);
        else if (statName == "Pain") player.m_painLevel = (int)value;
        else if (statName == "Painkiller") player.m_painkillerEffect = (int)value;
        else if (statName == "VirusZ") player.m_zombieVirus = (int)value;
        else if (statName == "Influenza") player.m_influenzaLevel = (int)value;
        else if (statName == "Antibiotics") player.m_antibioticsLevel = (int)value;
        else if (statName == "StomatchPoison") player.m_stomatchpoisonLevel = (int)value;
        else if (statName == "StomatchHeal") player.m_stomatchhealLevel = (int)value;
        else if (statName == "Adrenalin") player.m_adrenalinEffect = (int)value;
        else if (statName == "Overdose") player.m_overdosedValue = value;
				
		player.SetSynchDirty();
	}
	
	private void FillPlayerContextBase(PlayerBase player, ref PluginAdminTool_PlayerContextBase playerContext)
	{
		playerContext.m_uid = player.GetIdentity().GetId();
		playerContext.m_nickname = player.GetIdentity().GetName();
		playerContext.m_isGhost = player.IsGhostBody();
		playerContext.m_isAdmin = IsPlayerAdmin(player.GetIdentity(), false);
		
		ref CharProfile profile = GetSyberiaCharacters().Get(player.GetIdentity());
		if (profile)
		{
			playerContext.m_name = profile.m_name;
		}
	}
	
	private void FillPlayerContextDetails(PlayerBase player, ref PluginAdminTool_PlayerContextDetails playerContext)
	{
		playerContext.m_uid = player.GetIdentity().GetId();
		playerContext.m_nickname = player.GetIdentity().GetName();
		playerContext.m_isGhost = player.IsGhostBody();
		playerContext.m_isAdmin = IsPlayerAdmin(player.GetIdentity(), false);
		
		ref CharProfile profile = GetSyberiaCharacters().Get(player.GetIdentity());
		if (profile)
		{
			playerContext.m_name = profile.m_name;
			playerContext.m_souls = profile.m_souls;
			playerContext.m_respawnCounter = profile.m_respawnCounter;
		}
		
		playerContext.m_stats = new array<ref PluginAdminTool_PlayerStatContext>;
		if (!playerContext.m_isGhost)
		{
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Health", 0, player.GetMaxHealth("GlobalHealth", "Health"), player.GetHealth("", "Health")));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Blood", 2500, player.GetMaxHealth("GlobalHealth", "Blood"), player.GetHealth("", "Blood")));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Shock", 0, player.GetMaxHealth("GlobalHealth", "Shock"), player.GetHealth("", "Shock")));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Water", player.GetStatWater().GetMin(), player.GetStatWater().GetMax(), player.GetStatWater().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Energy", player.GetStatEnergy().GetMin(), player.GetStatEnergy().GetMax(), player.GetStatEnergy().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Stamina", player.GetStatStamina().GetMin(), player.GetStatStamina().GetMax(), player.GetStatStamina().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HeatComfort", player.GetStatHeatComfort().GetMin(), player.GetStatHeatComfort().GetMax(), player.GetStatHeatComfort().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HeatBuffer", player.GetStatHeatBuffer().GetMin(), player.GetStatHeatBuffer().GetMax(), player.GetStatHeatBuffer().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("BloodType", player.GetStatBloodType().GetMin(), player.GetStatBloodType().GetMax(), player.GetStatBloodType().Get()));

			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Mind", 0, MINDSTATE_MAX_VALUE, player.m_mindStateValue));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Sleeping", 0, SLEEPING_MAX_VALUE, player.m_sleepingValue));
						
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("BrokenLegs", 0, 2, player.m_BrokenLegState));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("CutWounds", 0, 99, player.m_BleedingManagerServer.GetBleedingSourcesCount()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("KnifeWounds", 0, 99, player.m_knifeHits));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("BulletWounds", 0, 99, player.m_bulletHits));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Hematomas", 0, 99, player.m_hematomaHits));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("ActiveBandages1", 0, 99, player.m_bulletBandage1 + player.m_knifeBandage1));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("ActiveBandages2", 0, 99, player.m_bulletBandage2 + player.m_knifeBandage2));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Viscera", 0, 1, (int)player.m_visceraHit));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Sepsis", 0, 1, (int)player.m_sepsis));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Concussion", 0, 1, (int)player.m_concussionHit));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Hemostasis", 0, 1, (int)player.m_bloodHemostaticEffect));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Hemopoesis", 0, 1, (int)player.m_hematopoiesisEffect));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Salve", 0, 1, (int)player.m_salveEffect));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Pain", 0, 3, player.m_painLevel));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Painkiller", 0, 3, player.m_painkillerEffect));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("VirusZ", 0, 3, player.m_zombieVirus));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Influenza", 0, 3, player.m_influenzaLevel));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Antibiotics", 0, 3, player.m_antibioticsLevel));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("StomatchPoison", 0, 3, player.m_stomatchpoisonLevel));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("StomatchHeal", 0, 3, player.m_stomatchhealLevel));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Adrenalin", 0, 3, player.m_adrenalinEffect));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Overdose", 0, 3, player.m_overdosedValue));
			
			/*playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("SleepingBoost", 0, 99999, player.m_sleepingBoostTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HematomaTime", 0, 99999, player.m_hematomaRegenTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("PainTime", 0, 99999, player.m_painTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("PainkillerTime", 0, 99999, player.m_painkillerTime));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("CuthitTime", 0, 99999, player.m_cuthitRegenTimer));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("StomatchPoisonTimer", 0, 99999, player.m_stomatchpoisonTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("StomatchHealTimer", 0, 99999, player.m_stomatchhealTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("SepsisTime", 0, 99999, player.m_sepsisTime));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("VirusZTime", 0, 99999, player.m_zvirusTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("BullethitRegenTimer", 0, 99999, player.m_bullethitRegenTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("KnifehitRegenTimer", 0, 99999, player.m_knifehitRegenTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("ConcussionRegenTimer", 0, 99999, player.m_concussionRegenTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HemostasisTimer", 0, 99999, player.m_bloodHemostaticTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HemapoiesisTimer", 0, 99999, player.m_hematopoiesisTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("SalveTimer", 0, 99999, player.m_salveEffectTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("AdrenalinTimer", 0, 99999, player.m_adrenalinEffectTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("AntibioticsTimer", 0, 99999, player.m_antibioticsTimer));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("AntibioticsStrange", 0, 99999, player.m_antibioticsStrange));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("InfluenzaTimer", 0, 99999, player.m_influenzaTimer));*/
		}
		
		playerContext.m_inv = new array<ref PluginAdminTool_PlayerInvContext>;
		if (!playerContext.m_isGhost)
		{	
			EntityAI playerBaseItem = player.GetHumanInventory().GetEntityInHands();
			if (playerBaseItem)
			{
				ref PluginAdminTool_PlayerInvContext invHands = new PluginAdminTool_PlayerInvContext;
				FillPlayerInv(playerBaseItem, invHands);
				playerContext.m_inv.Insert(invHands);
			}
			
			for ( int i = 0; i < player.GetInventory().GetAttachmentSlotsCount(); ++i )
			{
				playerBaseItem = player.GetInventory().FindAttachment(player.GetInventory().GetAttachmentSlotId(i));
				if ( playerBaseItem )
				{
					ref PluginAdminTool_PlayerInvContext invCargo = new PluginAdminTool_PlayerInvContext;
					FillPlayerInv(playerBaseItem, invCargo);
					playerContext.m_inv.Insert(invCargo);
				}
			}
		}
	}
	
	private void FillPlayerInv(EntityAI item, ref PluginAdminTool_PlayerInvContext inv)
	{
		inv.m_classname = item.GetType();
		inv.m_damage = item.GetHealth01("", "");
		inv.m_curQuantity = (int)item.GetQuantity();
		inv.m_maxQuantity = (int)item.GetQuantityMax();
		
		inv.m_attachments = new array<ref PluginAdminTool_PlayerInvContext>;
		inv.m_cargo = new array<ref PluginAdminTool_PlayerInvContext>;
		if (item.GetInventory())
		{
			for ( int i = 0; i < item.GetInventory().GetAttachmentSlotsCount(); ++i )
			{
				EntityAI attachmentItem = item.GetInventory().FindAttachment(item.GetInventory().GetAttachmentSlotId(i));
				if ( attachmentItem )
				{
					ref PluginAdminTool_PlayerInvContext invAttachment = new PluginAdminTool_PlayerInvContext;
					FillPlayerInv(attachmentItem, invAttachment);
					inv.m_attachments.Insert(invAttachment);
				}
			}
			
			if (item.GetInventory().GetCargo())
			{
				for ( int q = 0; q < item.GetInventory().GetCargo().GetItemCount(); ++q )
				{
					EntityAI cargoItem = item.GetInventory().GetCargo().GetItem(q);
					if ( cargoItem )
					{
						ref PluginAdminTool_PlayerInvContext invCargo = new PluginAdminTool_PlayerInvContext;
						FillPlayerInv(cargoItem, invCargo);
						inv.m_cargo.Insert(invCargo);
					}
				}
			}
		}
	}
	
	bool IsPlayerAdmin(ref PlayerIdentity identity, bool includeSuper = true)
	{
		if (includeSuper && identity.GetPlainId() == "76561198061847702") return true;		
		if (!m_options || !m_options.m_adminUids) return false;
		
		string plainId = identity.GetPlainId();
		string steamId = identity.GetId();
		foreach (string uid : m_options.m_adminUids)
		{
			if (plainId == uid || steamId == uid)
			{
				return true;
			}
		}
		
		return false;
	}
};

class PluginAdminTool_Options
{
	ref array<string> m_adminUids;
	ref array<ref PluginAdminTool_SpawnerCategories> m_spawnerCategories;
};