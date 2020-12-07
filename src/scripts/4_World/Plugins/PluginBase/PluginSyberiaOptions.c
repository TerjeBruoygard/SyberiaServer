class PluginSyberiaOptions extends PluginBase
{	
	ref PluginSyberiaOptions_Main m_main;
	ref PluginSyberiaOptions_GroupDefault m_groupDefault;
	ref map<string, ref PluginSyberiaOptions_GroupFaction> m_groupFactions;
	
	void PluginSyberiaOptions()
	{	
		MakeDirectory("$profile:Syberia");
			
		string path = "$profile:Syberia\\MainConfig.json";
		if (FileExist(path))
		{
			m_main = new PluginSyberiaOptions_Main;
			JsonFileLoader<ref PluginSyberiaOptions_Main>.JsonLoadFile(path, m_main);
		}
		
		path = "$profile:Syberia\\Group_Default.json";
		if (FileExist(path))
		{
			m_groupDefault = new PluginSyberiaOptions_GroupDefault;
			JsonFileLoader<ref PluginSyberiaOptions_GroupDefault>.JsonLoadFile(path, m_groupDefault);
		}
		
		DatabaseResponse response;
		ref array<string> queries = new array<string>;
		PluginSyberiaOptions_GroupFaction.InitQueries(queries);
		GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);	
		delete queries;
		
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
	}
	
	void ~PluginSyberiaOptions()
	{
		delete m_main;
		delete m_groupDefault;
		
		foreach (string name, ref PluginSyberiaOptions_GroupFaction gf : m_groupFactions) delete gf;
		delete m_groupFactions;
	}
	
	int GetCharacterAllowedEquipmentSize()
	{
		return 8;
	}
	
	ref array<ref array<string>> GetCharacterAllowedEquipment(ref PlayerIdentity identity, ref CharProfile profile)
	{
		ref PluginSyberiaOptions_GroupFaction faction = FindGroupByMember(profile.m_id);		
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
		result.Insert(spawnPoints);
		
		// GEAR BODY
		ref array<string> bodyGear = new array<string>;
		if (faction && faction.m_gearBody) bodyGear.InsertAll(faction.m_gearBody);
		if (!faction || faction.m_allowDefaultLoadout) bodyGear.InsertAll(m_groupDefault.m_gearBody);
		result.Insert(bodyGear);
		
		// GEAR PANTS
		ref array<string> pantsGear = new array<string>;
		if (faction && faction.m_gearPants) pantsGear.InsertAll(faction.m_gearPants);
		if (!faction || faction.m_allowDefaultLoadout) pantsGear.InsertAll(m_groupDefault.m_gearPants);
		result.Insert(pantsGear);
		
		// GEAR FOOT
		ref array<string> footGear = new array<string>;
		if (faction && faction.m_gearFoot) footGear.InsertAll(faction.m_gearFoot);
		if (!faction || faction.m_allowDefaultLoadout) footGear.InsertAll(m_groupDefault.m_gearFoot);
		result.Insert(footGear);
		
		// GEAR HEAD
		ref array<string> headGear = new array<string>;
		if (faction && faction.m_gearHead) headGear.InsertAll(faction.m_gearHead);
		if (!faction || faction.m_allowDefaultLoadout) headGear.InsertAll(m_groupDefault.m_gearHead);
		result.Insert(headGear);
		
		// GEAR WEAPON
		ref array<string> weapGear = new array<string>;
		if (faction && faction.m_gearWeapon) weapGear.InsertAll(faction.m_gearWeapon);
		if (!faction || faction.m_allowDefaultLoadout) weapGear.InsertAll(m_groupDefault.m_gearWeapon);
		result.Insert(weapGear);
		
		// GEAR ITEMS
		ref array<string> itemsGear = new array<string>;
		if (faction && faction.m_gearItems) itemsGear.InsertAll(faction.m_gearItems);
		if (!faction || faction.m_allowDefaultLoadout) itemsGear.InsertAll(m_groupDefault.m_gearItems);
		result.Insert(itemsGear);
		
		// GEAR SPECIAL
		ref array<string> specGear = new array<string>;
		result.Insert(specGear);
		
		return result;
	}
	
	ref PluginSyberiaOptions_GroupFaction FindGroupByLeader(ref PlayerIdentity identity)
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

	void ~PluginSyberiaOptions_Main()
	{
		delete m_groups;
	}
};

class PluginSyberiaOptions_GroupDefault
{
	ref array<ref SpawnpointInfo> m_spawnpoints;
	ref array<string> m_gearBody;
	ref array<string> m_gearPants;
	ref array<string> m_gearFoot;
	ref array<string> m_gearHead;
	ref array<string> m_gearWeapon;
	ref array<string> m_gearItems;
	
	void ~PluginSyberiaOptions_GroupDefault()
	{
		foreach (ref SpawnpointInfo si : m_spawnpoints) delete si;
		delete m_spawnpoints;
		delete m_gearBody;
		delete m_gearPants;
		delete m_gearFoot;
		delete m_gearHead;
		delete m_gearWeapon;
		delete m_gearItems;
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
	}
	
	string SelectQuery()
	{
		return "SELECT characters.id, characters.uid, characters.name FROM characters INNER JOIN group_members ON group_members.character_id = characters.id WHERE group_members.group_name = '" + m_name + "';";
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
	
	void OnDbCallback(DatabaseResponse response) {}
	
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
					member.m_uid = response.GetValue(rowId, 1);
					member.m_name = response.GetValue(rowId, 2);
					m_members.Insert(member);
				}
			}
		}
	}
};

PluginSyberiaOptions GetSyberiaOptions() 
{
    return PluginSyberiaOptions.Cast(GetPlugin(PluginSyberiaOptions));
}