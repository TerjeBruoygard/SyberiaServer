modded class PlayerBase
{
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		if (IsGhostBody())
		{
			return;
		}
		
		super.OnStoreSave(ctx);
	}
	
	bool IsGhostBody()
	{
		return (GetType() == "Survivor_Ghost");
	}
}