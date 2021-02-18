class SpawnpointInfo
{
	string m_name;
	ref array<ref SpawnpointPos> m_positions; 
	
	void SpawnpointInfo(string name)
	{
		m_name = name;
		m_positions = new array<ref SpawnpointPos>;
	}
	
	vector CalculateSpawnpoint()
	{
		if (m_positions.Count() > 0)
		{
			ref SpawnpointPos spawnPos = m_positions.GetRandomElement();
			float angle = Math.PI2 * Math.RandomFloat01();
			vector pos = spawnPos.m_pos;
			
			pos[0] = pos[0] + (spawnPos.m_radius * Math.Cos(angle));
			pos[2] = pos[2] + (spawnPos.m_radius * Math.Sin(angle));
			return pos;
		}
		
		return "0 0 0";
	}
	
	void ~SpawnpointInfo()
	{
		foreach (ref SpawnpointPos pos : m_positions) delete pos;
		delete m_positions;
	}
};

class SpawnpointPos
{
	vector m_pos;
	float m_radius;
	
	void SpawnpointPos(vector pos, float radius)
	{
		m_pos = pos;
		m_radius = radius;	
	}
};