modded class BrokenLegsMdfr
{
	override void OnActivate(PlayerBase player)
	{
		super.OnActivate(player);		
		player.GetBleedingManagerServer().SetPainLevel(2);
	}
};