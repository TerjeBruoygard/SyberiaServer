modded class DayZPlayerImplement
{
	void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		PlayerBase player = PlayerBase.Cast(this);
		if (player)
		{
			float noiseSkillMod = 1.0 - Math.Clamp(player.GetPerkFloatValue(SyberiaPerkType.SYBPERK_STEALTH_ZOMBIE_AGRO_DEC, 0, 0), 0, 1);
			noiseMultiplier = (noiseMultiplier * 2.0) * noiseSkillMod;
			noiseMultiplier = noiseMultiplier * GetSyberiaConfig().m_zombieDetectPlayerNoiseMultiplier;
		}
		
		super.AddNoise(noisePar, noiseMultiplier);
	}
};