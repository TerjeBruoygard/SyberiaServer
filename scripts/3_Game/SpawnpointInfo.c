class SpawnpointInfo
{
	string m_name;
	vector m_pos;
	int m_radius;
	
	void SpawnpointInfo(string name, vector pos, int radius)
	{
		m_name = name;
		m_pos = pos;
		m_radius = radius;
	}
};