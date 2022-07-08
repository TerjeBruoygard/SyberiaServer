modded class PluginSyberiaOptions extends PluginBase
{
	ref PluginSyberiaOptions_Main m_main;
	ref PluginSyberiaOptions_GroupDefault m_groupDefault;
	ref map<string, ref PluginSyberiaOptions_GroupFaction> m_groupFactions;
	ref array<ref PluginSyberiaOptions_GroupSingleUser> m_customLoadoutsRaw;
	ref map<string, ref PluginSyberiaOptions_GroupSingleUser> m_customLoadouts;
	ref map<int, float> m_skillModifiers;
	
	override void OnInit()
	{
		MakeDirectory("$profile:Syberia");
			
		string path = "$profile:Syberia\\MainConfig.json";
		if (FileExist(path))
		{
			m_main = new PluginSyberiaOptions_Main;
			JsonFileLoader<ref PluginSyberiaOptions_Main>.JsonLoadFile(path, m_main);
		}
		
		path = "$profile:Syberia\\ClientConfig.json";
		if (FileExist(path))
		{
			m_client = new PluginSyberiaOptions_Client;
			JsonFileLoader<ref PluginSyberiaOptions_Client>.JsonLoadFile(path, m_client);
		}
		
		path = "$profile:Syberia\\Group_Default.json";
		if (FileExist(path))
		{
			m_groupDefault = new PluginSyberiaOptions_GroupDefault;
			JsonFileLoader<ref PluginSyberiaOptions_GroupDefault>.JsonLoadFile(path, m_groupDefault);
		}
		
		SyberiaDatabaseInit.InitIfNot();
		
		m_groupFactions = new map<string, ref PluginSyberiaOptions_GroupFaction>;
		if (m_main.m_groups)
		{
			foreach(string group : m_main.m_groups)
			{
				path = "$profile:Syberia\\Group_" + group + ".json";
				if (FileExist(path))
				{
					ref PluginSyberiaOptions_GroupFaction groupfac = new PluginSyberiaOptions_GroupFaction;
					JsonFileLoader<ref PluginSyberiaOptions_GroupFaction>.JsonLoadFile(path, groupfac);
					groupfac.m_name = group;
					groupfac.LoadMembers();
					m_groupFactions.Insert(group, groupfac);
				}
			}
		}
		
		m_skillModifiers = new map<int, float>;
		if (m_main.m_skillModifiers)
		{
			for (int i = 0; i < SyberiaSkillType.SYBSKILL_TOTALCOUNT; i++)
			{
				float skillModValue = 1;
				foreach (ref PluginSyberiaOptions_SkillModifier skillMod : m_main.m_skillModifiers)
				{
					if (skillMod.m_id == i)
					{
						skillModValue = skillMod.m_mod;
					}
				}
				m_skillModifiers.Insert(i, skillModValue);
			}
		}
		
		path = "$profile:Syberia\\CustomLoadouts.json";
		if (FileExist(path))
		{
			m_customLoadouts = new map<string, ref PluginSyberiaOptions_GroupSingleUser>;
			m_customLoadoutsRaw = new array<ref PluginSyberiaOptions_GroupSingleUser>;
			JsonFileLoader<ref array<ref PluginSyberiaOptions_GroupSingleUser>>.JsonLoadFile(path, m_customLoadoutsRaw);
			foreach (ref PluginSyberiaOptions_GroupSingleUser singleUserLoadout : m_customLoadoutsRaw)
			{
				m_customLoadouts.Set(singleUserLoadout.m_uid, singleUserLoadout);
			}
		}
	}
	
	void ~PluginSyberiaOptions()
	{
		delete m_main;
		delete m_groupDefault;
		
		foreach (string name, ref PluginSyberiaOptions_GroupFaction gf : m_groupFactions) delete gf;
		delete m_groupFactions;
	}
	
	void SendClientOptions(PlayerIdentity identity, ref CharProfile profile)
	{
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_CLIENT_OPTIONS, identity, new Param3<ref PluginSyberiaOptions_Client, int, string>(m_client, profile.m_id, Syberia_ServerVersion));
	}
	
	int GetCharacterAllowedEquipmentSize()
	{
		return SyberiaScreenEquipPages.SYBSEP_TOTAL_COUNT;
	}
	
	ref SpawnpointInfo GetCharacterSpawnpoint(PlayerIdentity identity, ref CharProfile profile, ref PluginSyberiaOptions_GroupFaction faction, int spawnpointId)
	{
		ref PluginSyberiaOptions_GroupSingleUser customLoadout = null;
		if (m_customLoadouts)
		{
			if (!m_customLoadouts.Find(identity.GetPlainId(), customLoadout))
			{
				m_customLoadouts.Find(identity.GetId(), customLoadout);
			}
		}
		
		ref array<ref SpawnpointInfo> spawnPoints = new array<ref SpawnpointInfo>;
		if (faction && faction.m_spawnpoints)
		{
			foreach (ref SpawnpointInfo sp1 : faction.m_spawnpoints)
			{
				spawnPoints.Insert(sp1);
			}
		}
		if (!faction || faction.m_allowDefaultSpawnpoints)
		{
			foreach (ref SpawnpointInfo sp2 : m_groupDefault.m_spawnpoints)
			{
				spawnPoints.Insert(sp2);
			}		
		}
		if (customLoadout != null)
		{
			foreach (ref SpawnpointInfo sp3 : customLoadout.m_spawnpoints)
			{
				spawnPoints.Insert(sp3);
			}
		}
		
		if (spawnpointId < 0 || spawnpointId >= spawnPoints.Count())
		{
			spawnpointId = 0;
		}
		
		ref SpawnpointInfo result = spawnPoints.Get(spawnpointId);		
		delete spawnPoints;
		return result;
	}
	
	ref array<string> GetCharacterLoadoutItems(PlayerIdentity identity, ref CharProfile profile, ref PluginSyberiaOptions_GroupFaction faction, int loadoutId)
	{			
		ref PluginSyberiaOptions_GroupSingleUser customLoadout = null;
		if (m_customLoadouts)
		{
			if (!m_customLoadouts.Find(identity.GetPlainId(), customLoadout))
			{
				m_customLoadouts.Find(identity.GetId(), customLoadout);
			}
		}
		
		array<ref PluginSyberiaOptions_ItemsLoadout> loadouts = new array<ref PluginSyberiaOptions_ItemsLoadout>;
		if (faction && faction.m_gearItems)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout sp1 : faction.m_gearItems)
			{
				loadouts.Insert(sp1);
			}
		}
		if (!faction || faction.m_allowDefaultSpawnpoints)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout sp2 : m_groupDefault.m_gearItems)
			{
				loadouts.Insert(sp2);
			}		
		}
		if (customLoadout != null)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout sp3 : customLoadout.m_gearItems)
			{
				loadouts.Insert(sp3);
			}		
		}
		
		if (loadoutId < 0 || loadoutId >= loadouts.Count())
		{
			return null;
		}
		
		return loadouts.Get(loadoutId).m_items;		
	}
	
	ref array<string> GetCharacterSpecialItems(ref CharProfile profile, string loadoutName)
	{
		if (loadoutName == "#syb_perk_name_3010") return m_main.m_startMedicKit;
		if (loadoutName == "#syb_perk_name_4007") return m_main.m_startStealthKit;
		if (loadoutName == "#syb_perk_name_5010") return m_main.m_startSurvivorKit;
		
		return null;
	}
	
	ref array<ref array<string>> GetCharacterAllowedEquipment(PlayerIdentity identity, ref CharProfile profile, ref PluginSyberiaOptions_GroupFaction faction)
	{
		ref PluginSyberiaOptions_GroupSingleUser customLoadout = null;
		if (m_customLoadouts)
		{
			if (!m_customLoadouts.Find(identity.GetPlainId(), customLoadout))
			{
				m_customLoadouts.Find(identity.GetId(), customLoadout);
			}
		}
		
		ref array<ref array<string>> result = new array<ref array<string>>;
		
		// SPAWNPOINTS
		ref array<string> spawnPoints = new array<string>;
		if (faction && faction.m_spawnpoints)
		{
			foreach (ref SpawnpointInfo sp1 : faction.m_spawnpoints)
			{
				spawnPoints.Insert(sp1.m_name);
			}
		}
		if (!faction || faction.m_allowDefaultSpawnpoints)
		{
			foreach (ref SpawnpointInfo sp2 : m_groupDefault.m_spawnpoints)
			{
				spawnPoints.Insert(sp2.m_name);
			}		
		}
		if (customLoadout != null)
		{
			foreach (ref SpawnpointInfo sp3 : customLoadout.m_spawnpoints)
			{
				spawnPoints.Insert(sp3.m_name);
			}
		}
		result.Insert(spawnPoints);
		
		// GEAR BODY
		ref array<string> bodyGear = new array<string>;
		if (faction && faction.m_gearBody) bodyGear.InsertAll(faction.m_gearBody);
		if (!faction || faction.m_allowDefaultLoadout) bodyGear.InsertAll(m_groupDefault.m_gearBody);
		if (customLoadout != null) bodyGear.InsertAll(customLoadout.m_gearBody);
		result.Insert(bodyGear);
		
		// GEAR PANTS
		ref array<string> pantsGear = new array<string>;
		if (faction && faction.m_gearPants) pantsGear.InsertAll(faction.m_gearPants);
		if (!faction || faction.m_allowDefaultLoadout) pantsGear.InsertAll(m_groupDefault.m_gearPants);
		if (customLoadout != null) pantsGear.InsertAll(customLoadout.m_gearPants);
		result.Insert(pantsGear);
		
		// GEAR FOOT
		ref array<string> footGear = new array<string>;
		if (faction && faction.m_gearFoot) footGear.InsertAll(faction.m_gearFoot);
		if (!faction || faction.m_allowDefaultLoadout) footGear.InsertAll(m_groupDefault.m_gearFoot);
		if (customLoadout != null) footGear.InsertAll(customLoadout.m_gearFoot);
		result.Insert(footGear);
		
		// GEAR HEAD
		ref array<string> headGear = new array<string>;
		if (faction && faction.m_gearHead) headGear.InsertAll(faction.m_gearHead);
		if (!faction || faction.m_allowDefaultLoadout) headGear.InsertAll(m_groupDefault.m_gearHead);
		if (customLoadout != null) headGear.InsertAll(customLoadout.m_gearHead);
		result.Insert(headGear);
		
		// GEAR MASK
		ref array<string> maskGear = new array<string>;
		if (faction && faction.m_gearMask) maskGear.InsertAll(faction.m_gearMask);
		if (!faction || faction.m_allowDefaultLoadout) maskGear.InsertAll(m_groupDefault.m_gearMask);
		if (customLoadout != null) maskGear.InsertAll(customLoadout.m_gearMask);
		result.Insert(maskGear); 
		
		// GEAR GLOVES
		ref array<string> glovesGear = new array<string>;
		if (faction && faction.m_gearGloves) glovesGear.InsertAll(faction.m_gearGloves);
		if (!faction || faction.m_allowDefaultLoadout) glovesGear.InsertAll(m_groupDefault.m_gearGloves);
		if (customLoadout != null) glovesGear.InsertAll(customLoadout.m_gearGloves);
		result.Insert(glovesGear); 
		
		// GEAR VEST
		ref array<string> vestGear = new array<string>;
		if (faction && faction.m_gearVest) vestGear.InsertAll(faction.m_gearVest);
		if (!faction || faction.m_allowDefaultLoadout) vestGear.InsertAll(m_groupDefault.m_gearVest);
		if (customLoadout != null) vestGear.InsertAll(customLoadout.m_gearVest);
		result.Insert(vestGear); 
		
		// GEAR BACKPACKS
		ref array<string> backpackGear = new array<string>;
		if (faction && faction.m_gearBackpack) backpackGear.InsertAll(faction.m_gearBackpack);
		if (!faction || faction.m_allowDefaultLoadout) backpackGear.InsertAll(m_groupDefault.m_gearBackpack);
		if (customLoadout != null) backpackGear.InsertAll(customLoadout.m_gearBackpack);
		result.Insert(backpackGear); 
		
		// GEAR WEAPON
		ref array<string> weapGear = new array<string>;
		if (faction && faction.m_gearWeapon) weapGear.InsertAll(faction.m_gearWeapon);
		if (!faction || faction.m_allowDefaultLoadout) weapGear.InsertAll(m_groupDefault.m_gearWeapon);
		if (customLoadout != null) weapGear.InsertAll(customLoadout.m_gearWeapon);
		result.Insert(weapGear);
		
		// GEAR ITEMS
		ref array<string> itemsGear = new array<string>;
		if (faction && faction.m_gearItems)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout il1 : faction.m_gearItems)
			{
				itemsGear.Insert(il1.m_name);
			}
		}
		if (!faction || faction.m_allowDefaultLoadout)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout il2 : m_groupDefault.m_gearItems)
			{
				itemsGear.Insert(il2.m_name);
			}		
		}
		if (customLoadout != null)
		{
			foreach (ref PluginSyberiaOptions_ItemsLoadout il3 : customLoadout.m_gearItems)
			{
				itemsGear.Insert(il3.m_name);
			}
		}
		result.Insert(itemsGear);
		
		// GEAR SPECIAL
		ref array<string> specGear = new array<string>;
		if (profile && profile.m_skills)
		{
			if (profile.m_skills.HasPerk(SyberiaPerkType.SYBPERK_MEDICINE_START_LOADOUT)) specGear.Insert("#syb_perk_name_3010");
			if (profile.m_skills.HasPerk(SyberiaPerkType.SYBPERK_STEALTH_START_LOADOUT)) specGear.Insert("#syb_perk_name_4007");
			if (profile.m_skills.HasPerk(SyberiaPerkType.SYBPERK_SURVIVAL_START_LOADOUT)) specGear.Insert("#syb_perk_name_5010");
		}
		result.Insert(specGear);
		
		return result;
	}
	
	ref PluginSyberiaOptions_GroupFaction FindGroupByLeader(PlayerIdentity identity)
	{
		foreach (string name, ref PluginSyberiaOptions_GroupFaction group : m_groupFactions)
		{
			foreach (string leader : group.m_leaders)
			{
				string value = identity.GetPlainId();
				if (leader == value)
				{
					return group;
				}
				
				value = identity.GetId();
				if (leader == value)
				{
					return group;
				}
			}
		}
		
		return null;
	}
	
	ref PluginSyberiaOptions_GroupFaction FindGroupByMember(int character_id)
	{
		foreach (string name, ref PluginSyberiaOptions_GroupFaction group : m_groupFactions)
		{
			foreach (ref SyberiaPdaGroupMember member : group.m_members)
			{
				if (character_id == member.m_id)
				{
					return group;
				}
			}
		}
		
		return null;
	}
};

class PluginSyberiaOptions_Main
{
	int m_startSoulsCount = 3;
	int m_respawnSoulsPrice = 1;	
	int m_newchar_points = 10;
	int m_roleplay_mode = 0;
	ref array<string> m_groups;
	ref array<ref PluginSyberiaOptions_SkillModifier> m_skillModifiers;
	ref array<string> m_startMedicKit;
	ref array<string> m_startStealthKit;
	ref array<string> m_startSurvivorKit;

	void ~PluginSyberiaOptions_Main()
	{
		delete m_groups;
	
		foreach (ref PluginSyberiaOptions_SkillModifier sm : m_skillModifiers) delete sm;
		delete m_skillModifiers;	
	}
};

class PluginSyberiaOptions_SkillModifier
{
	int m_id;
	float m_mod;
	float m_decreaseOnDeath;
};

class PluginSyberiaOptions_GroupDefault
{
	ref array<ref SpawnpointInfo> m_spawnpoints;
	ref array<string> m_gearBody;
	ref array<string> m_gearPants;
	ref array<string> m_gearFoot;
	ref array<string> m_gearHead;
	ref array<string> m_gearMask;
    ref array<string> m_gearGloves;
    ref array<string> m_gearVest;
    ref array<string> m_gearBackpack;
	ref array<string> m_gearWeapon;
	ref array<ref PluginSyberiaOptions_ItemsLoadout> m_gearItems;
	
	void ~PluginSyberiaOptions_GroupDefault()
	{
		foreach (ref SpawnpointInfo si : m_spawnpoints) delete si;
		delete m_spawnpoints;
		delete m_gearBody;
		delete m_gearPants;
		delete m_gearFoot;
		delete m_gearHead;
		delete m_gearMask;
    	delete m_gearGloves;
    	delete m_gearVest;
    	delete m_gearBackpack;
		delete m_gearWeapon;
		
		foreach (ref PluginSyberiaOptions_ItemsLoadout il : m_gearItems) delete il;
		delete m_gearItems;
	}
};

class PluginSyberiaOptions_ItemsLoadout
{
	string m_name;
	ref array<string> m_items;
	
	void ~PluginSyberiaOptions_ItemsLoadout()
	{
		delete m_items;
	}	
};

class PluginSyberiaOptions_GroupFaction : PluginSyberiaOptions_GroupDefault
{
	string m_name;
	string m_displayName;
	int m_maxMembers;
	bool m_allowDefaultSpawnpoints;
	bool m_allowDefaultLoadout;
	ref array<string> m_leaders;
	ref array<ref SyberiaPdaGroupMember> m_members;
	
	void ~PluginSyberiaOptions_GroupFaction()
	{
		delete m_leaders;
		
		foreach (ref SyberiaPdaGroupMember gm : m_members) delete gm;
		delete m_members;
	}
	
	static void InitQueries(ref array<string> queries)
	{
		queries.Insert("CREATE TABLE IF NOT EXISTS group_members (id INTEGER PRIMARY KEY AUTOINCREMENT, group_name TEXT NOT NULL, character_id INTEGER UNIQUE NOT NULL);");	
		queries.Insert("DELETE FROM group_members WHERE group_members.character_id NOT IN (SELECT characters.id FROM characters);");
	}
	
	string SelectQuery()
	{
		return "SELECT characters.id, characters.displayName FROM characters INNER JOIN group_members ON group_members.character_id = characters.id WHERE group_members.group_name = '" + m_name + "';";
	}
	
	void AddMember(ref SyberiaPdaGroupMember member)
	{
		m_members.Insert(member);
		
		string query = "INSERT INTO group_members(group_name, character_id) VALUES('" + m_name + "', " + member.m_id + ");";
		GetDatabase().QueryAsync(SYBERIA_DB_NAME, query, this, "OnDbCallback");
	}
	
	void RemoveMember(ref SyberiaPdaGroupMember member)
	{
		m_members.RemoveItem(member);
		
		string query = "DELETE FROM group_members WHERE character_id = " + member.m_id + ";";
		GetDatabase().QueryAsync(SYBERIA_DB_NAME, query, this, "OnDbCallback");
	}
	
	void OnDbCallback(DatabaseResponse response, ref Param args) {}
	
	void LoadMembers()
	{
		DatabaseResponse response;
		if (GetDatabase().QuerySync(SYBERIA_DB_NAME, SelectQuery(), response))
		{
			if (response && response.GetRowsCount() > 0) 
			{
				m_members = new array<ref SyberiaPdaGroupMember>;
				for (int rowId = 0; rowId < response.GetRowsCount(); rowId++)
				{
					ref SyberiaPdaGroupMember member = new SyberiaPdaGroupMember;
					member.m_id = response.GetValue(rowId, 0).ToInt();
					member.m_name = response.GetValue(rowId, 1);
					m_members.Insert(member);
				}
			}
		}
	}
};

class PluginSyberiaOptions_GroupSingleUser : PluginSyberiaOptions_GroupDefault
{
	string m_uid;
};