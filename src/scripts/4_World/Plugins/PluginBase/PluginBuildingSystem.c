class PluginBuildingSystem extends PluginBase
{
	ref array<BuildingLivespace> m_livespaces = new array<BuildingLivespace>;
	ref map<House, ref array<BuildingLivespace>> m_housesCache = new map<House, ref array<BuildingLivespace>>;
	float m_checkDirtyTimer = 0;
	
	override void OnInit()
	{
		SyberiaDatabaseInit.InitIfNot();
	}
	
	void LoadLivespaces()
	{
		DatabaseResponse response = null;
		string selectQuery = "SELECT id, data FROM building_data;";
		GetDatabase().QuerySync(SYBERIA_DB_NAME, selectQuery, response);
		
		if (response != null)
		{
			for (int i = 0; i < response.GetRowsCount(); i++)
			{
				int id = response.GetValue(i, 0).ToInt();
				string data = response.GetValue(i, 1);
				SpawnLivespace(id, data);
			}
		}
	}

	private void SpawnLivespace(int id, string data)
	{
		ref map<string, string> values = new map<string, string>;
		ParseData(data, values);
		
		// Find house of livespace
		House house = null;
        House tempObject = null;
		string houseClassName = values.Get("hclass");
		vector housePosition = values.Get("hpos").ToVector();
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(housePosition, 0.1, objects, null);
		for (int i = 0; i < objects.Count(); i++)
		{
            tempObject = House.Cast(objects.Get(i));
			if (tempObject != null && tempObject.GetType() == houseClassName)
			{
				house = tempObject;
				break;
			}
		}
		
		if (house == null)
		{
			SybLogSrv("Failed to load livespace " + id + ". House '" + houseClassName + "' not found at '" + housePosition + "'.");
			return;
		}
		
		// Spawn livespace
		int livespaceId = values.Get("lid").ToInt();
		BuildingLivespace livespace = BuildingLivespace.SpawnLivespace(id, house, livespaceId, values);
		if (livespace == null)
		{
			SybLogSrv("Failed to spawn livespace " + id);
			return;
		}
	}
	
	void CreateLivespace(House house, int livespaceId, PlayerBase player)
	{
		if (house == null || livespaceId == -1 || player == null)
		{
			return;
		}
		
		PlayerIdentity identity = player.GetIdentity();
		if (identity == null)
		{
			return;
		}
		
		ref map<string, string> values = new map<string, string>;
		values.Insert("ownrs", identity.GetId());
		
		BuildingLivespace.SpawnLivespace(-1, house, livespaceId, values);
	}
	
	void InsertLivespace(BuildingLivespace livespace)
	{
		if (livespace != null && m_livespaces.Find(livespace) == -1)
		{
			m_livespaces.Insert(livespace);
			
			ref array<BuildingLivespace> cachedLivespaces;
			if (m_housesCache.Contains(livespace.GetHouse()))
			{
				cachedLivespaces = m_housesCache.Get(livespace.GetHouse());
			}
			else
			{
				cachedLivespaces = new array<BuildingLivespace>;
			}
			
			cachedLivespaces.Insert(livespace);
			m_housesCache.Set(livespace.GetHouse(), cachedLivespaces);
		}
	}
	
	void SendHomebookMenuOpen(PlayerBase player, BuildingLivespace livespace)
	{
		auto homebookData = new LivespaceHomebookData();
		livespace.GetLivespaceHomebookData(homebookData);
				
		auto params = new Param2<BuildingLivespace, ref LivespaceHomebookData>(livespace, homebookData);
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_HOMEBOOK_OPEN, player.GetIdentity(), params);	
	}
	
	BuildingLivespace FindByHousePoint(House house, vector pos)
	{
		ref array<BuildingLivespace> livespaces = null;
		if (m_housesCache.Contains(house))
		{
			livespaces = m_housesCache.Get(house);
		}
		
		if (livespaces == null)
		{
			return null;
		}
		
		vector localPos = house.WorldToModel(pos);
		foreach (BuildingLivespace livespace : livespaces)
		{
			if (livespace.IsReady() && GameHelpers.IntersectBBox(livespace.GetData().m_bboxStart, livespace.GetData().m_bboxEnd, localPos))
			{
				return livespace;
			}
		}
		
		return null;
	}
	
	private void ParseData(string data, ref map<string, string> values)
	{
		array<string> tuple = new array<string>;
		array<string> pairs = new array<string>;
		data.Split(";", pairs);
		for (int i = 0; i < pairs.Count(); i++)
		{
			string pair = pairs.Get(i);
			tuple.Clear();
			pair.Split(":", tuple);
			if (tuple.Count() == 2)
			{
				string header = tuple.Get(0);
				string value = tuple.Get(1);
				values.Set(header, value);
			}
		}
	}
	
	private void CreateOrUpdateLivespaceRecord(BuildingLivespace livespace)
	{
		int recordId = livespace.GetRecordId();
		map<string, string> values = new map<string, string>;
		values.Insert("hclass", livespace.GetHouse().GetType());
		values.Insert("hpos", livespace.GetHouse().GetPosition().ToString(false));
		values.Insert("lid", livespace.GetLivespaceId().ToString());
		values.Insert("ddat", livespace.SerializeDoors());
		values.Insert("wdat", livespace.SerializeBarricades());
		values.Insert("ownrs", livespace.SerializeOwners());
		values.Insert("mmbrs", livespace.SerializeMembers());
		
		string dataStr = "";
		foreach (string key, string value : values)
		{
			dataStr = dataStr + key + ":" + value + ";";
		}
		
		TStringArray queries = new TStringArray;
		if (recordId == -1)
		{
			// Create new livespace
			queries.Insert("INSERT INTO building_data(data) VALUES ('" + dataStr + "');");
			queries.Insert("SELECT last_insert_rowid();");		
			GetDatabase().TransactionAsync(SYBERIA_DB_NAME, queries, this, "OnCreateLivespaceRecord", new ref Param1<BuildingLivespace>(livespace));
		}
		else
		{
			// Update exist livespace
			queries.Insert("UPDATE building_data SET data='" + dataStr + "' WHERE id = " + recordId + ";");		
			GetDatabase().TransactionAsync(SYBERIA_DB_NAME, queries, this, "OnUpdateLivespaceRecord", null);
		}
	}
	
	protected void OnCreateLivespaceRecord(DatabaseResponse response, ref Param args)
	{
		ref auto metadata = Param1<BuildingLivespace>.Cast(args);	
		BuildingLivespace livespace = metadata.param1;	
		
		if (livespace && response && response.GetRowsCount() == 1)
		{
			int newId = response.GetValue(0, 0).ToInt();
			if (newId > 0)
			{
				livespace.SetRecordId(newId);
			}
		}
	}
	
	protected void OnUpdateLivespaceRecord(DatabaseResponse response, ref Param args)
	{
		// DO NOTHING
	}
	
	protected void OnDeleteLivespaceRecord(DatabaseResponse response, ref Param args)
	{
		// DO NOTHING
	}
	
	void DeleteLivespaceRecord(BuildingLivespace livespace)
	{
		if (!livespace)
		{
			return;
		}
		
		if (livespace.GetHouse() && m_housesCache.Contains(livespace.GetHouse()))
		{
			ref array<BuildingLivespace> cachedLivespaces = m_housesCache.Get(livespace.GetHouse());
			cachedLivespaces.RemoveItem(livespace);
			m_housesCache.Set(livespace.GetHouse(), cachedLivespaces);
		}
		
		int id = m_livespaces.Find(livespace);
		if (id != -1)
		{
			m_livespaces.Remove(id);
		}
		
		int recordId = livespace.GetRecordId();
		if (recordId != -1)
		{
			TStringArray queries = new TStringArray;
			queries.Insert("DELETE FROM building_data WHERE id = " + recordId + ";");		
			GetDatabase().TransactionAsync(SYBERIA_DB_NAME, queries, this, "OnDeleteLivespaceRecord", null);
		}
	}
	
	override void OnUpdate(float delta_time)
	{
		super.OnUpdate(delta_time);

		m_checkDirtyTimer = m_checkDirtyTimer + delta_time;
		if (m_checkDirtyTimer > 60)
		{
			m_checkDirtyTimer = 0;
			foreach (BuildingLivespace livespace : m_livespaces)
			{
				if (livespace && livespace.PopDbDirty())
				{
					CreateOrUpdateLivespaceRecord(livespace);
				}
			}
		}
	}
	
	override void OnDestroy()
	{		
		delete m_livespaces;
		delete m_housesCache;
	}
	
	static void InitQueries(ref array<string> queries)
	{
		queries.Insert("CREATE TABLE IF NOT EXISTS building_data (id INTEGER PRIMARY KEY AUTOINCREMENT, data TEXT NOT NULL);");
	}
};