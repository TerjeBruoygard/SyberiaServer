modded class SkillsContainer
{	
	string SerializeAsDbField()
	{
		string result;
		
		foreach (int id, float val : m_values)
		{
			result = result + id + ":" + val + ";";
		}
		
		return result;
	}
	
	void DeserializeAsDbField(string fieldValue)
	{
		m_values.Clear();
		
		array<string> skillPairs = new array<string>;
		array<string> partPairs = new array<string>;
		fieldValue.Split(";", skillPairs);
		
		foreach (string skillPair : skillPairs)
		{
			string trimedPair = skillPair.Trim();
			if (trimedPair == "") continue;
			
			partPairs.Clear();
			trimedPair.Split(":", partPairs);
			if (partPairs.Count() == 2)
			{
				int id = partPairs.Get(0).ToInt();
				float value = partPairs.Get(1).ToFloat();
				
				SetValue(id, value);
			}
		}
	}
	
	void AddExpirience(int id, float value)
	{
		int skillLevel = (int)GetValue(id);
		float skillMod = GetSyberiaOptions().m_skillModifiers.Get(id);
		float skillInc = (value * skillMod) / SkillsContainer.CalculateLevelSize(skillLevel);
		AddValue(id, skillInc);
	}
};