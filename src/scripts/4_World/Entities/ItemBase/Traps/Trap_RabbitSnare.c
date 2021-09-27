modded class Trap_RabbitSnare
{
	protected float m_skillModifier = 0;
	protected ref array<string> m_catchesEntitites;
	
	override void EEInit()
	{
		super.EEInit();
		m_CatchesGroundAnimal.Clear();
		m_MinimalDistanceFromPlayersToCatch = 100;

		m_catchesEntitites = new array<string>;
		m_catchesEntitites.Insert("DeadRooster");
		m_catchesEntitites.Insert("DeadChicken_White");
		m_catchesEntitites.Insert("DeadChicken_Spotted");
		m_catchesEntitites.Insert("DeadChicken_Brown");
		
		// ALWAYS keep rabbit last as that is how it gets the rabbit in case of rabbit specific bait
		m_catchesEntitites.Insert("DeadRabbit");
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		ctx.Write( m_skillModifier );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{   
		if ( !super.OnStoreLoad(ctx, version) )
			return false;
		
		if ( !ctx.Read( m_skillModifier ) )
		{
			m_skillModifier = 0;
			return false;
		}
		
		return true;
	}
	
    override void SetupTrapPlayer( PlayerBase player, bool set_position = true )
	{
		super.SetupTrapPlayer(player, set_position);
		
		if (player)
		{
			m_skillModifier = player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_USE_TRAPS, 0, 0);
			SybLogSrv("Snare trap " + this + " setup with skill modifier: " + m_skillModifier);
		}
	}
	
	override void SpawnCatch()
	{
		super.SpawnCatch();
		
		if ( m_catchesEntitites && m_catchesEntitites.Count() > 0 )
		{
			int count = m_catchesEntitites.Count() - 1;
			int randomCatchIndex = Math.RandomInt( 0, count );		
			if ( m_Bait && Math.RandomFloat01() < m_skillModifier )
			{
				ItemBase catch;
				if ( m_Bait.IsInherited( Worm ) )
				{
					// We can only catch chicken, so exclude the rabbit
					randomCatchIndex = Math.RandomInt( 0, count - 1 );
					catch = ItemBase.Cast( GetGame().CreateObjectEx( m_catchesEntitites.Get( randomCatchIndex ), m_PreyPos, ECE_PLACE_ON_SURFACE ) );
				}
				else	
				{
					// Get the last index, which is the rabbit
					randomCatchIndex = count;
					catch = ItemBase.Cast( GetGame().CreateObjectEx( m_catchesEntitites.Get( randomCatchIndex ), m_PreyPos, ECE_PLACE_ON_SURFACE ) );
				}
				
				// We set quantity of prey
				// if ( catch ) CatchSetQuant( catch );
			}
			
			// We update the state
			SetUsed();
			
			// We remove the bait from this trap
			if ( m_Bait )
			{
				m_Bait.Delete();
			}
		}
	}
};