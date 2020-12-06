ref PlayerBase GetPlayerByIdentity(ref PlayerIdentity identity)
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