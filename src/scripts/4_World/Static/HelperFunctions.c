ref PlayerBase GetPlayerByIdentity(PlayerIdentity identity)
{
	ref PlayerBase result = null;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);

	foreach (ref Man man : players)
	{
		ref PlayerBase player = PlayerBase.Cast(man);
		if (player && player.GetIdentity().GetPlayerId() == identity.GetPlayerId())
		{
			result = player;
			break;
		}
	}
	
	delete players;
	return result;
}

ref PlayerBase GetPlayerByGUID(string guid)
{
	ref PlayerBase result = null;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);

	foreach (ref Man man : players)
	{
		ref PlayerBase player = PlayerBase.Cast(man);
		if (player)
		{
			if (player.GetIdentity().GetId() == guid)
			{
				result = player;
				break;
			}
		}
	}
	
	delete players;
	return result;
}

ref PlayerBase GetPlayerByCharId(int id)
{
	ref PlayerBase result = null;
	ref array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);

	foreach (ref Man man : players)
	{
		ref PlayerBase player = PlayerBase.Cast(man);
		if (player && player.m_charProfile && player.m_charProfile.m_id == id)
		{
			result = player;
			break;
		}
	}
	
	delete players;
	return result;
}