modded class BrokenLegsMdfr
{
	override void OnActivate(PlayerBase player)
	{
		super.OnActivate(player);
		#ifdef EXPANSIONMODAI
		if (m_Player.IsAI())
		{
			return;
		}
		#endif
		#ifdef PersonBotsib_serv
		if (m_Player.m_Bot_Editor_name_ai && m_Player.m_Bot_Editor_name_ai != "")
		{
			return;
		}
		#endif	
		player.GetBleedingManagerServer().SetPainLevel(2);
	}
};