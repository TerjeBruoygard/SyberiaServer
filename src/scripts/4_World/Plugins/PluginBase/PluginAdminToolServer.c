modded class PluginAdminTool
{
	ref PluginAdminTool_Options m_options;
	ref array<ref CorpseData> m_corpsePtr;
	
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
			Param1< ref PluginAdminTool_SpawnItemContext > serverData;
        	if ( !ctx.Read( serverData ) ) return;
			ref PluginAdminTool_SpawnItemContext context = serverData.param1;
			
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{				
				string preffix = GameHelpers.FindItemPreffix(context.m_classname);
				float health = context.m_health * 0.01;
				float quantity = context.m_quantity * 0.01;
				int spawnType = context.m_spawnType;
				vector cursorPos = context.m_cursorPos;
				
				EntityAI entity = null;
				if (spawnType == 0) {
					entity = player.CreateInInventory(context.m_classname);
				}
				else if (spawnType == 1) {
					entity = EntityAI.Cast( GetGame().CreateObjectEx(context.m_classname, player.GetPosition(), ECE_PLACE_ON_SURFACE) );
				}
				else if (spawnType == 2) {
					entity = EntityAI.Cast( GetGame().CreateObjectEx(context.m_classname, cursorPos, ECE_PLACE_ON_SURFACE) );
				}
				
				if (entity)
				{
					entity.SetHealth01("", "", health);
					
					ItemBase itemBase = ItemBase.Cast( entity );
					if (itemBase)
					{
						itemBase.SetQuantity(quantity * GetGame().ConfigGetFloat(preffix + " " + context.m_classname + " varQuantityMax"));
					}	
					
					Magazine magazine = Magazine.Cast( entity );
					if (magazine)
					{
						float ammoCount = quantity * GetGame().ConfigGetFloat(preffix + " " + context.m_classname + " count");
						magazine.ServerSetAmmoCount((int)ammoCount);
					}	
					
					Car car = Car.Cast( entity );
					if (car)
					{
						car.Fill(CarFluid.FUEL, car.GetFluidCapacity(CarFluid.FUEL));
						car.Fill(CarFluid.OIL, car.GetFluidCapacity(CarFluid.OIL));
						car.Fill(CarFluid.BRAKE, car.GetFluidCapacity(CarFluid.BRAKE));
						car.Fill(CarFluid.COOLANT, car.GetFluidCapacity(CarFluid.COOLANT));
					}
					
					if (context.m_attachments && entity.GetInventory())
					{
						foreach (string attachment : context.m_attachments)
						{
							EntityAI attachmentItem = entity.GetInventory().CreateAttachment(attachment);
							if (GameHelpers.HasBatterySlot(attachment))
							{
								attachmentItem.GetInventory().CreateAttachment("Battery9V");
							}
						}
					}			
				}
			}
			
			delete context;
		}
	}
	
	override void ClearItems( ref ParamsReadContext ctx, ref PlayerIdentity sender ) 
	{
		if (sender && IsPlayerAdmin(sender))
		{
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{
				array<Object> objects = new array<Object>;
				GetGame().GetObjectsAtPosition3D(player.GetPosition(), 5, objects, null);
				foreach (Object obj : objects)
				{
					ItemBase item = ItemBase.Cast(obj);
					if (item)
					{
						GetGame().ObjectDelete(item);
						continue;
					}
					
					Container_Base container = Container_Base.Cast(obj);
					if (container)
					{
						GetGame().ObjectDelete(container);
						continue;
					}
					
					Car car = Car.Cast(obj);
					if (car)
					{
						GetGame().ObjectDelete(car);
						continue;
					}
				}
			}
		}
	}
	
	override void UpdateMap( ref ParamsReadContext ctx, ref PlayerIdentity sender ) 
	{
		if (sender && IsPlayerAdmin(sender))
		{
			ref PluginAdminTool_MapContext context = new PluginAdminTool_MapContext;
			FillMapContext(context, true, true, true);			
			GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_UPDATEMAP, sender, new Param1<ref PluginAdminTool_MapContext>(context) );
			delete context;
		}
	}
	
	private void FillMapContext(ref PluginAdminTool_MapContext context, bool players, bool corpses, bool vehicles)
	{
		context.m_playerPositions = new array<vector>;
		context.m_playerNames = new array<string>;
		context.m_bodiesPositions = new array<vector>;
		context.m_bodiesNames = new array<string>;
		context.m_vehiclePositions = new array<vector>;
		context.m_vehicleNames = new array<string>;
		
		if (players)
		{
			array<Man> playersArray = new array<Man>;
			GetGame().GetPlayers(playersArray);
			foreach (Man man : playersArray)
			{
				PlayerBase player = PlayerBase.Cast(man);
				if (player && player.m_charProfile)
				{
					context.m_playerPositions.Insert(player.GetPosition());
					context.m_playerNames.Insert(player.m_charProfile.m_name);
				}
			}
		}
		
		if (corpses && m_corpsePtr)
		{
			foreach (ref CorpseData corpse : m_corpsePtr)
			{
				if (corpse.m_Player && corpse.m_Player.m_charProfile)
				{
					context.m_bodiesPositions.Insert(corpse.m_Player.GetPosition());
					context.m_bodiesNames.Insert(corpse.m_Player.m_charProfile.m_name);
				}
			}	
		}	
		
		if (vehicles)
		{
			array<CarScript> cars = new array<CarScript>;
			CarScript.GetAllVehicles(cars);
			foreach (CarScript car : cars)
			{
				context.m_vehiclePositions.Insert(car.GetPosition());
				context.m_vehicleNames.Insert(car.GetType());
			}
		}
	}
	
	override void Teleport( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
		if (sender && IsPlayerAdmin(sender))
		{
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{
				Param2< vector, int > serverData;
	        	if ( !ctx.Read( serverData ) ) return;

				TeleportFnc(player, serverData.param1, serverData.param2);				
				UpdateMap(null, sender);
			}
		}
	}
	
	private void TeleportFnc(ref PlayerBase player, vector pos, int mode)
	{
		vector teleportPos = pos;
		if (mode == 0)
		{	
			teleportPos[1] = GetGame().SurfaceY(teleportPos[0], teleportPos[2]);
		}
		else if (mode == 1)
		{
			if (!player.m_freeCamMode)
				return;
			
			teleportPos[1] = GetGame().SurfaceY(teleportPos[0], teleportPos[2]) - 50;
			player.GetStatHeatBuffer().Set(player.GetStatHeatBuffer().GetMax());
		}
					
		Transport veh = GameHelpers.GetPlayerVehicle(player);
		if ( veh )
		{
			vector mat[4];
			veh.GetTransform(mat);
			mat[3] = teleportPos;
			veh.SetTransform(mat);
		}
		else
		{
			player.SetPosition(teleportPos);
		}
	}
	
	override void FreeCam( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
		if (sender && IsPlayerAdmin(sender))
		{
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{
				Param1< bool > serverData;
	        	if ( !ctx.Read( serverData ) ) return;
				player.m_freeCamMode = serverData.param1;
				
				if (serverData.param1)
				{
					TeleportFnc(player, player.GetPosition(), 1);
				}
				else
				{
					TeleportFnc(player, player.GetPosition(), 0);
				}
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetPlayerAllowDamage, 1500, false, player, !serverData.param1);
				GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_FREECAM, sender, serverData );
			}
		}
	}
	
	override void EspSynch( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
		if (sender && IsPlayerAdmin(sender))
		{
			PlayerBase player = GetPlayerByGUID(sender.GetId());
			if (player)
			{
				Param3< bool, bool, bool > serverData;
	        	if ( !ctx.Read( serverData ) ) return;
				
				bool synchPlayers = serverData.param1;
				bool synchBodies = serverData.param2;
				bool synchVehs = serverData.param3;
				ref PluginAdminTool_MapContext context = new PluginAdminTool_MapContext;
				FillMapContext(context, synchPlayers, synchBodies, synchVehs);					
				GetSyberiaRPC().SendToClient( SyberiaRPC.SYBRPC_ADMINTOOL_ESPSYNCH, sender, new Param1< ref PluginAdminTool_MapContext >(context) );
				delete context;
			}
		}
	}
	
	override void ObjMove( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
		if (sender && IsPlayerAdmin(sender))
		{
			Param3<Object, int, float> serverData;
			if ( !ctx.Read( serverData ) ) return;
			if ( !serverData.param1 ) return;
			
			Object obj = serverData.param1;
			int axis = serverData.param2;
			float value = serverData.param3;
			vector pos = obj.GetPosition();
			vector rot = obj.GetOrientation();
			if (axis == 0) pos[0] = pos[0] + value;
			else if (axis == 1) pos[1] = pos[1] + value;
			else if (axis == 2) pos[2] = pos[2] + value;
			else if (axis == 3) rot[0] = rot[0] + (value * 10);
			else if (axis == 4) rot[1] = rot[1] + (value * 10);
			else if (axis == 5) rot[2] = rot[2] + (value * 10);

			obj.SetPosition(pos);
			obj.SetOrientation(rot);
		}
	}
	
	override void ObjDel( ref ParamsReadContext ctx, ref PlayerIdentity sender )
	{
		if (sender && IsPlayerAdmin(sender))
		{
			Param1<Object> serverData;
			if ( !ctx.Read( serverData ) ) return;
			if ( !serverData.param1 ) return;			
			GetGame().ObjectDelete( serverData.param1 );
		}
	}
	
	private void SetPlayerAllowDamage(PlayerBase player, bool value)
	{
		if (player) player.SetAllowDamage(value);
	}

	private void ApplyPlayerContextStat(PlayerBase player, string statName, float value)
	{
		if (!player) return;
		
		ref CharProfile profile = GetSyberiaCharacters().Get(player.GetIdentity());
		if (!profile || !profile.m_skills) return;
		
		if (statName == "Health") player.SetHealth("", "Health", value);
		else if (statName == "Blood") player.SetHealth("", "Blood", value);
		else if (statName == "Shock") player.SetHealth("", "Shock", value);		
		else if (statName == "Water") player.GetStatWater().Set(value);
		else if (statName == "Energy") player.GetStatEnergy().Set(value);	
		else if (statName == "Mind") player.m_mindStateValue = value;
		else if (statName == "Sleeping") player.m_sleepingValue = (int)value;	
		else if (statName == "HeatBuffer") player.GetStatHeatBuffer().Set(value);
		else if (statName == "BrokenLegs") player.UpdateBrokenLegs( (int)value );
		else if (statName == "CutWounds")
		{
			if (player.GetBleedingManagerServer())
			{
				player.GetBleedingManagerServer().RemoveAllSources();
				if (value > 0)
				{
					player.GetBleedingManagerServer().AttemptAddBleedingSource(0);
				}
			}
		}
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
		else if (statName == "Immunity") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_IMMUNITY, value);
		else if (statName == "Athletics") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_ATHLETICS, value);
		else if (statName == "Strength") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_STRENGTH, value);
		else if (statName == "Medicine") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_MEDICINE, value);
		else if (statName == "Stealth") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_STEALTH, value);
		else if (statName == "Survival") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_SURVIVAL, value);
		else if (statName == "Farming") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_FARMING, value);
		else if (statName == "Hunting") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_HUNTING, value);
		else if (statName == "Fishing") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_FISHING, value);
		else if (statName == "Mechanics") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_MECHANICS, value);
		else if (statName == "Electronics") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_ELECTRONICS, value);
		else if (statName == "Carpentry") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_CARPENTRY, value);
		else if (statName == "Metalworking") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_METALWORKING, value);
		else if (statName == "Building") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_BUILDING, value);
		else if (statName == "Firearm") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_FIREARMS, value);
		else if (statName == "Lockpicking") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_LOCKPICKING, value);
		else if (statName == "Herbalism") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_HERBALISM, value);
		else if (statName == "Tailoring") profile.m_skills.SetSkillValue(SyberiaSkillType.SYBSKILL_SEAMSTRESS, value);
				
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
		if (!playerContext.m_isGhost && profile && profile.m_skills)
		{
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Health", 0, player.GetMaxHealth("GlobalHealth", "Health"), player.GetHealth("", "Health")));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Blood", 2500, player.GetMaxHealth("GlobalHealth", "Blood"), player.GetHealth("", "Blood")));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Shock", 0, player.GetMaxHealth("GlobalHealth", "Shock"), player.GetHealth("", "Shock")));
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Water", player.GetStatWater().GetMin(), player.GetStatWater().GetMax(), player.GetStatWater().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Energy", player.GetStatEnergy().GetMin(), player.GetStatEnergy().GetMax(), player.GetStatEnergy().Get()));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Mind", 0, MINDSTATE_MAX_VALUE, player.m_mindStateValue));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Sleeping", 0, SLEEPING_MAX_VALUE, player.m_sleepingValue));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("HeatBuffer", player.GetStatHeatBuffer().GetMin(), player.GetStatHeatBuffer().GetMax(), player.GetStatHeatBuffer().Get()));
			
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
			
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Immunity", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_IMMUNITY)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Athletics", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_ATHLETICS)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Strength", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_STRENGTH)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Medicine", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_MEDICINE)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Stealth", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_STEALTH)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Survival", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_SURVIVAL)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Farming", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_FARMING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Hunting", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_HUNTING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Fishing", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_FISHING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Mechanics", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_MECHANICS)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Electronics", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_ELECTRONICS)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Carpentry", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_CARPENTRY)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Metalworking", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_METALWORKING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Building", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_BUILDING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Firearm", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_FIREARMS)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Lockpicking", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_LOCKPICKING)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Herbalism", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_HERBALISM)));
			playerContext.m_stats.Insert(new PluginAdminTool_PlayerStatContext("Tailoring", 0, 100, profile.m_skills.GetSkillValue(SyberiaSkillType.SYBSKILL_SEAMSTRESS)));

			
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