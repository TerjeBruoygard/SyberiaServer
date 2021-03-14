modded class SkillsContainer
{	
	string SerializeAsDbField()
	{
		string result;
		
		foreach (int skillId, float skillVal : m_skills)
		{
			result = result + skillId + ":" + skillVal + ";";
		}
		
		result = result + "|";
		
		foreach (int perkId, int perkLvl : m_perks)
		{
			ref PerkInfo perkInfo = PerksCollection.m_Instance.GetPerk(perkId);
			if (perkInfo && perkLvl <= GetSkillValueInt(perkInfo.GetSkillId()))
			{
				result = result + perkId + ":" + perkLvl + ";";
			}
		}
		
		return result;
	}
	
	void DeserializeAsDbField(string fieldValue)
	{
		m_skills.Clear();
		m_perks.Clear();
		
		string trimedPair;
		array<string> mainPairs = new array<string>;
		array<string> mapPairs = new array<string>;
		array<string> partPairs = new array<string>;
		fieldValue.Split("|", mainPairs);
		
		if (mainPairs.Count() >= 1)
		{
			mapPairs.Clear();
			mainPairs.Get(0).Split(";", mapPairs);
			
			foreach (string skillPair1 : mapPairs)
			{
				trimedPair = skillPair1.Trim();
				if (trimedPair == "") continue;
				
				partPairs.Clear();
				trimedPair.Split(":", partPairs);
				if (partPairs.Count() == 2)
				{
					SetSkillValue( partPairs.Get(0).ToInt(), partPairs.Get(1).ToFloat());
				}
			}
		}
		
		if (mainPairs.Count() >= 2)
		{
			mapPairs.Clear();
			mainPairs.Get(1).Split(";", mapPairs);
			
			foreach (string skillPair2 : mapPairs)
			{
				trimedPair = skillPair2.Trim();
				if (trimedPair == "") continue;
				
				partPairs.Clear();
				trimedPair.Split(":", partPairs);
				if (partPairs.Count() == 2)
				{
					int perkId = partPairs.Get(0).ToInt();
					int perkLvl = partPairs.Get(1).ToInt();
					ref PerkInfo perkInfo = PerksCollection.m_Instance.GetPerk(perkId);
					if (perkInfo && perkLvl <= GetSkillValueInt(perkInfo.GetSkillId()))
					{
						SetPerk(perkId, perkLvl);
					}
				}
			}
		}
	}
	
	void AddExpirience(int id, float value)
	{
		int skillLevel = (int)GetSkillValue(id);
		float skillMod = GetSyberiaOptions().m_skillModifiers.Get(id);
		float skillInc = (value * skillMod) / SkillsContainer.CalculateSkillLevelSize(skillLevel);
		AddSkillValue(id, skillInc);
	}
};