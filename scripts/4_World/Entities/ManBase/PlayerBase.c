modded class PlayerBase
{
	bool IsGhostBody()
	{
		return (GetType().Contains("_Ghost"));
	}
}