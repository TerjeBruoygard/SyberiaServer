class Database
{	
	private RestContext m_restContext;
	
	void Database(RestContext restContext)
	{
		m_restContext = restContext;
	}
	
	/**
	\brief Processes query and returns data immediately (thread blocking operation!)
	*/
	bool QuerySync(string databaseName, string queryText, out DatabaseResponse response)
	{
		string responseData = m_restContext.POST_now(databaseName + "/query", queryText);
		if (responseData == queryText)
		{
			return false;
		}
		else
		{
			response = new DatabaseResponse(responseData);
			return true;
		}
	}
	
	/**
	\brief Processes query and calls callback function when finished
	*/
	void QueryAsync(string databaseName, string queryText, Class callbackClass, string callbackFnc)
	{
		m_restContext.POST(new DatabaseRestCallback(callbackClass, callbackFnc), databaseName + "/query", queryText);
	}
	
	/**
	\brief Processes transaction (multiple queries) and returns data immediately (thread blocking operation!)
	*/
	bool TransactionSync(string databaseName, ref array<string> queries, out DatabaseResponse response)
	{
		string queryText;
		if (!m_databaseResponseDeserializer.WriteToString(queries, false, queryText))
		{
			return false;
		}
		
		string responseData = m_restContext.POST_now(databaseName + "/transaction", queryText);
		if (responseData == queryText)
		{
			return false;
		}
		else
		{
			response = new DatabaseResponse(responseData);
			return true;
		}
	}
	
	/**
	\brief Processes transaction (multiple queries) and calls callback function when finished
	*/
	void TransactionAsync(string databaseName, ref array<string> queries, Class callbackClass, string callbackFnc)
	{
		string queryText;
		if (!m_databaseResponseDeserializer.WriteToString(queries, false, queryText))
		{
			GetGame().GameScript.CallFunctionParams(
				callbackClass, callbackFnc, null, 
				new Param1<DatabaseResponse>(null));
			
			return;
		}
		
		m_restContext.POST(new DatabaseRestCallback(callbackClass, callbackFnc), databaseName + "/transaction", queryText);
	}
};

class DatabaseRestCallback : RestCallback
{
	private Class m_callbackClass;
	private string m_callbackFnc;
	
	void DatabaseRestCallback(Class callbackClass, string callbackFnc)
	{
		m_callbackClass = callbackClass;
		m_callbackFnc = callbackFnc;
	}
	
	void OnError( int errorCode )
	{
		GetGame().GameScript.CallFunctionParams(
			m_callbackClass, m_callbackFnc, null, 
			new Param1<DatabaseResponse>(null));
	}
	
	void OnTimeout()
	{
		GetGame().GameScript.CallFunctionParams(
			m_callbackClass, m_callbackFnc, null, 
			new Param1<DatabaseResponse>(null));
	}
	
	void OnSuccess( string data, int dataSize )
	{
		if (dataSize == 0)
		{
			GetGame().GameScript.CallFunctionParams(
				m_callbackClass, m_callbackFnc, null, 
				new Param1<DatabaseResponse>(new DatabaseResponse("")));
		}
		else
		{
			GetGame().GameScript.CallFunctionParams(
				m_callbackClass, m_callbackFnc, null, 
				new Param1<DatabaseResponse>(new DatabaseResponse(data)));
		}
	}
};

class DatabaseResponse
{
	ref array<ref array<string>> m_data;	
	
	void DatabaseResponse(string jsonResponse)
	{
		if (jsonResponse.LengthUtf8() == 0)
		{
			m_data = new ref array<ref array<string>>;
			return;
		}
		
		string error;
		if (!m_databaseResponseDeserializer.ReadFromString(m_data, jsonResponse, error))
		{
			Error(error);
		}
	}
	
	ref array<string> GetRow(int index)
	{
		return m_data.Get(index);
	}
	
	string GetValue(int rowIndex, int columnIndex)
	{
		return GetRow(rowIndex).Get(columnIndex);
	}
	
	int GetRowsCount()
	{
		return m_data.Count();
	}
	
	int GetColumnsCount()
	{
		if (GetRowsCount() == 0)
			return 0;
		
		return GetRow(0).Count();
	}
	
	void ~DatabaseResponse()
	{
		if (m_data)
		{
			foreach (ref array<string> row : m_data)
			{
				delete row;
			}
			delete m_data;
		}
	}
};

class DatabaseOptions
{
	int databaseServerPort = 2312;
};

static ref Database m_database = null;
static ref DatabaseOptions m_databaseOptions = null;
static ref JsonSerializer m_databaseResponseDeserializer = new JsonSerializer;
ref Database GetDatabase()
{	
	if (m_database == null) 
	{
		if (!m_databaseOptions)
		{
			string jsonConfigPath = "$profile:\\DatabaseOptions.json";
			if (FileExist(jsonConfigPath))
			{
				JsonFileLoader<ref DatabaseOptions>.JsonLoadFile(jsonConfigPath, m_databaseOptions);
				Print("[DATABASE]: Load config file: " + jsonConfigPath);
			}
			else
			{
				m_databaseOptions = new ref DatabaseOptions;
				JsonFileLoader<ref DatabaseOptions>.JsonSaveFile(jsonConfigPath, m_databaseOptions);
				Print("[DATABASE]: Save config file: " + jsonConfigPath);
			}
		}
		
		RestApi restApi = GetRestApi();
		if (!restApi)
		{
			restApi = CreateRestApi();
		}
		
		RestContext restContext = restApi.GetRestContext("http:/" + "/localhost:" + m_databaseOptions.databaseServerPort + "/");
		m_database = new ref Database(restContext);
		Print("[DATABASE]: Database instance created.");
	}
	
	return m_database;
}