ref SkillsContainer CreateDefaultSkillsContainer()
{
	ref SkillsContainer result = new SkillsContainer();
	
	result.m_skills = new array<ref SkillContainer>;	
	result.m_skills.Insert(CreateDefaultSkillContainer("humanity", 0, false));
	result.m_skills.Insert(CreateDefaultSkillContainer("immunity", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("athletics", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("strength", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("medicine", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("stealth", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("survival", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("farming", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("hunting", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("mechanics", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("electronics", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("building", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("weapon", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("driving", 10, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("lockpicking", 0, true));
	result.m_skills.Insert(CreateDefaultSkillContainer("herbalism", 0, true));
	
	result.m_perks = new array<int>();
	
	return result;
}

ref SkillContainer CreateDefaultSkillContainer(string name, float value, bool isBounded)
{
	ref SkillContainer result = new SkillContainer();
	result.m_name = name;
	result.m_value = value;
	result.m_bounded = isBounded;
	result.m_achivments = new array<bool>;
	return result;
}