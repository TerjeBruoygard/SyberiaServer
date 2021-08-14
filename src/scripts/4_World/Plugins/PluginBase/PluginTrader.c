modded class PluginTrader
{
	ref PluginTrader_Config m_config;
	ref map<int, TraderPoint> m_traderPoints = new map<int, TraderPoint>;
	ref map<int, ref PluginTrader_TraderServer> m_traderCache = new map<int, ref PluginTrader_TraderServer>;
	ref map<int, ref PluginTrader_Data> m_traderData = new map<int, ref PluginTrader_Data>;
	
	override void OnInit()
	{
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_CLOSE_TRADER_MENU, this, "RpcRequstTraderMenuClose"); 
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_ACTION_TRADER, this, "RpcRequstTraderAction"); 
		
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
		if (m_config && m_config.m_traders)
		{
			foreach (ref PluginTrader_TraderServer trader : m_config.m_traders)
			{
				SpawnTrader(trader);
			}
		}
		else
		{
			Error("FAILED TO PARSE TRADER CONFIG");
		}
	}

	private void SpawnTrader(ref PluginTrader_TraderServer trader)
	{
		if (trader.m_traderId < 0)
		{
			Error("FAILED TO INITIALIZE TRADER WITH ID " + trader.m_traderId);
			return;
		}
		
		Object traderObj = GetGame().CreateObject(trader.m_classname, trader.m_position);
		if (!traderObj)
		{
			Error("FAILED TO INITIALIZE TRADER WITH ID " + trader.m_traderId);
			return;
		}
	
		traderObj.SetAllowDamage(false);
		traderObj.SetPosition(trader.m_position);
		traderObj.SetOrientation(Vector(trader.m_rotation, 0, 0));
				
		EntityAI traderEntity;
		if (EntityAI.CastTo(traderEntity, traderObj) && trader.m_attachments)
		{
			foreach (string attachment : trader.m_attachments)
			{
				traderEntity.GetInventory().CreateInInventory(attachment);
			}
		}
		
		PlayerBase traderHuman;
		if (PlayerBase.CastTo(traderHuman, traderObj))
		{
			traderHuman.MarkAsNPC();
		}
		
		TraderPoint traderPoint = TraderPoint.Cast( GetGame().CreateObject("TraderPoint", trader.m_position) );
		traderPoint.SetPosition(trader.m_position);
		traderPoint.SetAllowDamage(false);
		traderPoint.InitTraderPoint(trader.m_traderId, traderObj);
				
		m_traderPoints.Insert(trader.m_traderId, traderPoint);		
		m_traderCache.Insert(trader.m_traderId, trader);
		
		if (!m_traderData.Contains(trader.m_traderId))
		{
			ref PluginTrader_Data traderData = new PluginTrader_Data;
			traderData.m_items = new map<string, float>;
			m_traderData.Insert( trader.m_traderId, traderData );
		}
		
		SybLogSrv("Trader " + trader.m_traderId + " successfully initialized.");
	}
	
	void SendTraderMenuOpen(PlayerBase player, int traderId)
	{
		if (!player)
			return;
		
		ref PluginTrader_TraderServer trader;
		if (!m_traderCache.Find(traderId, trader))
			return;
		
		TraderPoint traderPoint;
		if ( !m_traderPoints.Find(traderId, traderPoint) )
			return;
		
		ref PluginTrader_Data traderData;
		if ( !m_traderData.Find(traderId, traderData) )
			return;
		
		if (traderPoint.HasActiveUser())
		{
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, player.GetIdentity(), new Param1<string>("#syb_trader_blocked"));
			return;
		}
		
		traderPoint.SetActiveUser(player);
		
		Param3<int, ref PluginTrader_Trader, ref PluginTrader_Data> params = 
			new Param3<int, ref PluginTrader_Trader, ref PluginTrader_Data>(traderId, PluginTrader_Trader.Cast(trader), traderData);
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_OPEN_TRADE_MENU, player.GetIdentity(), params);
	}
	
	void RpcRequstTraderMenuClose(ref ParamsReadContext ctx, ref PlayerIdentity sender)
    {   
		Param1<int> clientData;
       	if ( !ctx.Read( clientData ) ) return;		
		
		TraderPoint traderPoint;
		if ( m_traderPoints.Find(clientData.param1, traderPoint) )
		{
			traderPoint.SetActiveUser(null);
		}
	}
	
	void RpcRequstTraderAction(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		// Prepare
		Param2<int, ref array<ItemBase>> clientData;
       	if ( !ctx.Read( clientData ) ) return;
		
		int traderId = clientData.param1;
		ref array<ItemBase>	sellItems = clientData.param2;
		
		ref PluginTrader_TraderServer traderInfo;
		if (!m_traderCache.Find(traderId, traderInfo))
			return;
		
		ref PluginTrader_Data traderData;
		if ( !m_traderData.Find(traderId, traderData) )
			return;
		
		// Validation
		int resultPrice = 0;
		foreach (ItemBase sellItem1 : sellItems)
		{
			resultPrice = resultPrice + CalculateSellPrice(PluginTrader_Trader.Cast(traderInfo), traderData, sellItem1);
		}
		
		if (resultPrice < 0)
		{
			return;
		}
		
		
		
		// Sell items		
		foreach (ItemBase sellItem2 : sellItems)
		{
			if (sellItem2)
			{
				string classname = sellItem2.GetType();
				float maxQuantity = CalculateTraiderItemQuantityMax(traderInfo, classname);
				float itemQuantity = CalculateItemQuantity01(sellItem2);
				
				if (traderData.m_items.Contains(classname))
				{
					traderData.m_items.Set( classname, Math.Min(maxQuantity, traderData.m_items.Get(classname) + itemQuantity) );
				}
				else
				{
					traderData.m_items.Set( classname, Math.Min(maxQuantity, itemQuantity) );
				}
				
				SybLogSrv("TRADER " + traderId + " BUY ITEM " + classname + ":" + itemQuantity);
			}
		}
		
		// Delete sell items
		foreach (ItemBase sellItem2 : sellItems)
		{
			if (sellItem2)
			{
				GetGame().ObjectDelete(sellItem2);				
			}
		}
		
		// Send response
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_ACTION_TRADER, sender, new Param1<ref PluginTrader_Data>(traderData));
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
		delete m_traderData;
	}
};

class PluginTrader_Config
{
	ref array<ref PluginTrader_TraderServer> m_traders;
	
	void ~PluginTrader_Config()
	{		
		if (m_traders)
		{
			foreach (ref PluginTrader_TraderServer trader : m_traders)
			{
				delete trader;
			}
			delete m_traders;
		}
	}
};

class PluginTrader_TraderServer : PluginTrader_Trader
{
    string m_classname;
	ref array<string> m_attachments;
    vector m_position;
    float m_rotation;
};