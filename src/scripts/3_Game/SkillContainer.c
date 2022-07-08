modded class SkillsContainer
{
	void SetValue(int id, float value)
	{
		value = Math.Clamp(value, GetMin(id), GetMax(id));
		
		if (m_values.Contains(id))
		{
			m_values.Set(id, value);
		}
		else
		{
			m_values.Insert(id, value);
		}
	}
	
	void AddValue(int id, float value)
	{
		SetValue(id, GetValue(id) + value);
	}
};