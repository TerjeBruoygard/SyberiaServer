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
			// Init queries end
			
			GetDatabase().TransactionSync(SYBERIA_DB_NAME, queries, response);	
		}
	}
};