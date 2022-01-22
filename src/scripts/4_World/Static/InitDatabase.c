class SyberiaDatabaseInit
{
	static bool m_initialized = false;
	
	static void InitIfNot()
	{
		if (!m_initialized)
		{
			m_initialized = true;
			
			DatabaseResponse response;
			TStringArray queries = new TStringArray;
			
			// Init queries start
			CharProfile.InitQueries(queries);
			PluginSyberiaOptions_GroupFaction.InitQueries(queries);
			PluginTrader.InitQueries(queries);
			PluginBuildingSystem.InitQueries(queries);
			// Init queries end
			
			GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);
			
			// Update database
			queries.Clear();	
			CharProfile.UpgradeQueries(queries);
			foreach (string query : queries)
			{
				GetDatabase().QueryNoStrictSync(SYBERIA_DB_NAME, query);
			}
		}
	}
};