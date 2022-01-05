modded class LaughterSymptom
{
    override void OnGetActivatedServer(PlayerBase player)
	{
		if (GetSyberiaConfig().m_disableLaughterSymptom == false)
        {
            super.OnGetActivatedServer(player);
        }
	}
};