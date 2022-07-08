modded class CAContinuousMineWood
{
	override void CreatePrimaryItems(ActionData action_data)
	{
		if (action_data.m_Player)
		{
			action_data.m_Player.AddExperience(SyberiaSkillType.SYBSKILL_STRENGTH, GetSyberiaConfig().m_skillsExpStrengthMineWoodOrStone);
		}
		
		super.CreatePrimaryItems(action_data);
	}
};