modded class PluginRecipesManager
{	
	ref array<string> m_disabledDefaultRecipes;
	ref array<ref PluginRecipesManager_CustomRecipeData> m_customRecipes;
	
	override protected void PostProcessingRecipes()
	{
		MakeDirectory("$profile:Syberia");
			
		ref PluginRecipesManager_Settings settings = new PluginRecipesManager_Settings;
		string path = "$profile:Syberia\\CraftingSettings.json";
		if (FileExist(path))
		{
			JsonFileLoader<ref PluginRecipesManager_Settings>.JsonLoadFile(path, settings);
		}
		
		settings.m_allDefaultRecipes.Clear();
		foreach (ref RecipeBase recipe : m_RecipeList)
		{
			settings.m_allDefaultRecipes.Insert(recipe.ClassName());
		}

		m_disabledDefaultRecipes = new array<string>;
		foreach (string disabledRecipeClass : settings.m_disabledDefaultRecipes)
		{
			UnregisterRecipe(disabledRecipeClass);
			m_disabledDefaultRecipes.Insert(disabledRecipeClass);
		}
		
		m_customRecipes = settings.m_customRecipes;
		foreach (ref PluginRecipesManager_CustomRecipeData customRecipe : m_customRecipes)
		{
			ref SybCustomRecipe recipeObj = new SybCustomRecipe();
			recipeObj.InitCustom(customRecipe);
			RegisterRecipe(recipeObj);
			SybLogSrv("Custom recipe registered: " + recipeObj.GetName());
		}
	
		JsonFileLoader<ref PluginRecipesManager_Settings>.JsonSaveFile(path, settings);
	}
	
	void SendCraftingSettings(PlayerIdentity identity)
	{
		if (m_disabledDefaultRecipes == null || m_customRecipes == null)
		{
			return;
		}
		
		if (m_disabledDefaultRecipes.Count() == 0 && m_customRecipes.Count() == 0)
		{
			return;
		}
		
		GetSyberiaRPC().SendToClient(SyberiaRPC.SYBRPC_SYNC_CRAFTS, identity, new Param2<ref array<string>, ref array<ref PluginRecipesManager_CustomRecipeData>>(m_disabledDefaultRecipes, m_customRecipes));
	}
};

class PluginRecipesManager_Settings
{
	ref array<string> m_allDefaultRecipes;
	ref array<string> m_disabledDefaultRecipes;
	ref array<ref PluginRecipesManager_CustomRecipeData> m_customRecipes;
	
	void PluginRecipesManager_Settings()
	{
		m_allDefaultRecipes = new array<string>;
		m_disabledDefaultRecipes = new array<string>;
		m_customRecipes = new array<ref PluginRecipesManager_CustomRecipeData>;
	}
};