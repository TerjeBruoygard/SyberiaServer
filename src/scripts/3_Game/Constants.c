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
	int sleepingDecPerSec;
    bool sleepingUnconsionEnabled;
    int sleepingIncPerUnconsionBoostValue;
    int sleepingIncPerUnconsionBoostTime;
    int sleepingIncPerSleepingLvl1Sec;
    int sleepingIncPerSleepingLvl2Sec;
    float sleepingHealPerSec01;
    float sleepingHealInfluenzaChance;
	
	// Medicine
	float sepsisStage1TimeSec;
	float sepsisStage2TimeSec;
	float sepsisDeathTimeSec;
	float sepsisHighTemperatureValue;
	float sepsisZombieHitChance;
	float sepsisKnifeHitChance;
	float sepsisBulletHitChance;
	float sepsisDirtySyringeChance;
	float sepsisDitryHandsBandageChance;
	float sepsisDitryHandsSyrgeryChance;
	float zvirusStage1TimeSec;
	float zvirusStage2TimeSec;
	float zvirusDeathTimeSec;
	float zvirusBloodRegenPerSec;
	float zvirusZombieHitChance;
	float zvirusAntidotBloodDmg01;
	float zvirusAntidotHealthDmg01;
	float painLvl1TimeSec;
	float painLvl2TimeSec;
	float painLvl3TimeSec;
	float painMaxDurationSec;
	float bleedingBullethitHealthlosePerSecond;
	float bleedingBullethitOpenPerSecond;
	float bleedingBullethitCupd1PerSecond;
	float bleedingBullethitCupd2PerSecond;
	float bleedingBullethitRemoveBandagedTimeSec;
	float bleedingKnifehitHealthlosePerSecond;
	float bleedingKnifehitOpenPerSecond;
	float bleedingKnifehitCupd1PerSecond;
	float bleedingKnifehitCupd2PerSecond;
	float bleedingKnifehitRemoveBandagedTimeSec;
	float bleedingVisceraBloodlosePerSecond;
	float bleedingVisceraHealthlosePerSecond;
	float bleedingHematomaBloodlosePerSecond;
	float bleedingHematomaHealthlosePerSecond;
	float bleedingZombieHitChance;
	float hematomaPlayerhandsHitChance;
	float hematomaZombieHitChance;
	float hematomaRegenTimerSec;
	float hematomaRegenTimeBoostOnSalve;
	float cuthitRegenTimerSec;
	float visceraKnifehitTorsoChance;
	float visceraBullethitTorsoChance;
	float concussionRegenTimeSec;
	float hemostaticEffectModifier;
	float hematopoiesisEffectBloodPerSec;
	float adrenalinEffectShock01Lvl1PerSec;
	float adrenalinEffectShock01Lvl2PerSec;
	float adrenalinEffectShock01Lvl3PerSec;
	float overdoseDecrementPerSec;
	float overdoseUnconChangePerSec;
	float mindstateHealPerSec;
	int mindstateEmptyDeadtimeSec;
	ref array<float> influenzaIncubatePeriodsSec = new array<float>;
	ref array<float> influenzaTemperatureLevels = new array<float>;
	ref array<float> influenzaSympthomChance = new array<float>;
	ref array<float> influenzaDeathTime = new array<float>;
	float influenzaApplyOnColdWarnChance;
	float influenzaApplyOnColdCritChance;
	float antibioticsGlobalEffectivityModifier;
	ref array<float> stomatchpoisonVomitChance = new array<float>;
	ref array<int> stomatchpoisonWaterDrainFromVomit = new array<int>;
	ref array<int> stomatchpoisonEnergyDrainFromVomit = new array<int>;
	float stomatchhealModifier;
	ref array<int> stomatchpoisonRawMeat = new array<int>;
	ref array<int> stomatchpoisonBurnedMeat = new array<int>;
	ref array<int> stomatchpoisonRottenMeat = new array<int>;
	ref array<int> stomatchpoisonBurnedFood = new array<int>;
	ref array<int> stomatchpoisonRottenFood = new array<int>;
	ref array<int> stomatchpoisonDirtyHands = new array<int>;
	ref array<int> stomatchpoisonInfection = new array<int>;
	float stomatchpoisonChanceDrinkPond;
	ref array<int> stomatchpoisonDrinkPond = new array<int>;
	float stomatchpoisonChanceDrinkWell;
	ref array<int> stomatchpoisonDrinkWell = new array<int>;
	ref array<int> stomatchpoisonDefaultTimes = new array<int>;
	
	// Zombies
	bool zombieOpenDoorEnable;
	float zombieOpenDoorDistance;
	float zombieFindHouseDistance;
	
	// Temperature
	float temperatureHighConsumeItemsMod;
	float temperatureLowConsumeItemsMod;
	float temperatureDrinkPondPenalty;
	float temperatureDrinkWellPenalty;
	float temperatureFillWaterBottleTotal;
	float temperatureFillWaterBottleMod;
	
	// Others
	int bookPagesCountToCut;
	
	override protected void Init()
	{
		super.Init();
		
		// Sleeping
		sleepingDecPerSec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingDecPerSec");
	    sleepingUnconsionEnabled = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingUnconsionEnabled") == 1;
	    sleepingIncPerUnconsionBoostValue = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostValue");
	    sleepingIncPerUnconsionBoostTime = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostTime");
	    sleepingIncPerSleepingLvl1Sec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl1Sec");
	    sleepingIncPerSleepingLvl2Sec = GetGame().ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl2Sec");
	    sleepingHealPerSec01 = GetGame().ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealPerSec01");
	    sleepingHealInfluenzaChance = GetGame().ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealInfluenzaChance");
		
		// Medicine
		sepsisStage1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage1TimeSec");
		sepsisStage2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage2TimeSec");
		sepsisDeathTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDeathTimeSec");
		sepsisHighTemperatureValue = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisHighTemperatureValue");
		sepsisZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisZombieHitChance");
		sepsisKnifeHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisKnifeHitChance");
		sepsisBulletHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisBulletHitChance");
		sepsisDirtySyringeChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDirtySyringeChance");
		sepsisDitryHandsBandageChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDitryHandsBandageChance");
		sepsisDitryHandsSyrgeryChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem sepsisDitryHandsSyrgeryChance");
		zvirusStage1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage1TimeSec");
		zvirusStage2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage2TimeSec");
		zvirusDeathTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusDeathTimeSec");
		zvirusBloodRegenPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusBloodRegenPerSec");
		zvirusZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusZombieHitChance");
		zvirusAntidotBloodDmg01 = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotBloodDmg01");
		zvirusAntidotHealthDmg01 = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotHealthDmg01");
		painLvl1TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl1TimeSec");
		painLvl2TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl2TimeSec");
		painLvl3TimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painLvl3TimeSec");
		painMaxDurationSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem painMaxDurationSec");
		bleedingBullethitHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitHealthlosePerSecond");
		bleedingBullethitOpenPerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitOpenPerSecond");
		bleedingBullethitCupd1PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd1PerSecond");
		bleedingBullethitCupd2PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd2PerSecond");
		bleedingBullethitRemoveBandagedTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitRemoveBandagedTimeSec");
		bleedingKnifehitHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitHealthlosePerSecond");
		bleedingKnifehitOpenPerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitOpenPerSecond");
		bleedingKnifehitCupd1PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd1PerSecond");
		bleedingKnifehitCupd2PerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd2PerSecond");
		bleedingKnifehitRemoveBandagedTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitRemoveBandagedTimeSec");
		bleedingVisceraBloodlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraBloodlosePerSecond");
		bleedingVisceraHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraHealthlosePerSecond");
		bleedingHematomaBloodlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaBloodlosePerSecond");
		bleedingHematomaHealthlosePerSecond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaHealthlosePerSecond");
		bleedingZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem bleedingZombieHitChance");
		hematomaPlayerhandsHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaPlayerhandsHitChance");
		hematomaZombieHitChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaZombieHitChance");
		hematomaRegenTimerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimerSec");
		hematomaRegenTimeBoostOnSalve = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimeBoostOnSalve");
		cuthitRegenTimerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem cuthitRegenTimerSec");
		visceraKnifehitTorsoChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem visceraKnifehitTorsoChance");
		visceraBullethitTorsoChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem visceraBullethitTorsoChance");
		concussionRegenTimeSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem concussionRegenTimeSec");
		hemostaticEffectModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hemostaticEffectModifier");
		hematopoiesisEffectBloodPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem hematopoiesisEffectBloodPerSec");
		adrenalinEffectShock01Lvl1PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl1PerSec");
		adrenalinEffectShock01Lvl2PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl2PerSec");
		adrenalinEffectShock01Lvl3PerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem adrenalinEffectShock01Lvl3PerSec");
		overdoseDecrementPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem overdoseDecrementPerSec");
		overdoseUnconChangePerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem overdoseUnconChangePerSec");
		mindstateHealPerSec = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem mindstateHealPerSec");
		mindstateEmptyDeadtimeSec = GetGame().ConfigGetInt("CfgSyberia MedicineSystem mindstateEmptyDeadtimeSec");
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaIncubatePeriodsSec", influenzaIncubatePeriodsSec);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaTemperatureLevels", influenzaTemperatureLevels);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaSympthomChance", influenzaSympthomChance);
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaDeathTime", influenzaDeathTime);
		influenzaApplyOnColdWarnChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdWarnChance");
		influenzaApplyOnColdCritChance = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdCritChance");
		antibioticsGlobalEffectivityModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem antibioticsGlobalEffectivityModifier");
		GetGame().ConfigGetFloatArray("CfgSyberia MedicineSystem stomatchpoisonVomitChance", stomatchpoisonVomitChance);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonWaterDrainFromVomit", stomatchpoisonWaterDrainFromVomit);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonEnergyDrainFromVomit", stomatchpoisonEnergyDrainFromVomit);
		stomatchhealModifier = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchhealModifier");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRawMeat", stomatchpoisonRawMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedMeat", stomatchpoisonBurnedMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenMeat", stomatchpoisonRottenMeat);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedFood", stomatchpoisonBurnedFood);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenFood", stomatchpoisonRottenFood);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDirtyHands", stomatchpoisonDirtyHands);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonInfection", stomatchpoisonInfection);
		stomatchpoisonChanceDrinkPond = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkPond");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkPond", stomatchpoisonDrinkPond);
		stomatchpoisonChanceDrinkWell = GetGame().ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkWell");
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkWell", stomatchpoisonDrinkWell);
		GetGame().ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDefaultTimes", stomatchpoisonDefaultTimes);
		
		// Zombies
		zombieOpenDoorEnable = GetGame().ConfigGetInt("CfgSyberia ZombieSystem zombieOpenDoorEnable") == 1;
		zombieOpenDoorDistance = GetGame().ConfigGetFloat("CfgSyberia ZombieSystem zombieOpenDoorDistance");
		zombieFindHouseDistance = GetGame().ConfigGetFloat("CfgSyberia ZombieSystem zombieFindHouseDistance");
		
		// Temperature
		temperatureHighConsumeItemsMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureHighConsumeItemsMod");
		temperatureLowConsumeItemsMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureLowConsumeItemsMod");
		temperatureDrinkPondPenalty = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureDrinkPondPenalty");
		temperatureDrinkWellPenalty = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureDrinkWellPenalty");
		temperatureFillWaterBottleTotal = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureFillWaterBottleTotal");
		temperatureFillWaterBottleMod = GetGame().ConfigGetFloat("CfgSyberia TemperatureSystem temperatureFillWaterBottleMod");
		
		// Others
		bookPagesCountToCut = GetGame().ConfigGetInt("CfgSyberia IngameSystem bookPagesCountToCut");
	}
};