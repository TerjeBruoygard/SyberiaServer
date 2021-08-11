modded class PluginTrader
{
	ref PluginTrader_Config m_config;
	ref map<int, TraderPoint> m_traderPoints = new map<int, TraderPoint>;
	ref map<int, ref PluginTrader_TraiderServer> m_traderCache = new map<int, ref PluginTrader_TraiderServer>;
	ref map<int, ref PluginTrader_StorageServer> m_storageCache = new map<int, ref PluginTrader_StorageServer>;
	
	override void OnInit()
	{
		MakeDirectory("$profile:Syberia");

		string path = "$profile:Syberia\\TradingConfig.json";
		if (FileExist(path))
		{
			m_config = new PluginTrader_Config;
			JsonFileLoader<ref PluginTrader_Config>.JsonLoadFile(path, m_config);
		}
		else
		{
			Error("FAILED TO FIND " + path);
		}
	}
	
	void InitializeTraders()
	{
		if (m_config && m_config.m_traders && m_config.m_storages)
		{
			foreach (ref PluginTrader_StorageServer storage : m_config.m_storages)
			{
				m_storageCache.Insert(storage.m_storageId, storage);
			}
			
			foreach (ref PluginTrader_TraiderServer traider : m_config.m_traders)
			{
				SpawnTrader(traider);
			}
		}
		else
		{
			Error("FAILED TO PARSE TRADER CONFIG");
		}
	}

	private void SpawnTrader(ref PluginTrader_TraiderServer traider)
	{
		if (traider.m_traderId < 0 || traider.m_storageId < 0)
		{
			Error("FAILED TO INITIALIZE TRADER WITH ID " + traider.m_traderId);
			return;
		}
		
		Object traderObj = GetGame().CreateObject(traider.m_classname, traider.m_position);
		if (!traderObj)
		{
			Error("FAILED TO INITIALIZE TRADER WITH ID " + traider.m_traderId);
			return;
		}
	
		traderObj.SetAllowDamage(false);
		traderObj.SetPosition(traider.m_position);
		traderObj.SetOrientation(Vector(traider.m_rotation, 0, 0));
				
		EntityAI traderEntity;
		if (EntityAI.CastTo(traderEntity, traderObj) && traider.m_attachments)
		{
			foreach (string attachment : traider.m_attachments)
			{
				traderEntity.GetInventory().CreateInInventory(attachment);
			}
		}
		
		PlayerBase traderHuman;
		if (PlayerBase.CastTo(traderHuman, traderObj))
		{
			traderHuman.MarkAsNPC();
		}
		
		TraderPoint traderPoint = TraderPoint.Cast( GetGame().CreateObject("TraderPoint", traider.m_position) );
		traderPoint.SetPosition(traider.m_position);
		traderPoint.SetAllowDamage(false);
		traderPoint.InitTraderPoint(traider.m_traderId, traderObj);
				
		m_traderPoints.Insert(traider.m_traderId, traderPoint);		
		m_traderCache.Insert(traider.m_traderId, traider);
		SybLogSrv("Traider " + traider.m_traderId + " successfully initialized with " + traider.m_storageId + " storage.");
	}
	
	void SendTraderMenuOpen(PlayerBase player, int traderId)
	{
		if (!player)
			return;
		
		ref PluginTrader_TraiderServer traider;
		if (!m_traderCache.Find(traderId, traider))
			return;
		
		ref PluginTrader_StorageServer storage;
		if (!m_storageCache.Find(traider.m_storageId, storage))
			return;
		
		Param3<int, ref PluginTrader_Traider, ref PluginTrader_Storage> params = 
			new Param3<int, ref PluginTrader_Traider, ref PluginTrader_Storage>(
			traderId, 
			PluginTrader_Traider.Cast(traider), 
			PluginTrader_Storage.Cast(storage));
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_OPEN_TRADE_MENU, player.GetIdentity(), params);
	}
	
	override void OnDestroy()
	{
		if (m_config)
		{
			delete m_config;
		}
		
		if (m_traderPoints)
		{
			foreach (int id, TraderPoint obj : m_traderPoints)
			{
				GetGame().ObjectDelete(obj);
			}
			delete m_traderPoints;
		}
		
		delete m_traderCache;
		delete m_storageCache;
	}
};

class PluginTrader_Config
{
	ref array<ref PluginTrader_StorageServer> m_storages;
	ref array<ref PluginTrader_TraiderServer> m_traders;
	
	void ~PluginTrader_Config()
	{
		if (m_storages)
		{
			foreach (ref PluginTrader_StorageServer storage : m_storages)
			{
				delete storage;
			}
			delete m_storages;
		}
		
		if (m_traders)
		{
			foreach (ref PluginTrader_TraiderServer traider : m_traders)
			{
				delete traider;
			}
			delete m_traders;
		}
	}
};

class PluginTrader_StorageServer : PluginTrader_Storage
{
	int m_storageId;
    string m_storageName;
};

class PluginTrader_TraiderServer : PluginTrader_Traider
{
    int m_storageId;
    string m_classname;
	ref array<string> m_attachments;
    vector m_position;
    float m_rotation;
};