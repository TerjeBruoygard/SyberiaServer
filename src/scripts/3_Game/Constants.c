const string SyberiaServer_ModPreffix = "[SYBERIA SERVER] ";
const bool SyberiaServer_DebugMode = false;

void SybLogSrv(string message)
{
	if (SyberiaServer_DebugMode) Print(SyberiaServer_ModPreffix + message);
}

const string SYBERIA_DB_NAME = "Syberia";

// STORAGE VERSIONING
const int SYBERIA_V100_VERSION = 7834100;

modded class SyberiaConfig
{
	// Sleeping
	int m_sleepingDecPerSec;
    bool m_sleepingUnconsionEnabled;
    int m_sleepingIncPerUnconsionBoostValue;
    int m_sleepingIncPerUnconsionBoostTime;
    int m_sleepingIncPerSleepingLvl1Sec;
    int m_sleepingIncPerSleepingLvl2Sec;
    float m_sleepingHealPerSec01;
    float m_sleepingHealInfluenzaChance;
	
	// Medicine
	float m_sepsisStage1TimeSec;
	float m_sepsisStage2TimeSec;
	float m_sepsisDeathTimeSec;
	float m_sepsisHighTemperatureValue;
	float m_sepsisZombieHitChance;
	float m_sepsisKnifeHitChance;
	float m_sepsisBulletHitChance;
	float m_sepsisDirtySyringeChance;
	float m_sepsisDitryHandsBandageChance;
	float m_sepsisDitryHandsSyrgeryChance;
	float m_zvirusStage1TimeSec;
	float m_zvirusStage2TimeSec;
	float m_zvirusDeathTimeSec;
	float m_zvirusBloodRegenPerSec;
	float m_zvirusZombieHitChance;
	float m_zvirusAntidotBloodDmg01;
	float m_zvirusAntidotHealthDmg01;
	float m_painLvl1TimeSec;
	float m_painLvl2TimeSec;
	float m_painLvl3TimeSec;
	float m_painMaxDurationSec;
	float m_bleedingBullethitHealthlosePerSecond;
	float m_bleedingBullethitOpenPerSecond;
	float m_bleedingBullethitCupd1PerSecond;
	float m_bleedingBullethitCupd2PerSecond;
	float m_bleedingBullethitRemoveBandagedTimeSec;
	float m_bleedingKnifehitHealthlosePerSecond;
	float m_bleedingKnifehitOpenPerSecond;
	float m_bleedingKnifehitCupd1PerSecond;
	float m_bleedingKnifehitCupd2PerSecond;
	float m_bleedingKnifehitRemoveBandagedTimeSec;
	float m_bleedingVisceraBloodlosePerSecond;
	float m_bleedingVisceraHealthlosePerSecond;
	float m_bleedingHematomaBloodlosePerSecond;
	float m_bleedingHematomaHealthlosePerSecond;
	float m_bleedingZombieHitChance;
	float m_hematomaPlayerhandsHitChance;
	float m_hematomaZombieHitChance;
	float m_hematomaRegenTimerSec;
	float m_hematomaRegenTimeBoostOnSalve;
	float m_cuthitRegenTimerSec;
	float m_visceraKnifehitTorsoChance;
	float m_visceraBullethitTorsoChance;
	float m_concussionRegenTimeSec;
	float m_hemostaticEffectModifier;
	float m_hematopoiesisEffectBloodPerSec;
	float m_adrenalinEffectShock01Lvl1PerSec;
	float m_adrenalinEffectShock01Lvl2PerSec;
	float m_adrenalinEffectShock01Lvl3PerSec;
	float m_overdoseDecrementPerSec;
	float m_overdoseUnconChangePerSec;
	float m_mindstateHealPerSec;
	int m_mindstateEmptyDeadtimeSec;
	ref array<float> m_influenzaIncubatePeriodsSec = new array<float>;
	ref array<float> m_influenzaTemperatureLevels = new array<float>;
	ref array<float> m_influenzaSympthomChance = new array<float>;
	ref array<float> m_influenzaDeathTime = new array<float>;
	float m_influenzaApplyOnColdWarnChance;
	float m_influenzaApplyOnColdCritChance;
	float m_antibioticsGlobalEffectivityModifier;
	ref array<float> m_stomatchpoisonVomitChance = new array<float>;
	ref array<int> m_stomatchpoisonWaterDrainFromVomit = new array<int>;
	ref array<int> m_stomatchpoisonEnergyDrainFromVomit = new array<int>;
	float m_stomatchhealModifier;
	ref array<int> m_stomatchpoisonRawMeat = new array<int>;
	ref array<int> m_stomatchpoisonBurnedMeat = new array<int>;
	ref array<int> m_stomatchpoisonRottenMeat = new array<int>;
	ref array<int> m_stomatchpoisonBurnedFood = new array<int>;
	ref array<int> m_stomatchpoisonRottenFood = new array<int>;
	ref array<int> m_stomatchpoisonDirtyHands = new array<int>;
	ref array<int> m_stomatchpoisonInfection = new array<int>;
	float m_stomatchpoisonChanceDrinkPond;
	ref array<int> m_stomatchpoisonDrinkPond = new array<int>;
	float m_stomatchpoisonChanceDrinkWell;
	ref array<int> m_stomatchpoisonDrinkWell = new array<int>;
	ref array<int> m_stomatchpoisonDefaultTimes = new array<int>;
	
	// Zombies
	bool m_zombieOpenDoorEnable;
	float m_zombieOpenDoorDistance;
	float m_zombieFindHouseDistance;
	
	// Temperature
	float m_temperatureHighConsumeItemsMod;
	float m_temperatureLowConsumeItemsMod;
	float m_temperatureDrinkPondPenalty;
	float m_temperatureDrinkWellPenalty;
	float m_temperatureFillWaterBottleTotal;
	float m_temperatureFillWaterBottleMod;
	
	// Others
	int m_bookPagesCountToCut;
	
	override protected void Init()
	{
		super.Init();
		
		// Sleeping
		m_sleepingDecPerSec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingDecPerSec");
	    m_sleepingUnconsionEnabled = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingUnconsionEnabled") == 1;
	    m_sleepingIncPerUnconsionBoostValue = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostValue");
	    m_sleepingIncPerUnconsionBoostTime = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostTime");
	    m_sleepingIncPerSleepingLvl1Sec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl1Sec");
	    m_sleepingIncPerSleepingLvl2Sec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl2Sec");
	    m_sleepingHealPerSec01 = GetGame().ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealPerSec01");
	    m_sleepingHealInfluenzaChance = GetGame().ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealInfluenzaChance");
		
		// Medicine
		m_sepsisStage1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage1TimeSec");
		m_sepsisStage2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage2TimeSec");
		m_sepsisDeathTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDeathTimeSec");
		m_sepsisHighTemperatureValue = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisHighTemperatureValue");
		m_sepsisZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisZombieHitChance");
		m_sepsisKnifeHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisKnifeHitChance");
		m_sepsisBulletHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisBulletHitChance");
		m_sepsisDirtySyringeChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDirtySyringeChance");
		m_sepsisDitryHandsBandageChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDitryHandsBandageChance");
		m_sepsisDitryHandsSyrgeryChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDitryHandsSyrgeryChance");
		m_zvirusStage1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage1TimeSec");
		m_zvirusStage2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage2TimeSec");
		m_zvirusDeathTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusDeathTimeSec");
		m_zvirusBloodRegenPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusBloodRegenPerSec");
		m_zvirusZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusZombieHitChance");
		m_zvirusAntidotBloodDmg01 = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotBloodDmg01");
		m_zvirusAntidotHealthDmg01 = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotHealthDmg01");
		m_painLvl1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl1TimeSec");
		m_painLvl2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl2TimeSec");
		m_painLvl3TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl3TimeSec");
		m_painMaxDurationSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painMaxDurationSec");
		m_bleedingBullethitHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitHealthlosePerSecond");
		m_bleedingBullethitOpenPerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitOpenPerSecond");
		m_bleedingBullethitCupd1PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd1PerSecond");
		m_bleedingBullethitCupd2PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd2PerSecond");
		m_bleedingBullethitRemoveBandagedTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitRemoveBandagedTimeSec");
		m_bleedingKnifehitHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitHealthlosePerSecond");
		m_bleedingKnifehitOpenPerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitOpenPerSecond");
		m_bleedingKnifehitCupd1PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd1PerSecond");
		m_bleedingKnifehitCupd2PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd2PerSecond");
		m_bleedingKnifehitRemoveBandagedTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitRemoveBandagedTimeSec");
		m_bleedingVisceraBloodlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraBloodlosePerSecond");
		m_bleedingVisceraHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraHealthlosePerSecond");
		m_bleedingHematomaBloodlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaBloodlosePerSecond");
		m_bleedingHematomaHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaHealthlosePerSecond");
		m_bleedingZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingZombieHitChance");
		m_hematomaPlayerhandsHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaPlayerhandsHitChance");
		m_hematomaZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaZombieHitChance");
		m_hematomaRegenTimerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimerSec");
		m_hematomaRegenTimeBoostOnSalve = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimeBoostOnSalve");
		m_cuthitRegenTimerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem cuthitRegenTimerSec");
		m_visceraKnifehitTorsoChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem visceraKnifehitTorsoChance");
		m_visceraBullethitTorsoChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem visceraBullethitTorsoChance");
		m_concussionRegenTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem concussionRegenTimeSec");
		m_hemostaticEffectModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hemostaticEffectModifier");
		m_hematopoiesisEffectBloodPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematopoiesisEffectBloodPerSec");
		m_adrenalinEffectShock01Lvl1PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl1PerSec");
		m_adrenalinEffectShock01Lvl2PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl2PerSec");
		m_adrenalinEffectShock01Lvl3PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl3PerSec");
		m_overdoseDecrementPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem overdoseDecrementPerSec");
		m_overdoseUnconChangePerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem overdoseUnconChangePerSec");
		m_mindstateHealPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem mindstateHealPerSec");
		m_mindstateEmptyDeadtimeSec = GetGame().ConfigGetInt("CfgSyberia MedicineSystem mindstateEmptyDeadtimeSec");
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaIncubatePeriodsSec", m_influenzaIncubatePeriodsSec);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaTemperatureLevels", m_influenzaTemperatureLevels);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaSympthomChance", m_influenzaSympthomChance);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaDeathTime", m_influenzaDeathTime);
		m_influenzaApplyOnColdWarnChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdWarnChance");
		m_influenzaApplyOnColdCritChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdCritChance");
		m_antibioticsGlobalEffectivityModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem antibioticsGlobalEffectivityModifier");
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem stomatchpoisonVomitChance", m_stomatchpoisonVomitChance);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonWaterDrainFromVomit", m_stomatchpoisonWaterDrainFromVomit);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonEnergyDrainFromVomit", m_stomatchpoisonEnergyDrainFromVomit);
		m_stomatchhealModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchhealModifier");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRawMeat", m_stomatchpoisonRawMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedMeat", m_stomatchpoisonBurnedMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenMeat", m_stomatchpoisonRottenMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedFood", m_stomatchpoisonBurnedFood);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenFood", m_stomatchpoisonRottenFood);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDirtyHands", m_stomatchpoisonDirtyHands);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonInfection", m_stomatchpoisonInfection);
		m_stomatchpoisonChanceDrinkPond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkPond");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkPond", m_stomatchpoisonDrinkPond);
		m_stomatchpoisonChanceDrinkWell = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkWell");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkWell", m_stomatchpoisonDrinkWell);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDefaultTimes", m_stomatchpoisonDefaultTimes);
		
		// Zombies
		m_zombieOpenDoorEnable = GetGame().ConfigGetInt("CfgSyberia ZombieSystem zombieOpenDoorEnable") == 1;
		m_zombieOpenDoorDistance = GetGame().ConfigGetFloat("CfgSyberia ZombieSystem zombieOpenDoorDistance");
		m_zombieFindHouseDistance = GetGame().ConfigGetFloat("CfgSyberia ZombieSystem zombieFindHouseDistance");
		
		// Temperature
		m_temperatureHighConsumeItemsMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureHighConsumeItemsMod");
		m_temperatureLowConsumeItemsMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureLowConsumeItemsMod");
		m_temperatureDrinkPondPenalty = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureDrinkPondPenalty");
		m_temperatureDrinkWellPenalty = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureDrinkWellPenalty");
		m_temperatureFillWaterBottleTotal = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureFillWaterBottleTotal");
		m_temperatureFillWaterBottleMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureFillWaterBottleMod");
		
		// Others
		m_bookPagesCountToCut = GetGame().ConfigGetInt("CfgSyberia IngameSystem bookPagesCountToCut");
	}
};