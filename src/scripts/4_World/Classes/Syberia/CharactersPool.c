void GetMaleCharactersMetadata(ref array<ref CharacterMetadata> result)
{
	int count = GetGame().ConfigGetChildrenCount("CfgSyberia Characters");
	for (int i = 0; i < count; i++)
	{
		string name;
		GetGame().ConfigGetChildName("CfgSyberia Characters", i, name);
		
		string path = "CfgSyberia Characters " + name;
		if (GetGame().ConfigGetTextOut(path + " gender") == "male")
		{
			string facelogo = GetGame().ConfigGetTextOut(path + " facelogo");
			result.Insert(new CharacterMetadata(name, facelogo));
		}
	}
};

void GetFemaleCharactersMetadata(ref array<ref CharacterMetadata> result)
{
	int count = GetGame().ConfigGetChildrenCount("CfgSyberia Characters");
	for (int i = 0; i < count; i++)
	{
		string name;
		GetGame().ConfigGetChildName("CfgSyberia Characters", i, name);
		
		string path = "CfgSyberia Characters " + name;
		if (GetGame().ConfigGetTextOut(path + " gender") == "female")
		{
			string facelogo = GetGame().ConfigGetTextOut(path + " facelogo");
			result.Insert(new CharacterMetadata(name, facelogo));
		}
	}
};