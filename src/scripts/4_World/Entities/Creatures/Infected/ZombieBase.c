modded class ZombieBase extends DayZInfected
{
	float m_doorsCheckTimer;
	
	override void Init()
	{
		super.Init();
		m_doorsCheckTimer = 0;
	}
	
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (GetSyberiaConfig().m_zombieOpenDoorEnable && m_ActualTarget)
		{
			m_doorsCheckTimer = m_doorsCheckTimer + pDt;		
			if (m_doorsCheckTimer > 1.5)
			{
				m_doorsCheckTimer = 0;
				HandleHouseDoors();
			}
		}
		
		return false;
	}
	
	private void HandleHouseDoors()
	{
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition(GetPosition(), GetSyberiaConfig().m_zombieFindHouseDistance, objects, NULL);

		if (objects.Count() > 0)
		{
			foreach (Object obj : objects)
			{
				Building building;
				if( Building.CastTo(building, obj) )
				{
					HandleBuilding(building);
				}
			}
		}
	}
	
	private void HandleBuilding(Building building)
	{
		int doorsCount = GetGame().ConfigGetChildrenCount("CfgVehicles " + building.GetType() + " Doors");
		for (int i = 0; i < doorsCount; i++)
		{
			if ( !building.IsDoorOpen(i) && !building.IsDoorLocked(i))
			{
				vector doorPos = building.GetDoorSoundPos(i);
				if (vector.Distance(doorPos, GetPosition()) < GetSyberiaConfig().m_zombieOpenDoorDistance)
				{
					building.OpenDoor(i);
					SyberiaSoundEmitter.Spawn("ZombieDoorBreak" + Math.RandomIntInclusive(1, 5) + "_SoundEmitter", doorPos);
				}
			}
		}
	}
};