modded class PluginTrader
{
	ref PluginTrader_Config m_config;
	ref map<int, TraderPoint> m_traderPoints = new map<int, TraderPoint>;
	ref map<int, ref PluginTrader_TraderServer> m_traderCache = new map<int, ref PluginTrader_TraderServer>;
	ref map<int, ref PluginTrader_Data> m_traderData = new map<int, ref PluginTrader_Data>;
	float m_updateTimer = 0;
	
	override void OnInit()
	{
		SyberiaDatabaseInit.InitIfNot();
		
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
				
		DatabaseResponse response = null;
		string selectQuery = "SELECT classname, data FROM trader_data WHERE trader_id = " + trader.m_traderId.ToString() + ";";
		GetDatabase().QuerySync(SYBERIA_DB_NAME, selectQuery, response);
		ref PluginTrader_Data traderData = new PluginTrader_Data;
		traderData.DeserializeFromDb( response );		
		m_traderData.Insert( trader.m_traderId, traderData );
		
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
		traderPoint.SetAllowDamage(false);
		traderPoint.SetPosition(trader.m_position);
		traderPoint.SetOrientation(Vector(trader.m_rotation, 0, 0));		
		traderPoint.InitTraderPoint(trader.m_traderId, traderObj);
				
		m_traderPoints.Insert(trader.m_traderId, traderPoint);		
		m_traderCache.Insert(trader.m_traderId, trader);
		
		SybLogSrv("Trader " + trader.m_traderId + " successfully initialized.");
	}

	void OnUpdateTraderDataDB(DatabaseResponse response, ref Param args)
	{
		// Do nothing
	}
	
	void SendTraderMenuOpen(PlayerBase player, int traderId)
	{
		if (!player)
			return;
		
		if (!player.GetIdentity())
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
		
		PluginAdminTool pluginAdminTool = PluginAdminTool.Cast(GetPlugin(PluginAdminTool));
		if (!pluginAdminTool)
			return;
		
		if (traderPoint.HasActiveUser())
		{
			GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, player.GetIdentity(), new Param1<string>("#syb_trader_blocked"));
			return;
		}
		
		traderPoint.SetActiveUser(player);
		
		bool isPlayerAdmin = pluginAdminTool.IsPlayerAdmin(player.GetIdentity());
		Param4<int, ref PluginTrader_Trader, ref PluginTrader_Data, bool> params = 
			new Param4<int, ref PluginTrader_Trader, ref PluginTrader_Data, bool>(traderId, trader, traderData, isPlayerAdmin);
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_OPEN_TRADE_MENU, player.GetIdentity(), params);
	}
	
	void RpcRequstTraderMenuClose(ParamsReadContext ctx, PlayerIdentity sender)
    {   
		Param1<int> clientData;
       	if ( !ctx.Read( clientData ) ) return;		
		
		TraderPoint traderPoint;
		if ( m_traderPoints.Find(clientData.param1, traderPoint) )
		{
			traderPoint.SetActiveUser(null);
		}
	}
	
	void RpcRequstTraderAction(ParamsReadContext ctx, PlayerIdentity sender)
	{
		PluginAdminTool pluginAdminTool = PluginAdminTool.Cast(GetPlugin(PluginAdminTool));
		if (!pluginAdminTool)
			return;
		
		PlayerBase player = GetPlayerByIdentity(sender);
		if (!player)
			return;
		
		// Prepare
		Param3<int, ref array<ItemBase>, ref map<string, float>> clientData;
       	if ( !ctx.Read( clientData ) ) return;
		
		int traderId = clientData.param1;
		ref array<ItemBase>	sellItems = clientData.param2;
		ref map<string, float> buyItems = clientData.param3;
		bool isPlayerAdmin = pluginAdminTool.IsPlayerAdmin(sender);
		
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
			if (CanSellItem(traderInfo, sellItem1))
			{
				resultPrice = resultPrice + CalculateSellPrice(traderInfo, traderData, sellItem1);
			}
		}
		
		foreach (string buyClassname1, float buyQuantity1 : buyItems)
		{
			if (CanBuyItem(traderInfo, buyClassname1))
			{
				resultPrice = resultPrice - CalculateBuyPrice(traderInfo, traderData, buyClassname1, buyQuantity1);
			}
		}
		
		if (!isPlayerAdmin)
		{
			if (resultPrice < 0)
			{
				return;
			}
		}
		
		// Sell items		
		TStringArray updateDbQueries = new TStringArray;
		foreach (ItemBase sellItem2 : sellItems)
		{
			if (sellItem2 && (CanSellItem(traderInfo, sellItem2) || isPlayerAdmin) )
			{
				string classname = sellItem2.GetType();
				float maxQuantity = CalculateTraiderItemQuantityMax(traderInfo, classname);
				float itemQuantity = CalculateItemQuantity01(sellItem2);
				float newValue = 0;
				
				if (traderData.m_items.Contains(classname))
				{
					newValue = Math.Min(maxQuantity, traderData.m_items.Get(classname) + itemQuantity);
					traderData.m_items.Set( classname, newValue );
					updateDbQueries.Insert("UPDATE trader_data SET data = '" + newValue + "' WHERE trader_id = " + traderId + " AND classname = '" + classname + "';");
				}
				else
				{
					newValue = Math.Min(maxQuantity, itemQuantity); 
					traderData.m_items.Set( classname, newValue );
					updateDbQueries.Insert("INSERT INTO trader_data (trader_id, classname, data) VALUES (" + traderId + ", '" + classname + "', '" + newValue + "');");
				}
				
				SybLogSrv("TRADER " + traderId + " BUY ITEM " + classname + ":" + itemQuantity);
			}
		}
		
		foreach (string buyClassname2, float buyQuantity2 : buyItems)
		{
			if (CanBuyItem(traderInfo, buyClassname2) || isPlayerAdmin)
			{
				if (traderData.m_items.Contains(buyClassname2) && traderData.m_items.Get(buyClassname2) >= buyQuantity2)
				{
					float newValue2 = Math.Max(0, traderData.m_items.Get(buyClassname2) - buyQuantity2);				
					if (newValue2 == 0)
					{
						traderData.m_items.Remove(buyClassname2);
						updateDbQueries.Insert("DELETE FROM trader_data WHERE trader_id = " + traderId + " AND classname = '" + buyClassname2 + "';");
					}
					else
					{
						traderData.m_items.Set( buyClassname2, newValue2 );
						updateDbQueries.Insert("UPDATE trader_data SET data = '" + newValue2 + "' WHERE trader_id = " + traderId + " AND classname = '" + buyClassname2 + "';");
					}
				}
			}
			
			SybLogSrv("TRADER " + traderId + " SELL ITEM " + buyClassname2 + ":" + buyQuantity2);
		}
		
		// Update database
		GetDatabase().TransactionAsync(SYBERIA_DB_NAME, updateDbQueries, this, "OnUpdateTraderDataDB", null);
		
		// Delete sell items
		foreach (ItemBase sellItem3 : sellItems)
		{
			if (sellItem3)
			{
				GetGame().ObjectDelete(sellItem3);				
			}
		}
		
		// Spawn buy items
		foreach (string buyClassname3, float buyQuantity3 : buyItems)
		{
			if (CanBuyItem(traderInfo, buyClassname3) || isPlayerAdmin)
			{
				float calcQuantity = buyQuantity3;
				while (calcQuantity > 0)
				{
					ItemBase buyEntity;
					InventoryLocation inv_loc = new InventoryLocation;
					if (player.GetInventory().FindFirstFreeLocationForNewEntity(buyClassname3, FindInventoryLocationType.ANY, inv_loc))
					{
						buyEntity = ItemBase.Cast( player.SpawnItemOnLocation(buyClassname3, inv_loc, true) );
					}
					else
					{
						buyEntity = ItemBase.Cast( GetGame().CreateObject(buyClassname3, player.GetPosition()) );
					}
					
					if (buyEntity)
					{
						float spawnQuantity01 = Math.Clamp(calcQuantity, 0, 1);
						if (buyEntity.IsInherited( Magazine ))
						{
							Magazine buyMagazine;
							Class.CastTo(buyMagazine, buyEntity);
							if ( buyEntity.IsInherited( Ammunition_Base ))
							{
								buyMagazine.ServerSetAmmoCount( (int)Math.Round(buyMagazine.GetAmmoMax() * spawnQuantity01) );
							}
							else
							{
								buyMagazine.ServerSetAmmoCount(0);
							}
						}
						else if (GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + buyClassname3 + " liquidContainerType"))
						{
							buyEntity.SetQuantityNormalized(0);
						}
						else
						{
							buyEntity.SetQuantityNormalized(spawnQuantity01);
						}
					}
					else
					{
						break;
					}
					
					calcQuantity = calcQuantity - 1;
				}
			}
		}
		
		// Send response
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_ACTION_TRADER, sender, new Param1<ref PluginTrader_Data>(traderData));
	}
	
	override void OnUpdate(float delta_time)
	{
		super.OnUpdate(delta_time);
		
		m_updateTimer = m_updateTimer + delta_time;
		if (m_updateTimer > 1.0)
		{
			m_updateTimer = 0;
			foreach (int traderId, TraderPoint point : m_traderPoints)
			{
				point.OnTick();
			}
		}
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
	
	static void InitQueries(ref array<string> queries)
	{
		queries.Insert("CREATE TABLE IF NOT EXISTS trader_data (id INTEGER PRIMARY KEY AUTOINCREMENT, trader_id INTEGER, classname TEXT NOT NULL, data TEXT NOT NULL);");
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
    float m_rotation;
};

modded class PluginTrader_Data
{
	void DeserializeFromDb(ref DatabaseResponse response)
	{
		m_items = new map<string, float>;
		
		if (!response)
			return;
		
		for (int i = 0; i < response.GetRowsCount(); i++)
		{
			string classname = response.GetValue(i, 0);
			float quantity = response.GetValue(i, 1).ToFloat();
			bool exist = false;
			
			if (GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + classname))
			{
				exist = true;
			}
			else if (GetGame().ConfigIsExisting(CFG_MAGAZINESPATH + " " + classname))
			{
				exist = true;
			}
			else if (GetGame().ConfigIsExisting(CFG_WEAPONSPATH + " " + classname))
			{
				exist = true;
			}
			
			if (exist)
			{
				m_items.Insert( classname, quantity );
			}
		}
	}
};