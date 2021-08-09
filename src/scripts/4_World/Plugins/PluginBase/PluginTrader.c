modded class PluginTrader
{
	ref PluginTrader_Config m_config;
	ref map<int, TraderPoint> m_traderPoints = new map<int, TraderPoint>;
	
	override void OnInit()
	{
		super.OnInit();
		
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
			foreach (ref PluginTrader_Traider traider : m_config.m_traders)
			{
				SpawnTrader(traider);
			}
		}
		else
		{
			Error("FAILED TO PARSE TRADER CONFIG");
		}
	}

	private void SpawnTrader(ref PluginTrader_Traider traider)
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

		vector yawPitchRoll = traderObj.GetLocalYawPitchRoll();
		yawPitchRoll[0] = traider.m_rotation;		
		traderObj.SetAllowDamage(false);
		traderObj.SetPosition(traider.m_position);
		traderObj.SetYawPitchRoll(yawPitchRoll);
				
		EntityAI traderEntity;
		if (EntityAI.CastTo(traderEntity, traderObj) && traider.m_attachments)
		{
			foreach (string attachment : traider.m_attachments)
			{
				traderEntity.GetInventory().CreateInInventory(attachment);
			}
		}
		
		TraderPoint traderPoint = TraderPoint.Cast( GetGame().CreateObject("TraderPoint", traider.m_position) );
		traderPoint.SetPosition(traider.m_position);
		traderPoint.SetAllowDamage(false);
		traderPoint.InitTraderPoint(traider.m_traderId, traderObj);
				
		m_traderPoints.Insert(traider.m_traderId, traderPoint);
		SybLogSrv("Traider " + traider.m_traderId + " successfully initialized with " + traider.m_storageId + " storage.");
	}
	
	override void OnDestroy()
	{
		super.OnDestroy();
		
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
	}
};

class PluginTrader_Config
{
	ref array<ref PluginTrader_Storage> m_storages;
	ref array<ref PluginTrader_Traider> m_traders;
	
	void ~PluginTrader_Config()
	{
		if (m_storages)
		{
			foreach (ref PluginTrader_Storage storage : m_storages)
			{
				delete storage;
			}
			delete m_storages;
		}
		
		if (m_traders)
		{
			foreach (ref PluginTrader_Traider traider : m_traders)
			{
				delete traider;
			}
			delete m_traders;
		}
	}
};

class PluginTrader_Storage
{
	int m_storageId;
    string m_storageName;
    int m_storageMaxSize;
    float m_storageCommission;
    string m_dumpingByAmountAlgorithm;
    float m_dumpingByAmountModifier;
	string m_dumpingByQualityAlgorithm;
    float m_dumpingByQualityModifier;
};

class PluginTrader_Traider
{
	int m_traderId;
    int m_storageId;
    string m_classname;
	ref array<string> m_attachments;
    vector m_position;
    float m_rotation;
    ref array<string> m_purchaseFilter;
    ref array<string> m_saleFilter;
};