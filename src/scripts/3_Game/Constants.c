const string SyberiaServer_ModPreffix = "[SYBERIA SERVER] ";
const bool SyberiaServer_DebugMode = true;
const string Syberia_ServerVersion = "OPEN BETA 0.49L";

modded class SyberiaVersionChecker {
	override void CLIENT_AND_SERVER_MODS_VERSION_MISSMATCH_0_49K() {}
}

void SybLogSrv(string message)
{
	if (SyberiaServer_DebugMode) Print(SyberiaServer_ModPreffix + message);
}

const string SYBERIA_DB_NAME = "Syberia";

// STORAGE VERSIONING
const int SYBERIA_B10_VERSION = 7834100;
const int SYBERIA_B20_VERSION = 7834101;
const int SYBERIA_B49_VERSION = 7834102;

modded class SyberiaConfig
{
	// Sleeping
	int m_sleepingDecPerSec;
    bool m_sleepingUnconsionEnabled;
    int m_sleepingIncPerUnconsionBoostValue;
    int m_sleepingIncPerUnconsionBoostTime;
    float m_sleepingIncPerSleepingLvl1Sec;
    float m_sleepingIncPerSleepingLvl2Sec;
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
	float m_zvirusStage1TimeSec;
	float m_zvirusStage2TimeSec;
	float m_zvirusDeathTimeSec;
	float m_zvirusBloodLosePerSec;
    float m_zvirusHealthLosePerSec;
	float m_zvirusCuthitSpawnChance;
    float m_zvirusPainSpawnChance;
	float m_zvirusVommitSpawnChance;
    float m_zvirusBlindnessSpawnChance;
    float m_zvirusFeverblurSpawnChance;
	float m_zvirusZombieHitChance;
	float m_zvirusZombieBloodTransferChance;
	float m_zvirusAntidotBloodDmg01;
	float m_zvirusAntidotHealthDmg01;
	float m_zvirusOnHitIncrementTimer;
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
	float m_bleedingKnifehitZombieChance;
	float m_bleedingVisceraBloodlosePerSecond;
	float m_bleedingVisceraHealthlosePerSecond;
	float m_bleedingHematomaBloodlosePerSecond;
	float m_bleedingHematomaHealthlosePerSecond;
	float m_bleedingZombieHitChance;
	float m_hematomaPlayerhandsHitChance;
	float m_hematomaZombieHitChance;
	float m_hematomaRegenTimerSec;
	float m_hematomaRegenTimeBoostOnSalve;
	float m_hematomaPainChance;
	float m_cuthitRegenTimerSec;
	float m_visceraKnifehitTorsoChance;
	float m_visceraBullethitTorsoChance;
	float m_concussionRegenTimeSec;
	float m_concussionZombieHitChance;
	float m_hemostaticEffectModifier;
	float m_hematopoiesisEffectBloodPerSec;
	ref array<float> m_adrenalinEffectShockUpPerSec = new array<float>;
	ref array<float> m_adrenalinEffectStaminaDepletionMod = new array<float>;
	ref array<float> m_adrenalinEffectStaminaRecoveryMod = new array<float>;
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
	ref array<int> m_stomatchpoisonGasoline = new array<int>;
	float m_stomatchpoisonChanceDrinkPond;
	ref array<int> m_stomatchpoisonDrinkPond = new array<int>;
	float m_stomatchpoisonChanceDrinkWell;
	ref array<int> m_stomatchpoisonDrinkWell = new array<int>;
	ref array<int> m_stomatchpoisonDefaultTimes = new array<int>;
	float m_startSurgeryVisceraChance;
    float m_startSurgeryBulletChance;
    float m_startSurgeryKnifeChance;
	float m_surgerySideEffectBloodLoseCount;
    float m_surgerySideEffectHealthLoseCount;
	ref array<float> m_radiationDoseDecrementPerSec = new array<float>;
	ref array<float> m_radiationLevels = new array<float>;
	ref array<float> m_radiationHealthDamage = new array<float>;
	float m_radiationSleepingDec;
	float m_shockDecrementOnLowHealthAndBlood;
	ref array<float> m_zombieHitDecreaseMind = new array<float>;
	ref array<float> m_zombieKillDecreaseMind = new array<float>;
	ref array<float> m_antidepresantMindInc = new array<float>;
	bool m_disableLaughterSymptom;
	
	// Zombies
	bool m_zombieOpenDoorEnable;
	float m_zombieOpenDoorDistance;
	float m_zombieFindHouseDistance;
	float m_zombieBloodHandsChanceOnLooting;
	bool m_zombieAttackPlayersInUnconscious;
	float m_zombieDetectPlayerVisibilityMultiplier;
	float m_zombieDetectPlayerNoiseMultiplier;
	
	// Animals
	float m_animalsButchingKnifeDamage;
	
	// Attack system
	float m_damageNonWeaponItemInHandsOnAttack;
	float m_damageGlovesOnHandsOnAttack;
	float m_damageHandsOnAttackChance;
	float m_bodyGuardModifier;
	float m_zombieAttackDistanceChase;
	float m_zombieAttackDistanceFight;
	
	// InGame System
	int m_bookPagesCountToCut;
	float m_gasMaskFilterDegradationInToxicZone;
	float m_gasMaskFilterDegradationInRadZone;
	float m_gasMaskFilterDegradationDefault;
	float m_igniteFireplaceBaseSkillcheckChance;
	float m_igniteFireplaceIndoorAdditionalChance;
	float m_damageClothingInRadiationZone;
	float m_defaultStaminaDepletionMultiplier;
    float m_defaultStaminaRecoveryMultiplier;
	float m_dirtyFruitsSpawnChance;
    float m_dirtyMushroomsSpawnChance;
	
	// Skills Experience System
	float m_skillsExpImmunityPain;
    float m_skillsExpImmunityHematoma;
    float m_skillsExpImmunityInfluenza;
    float m_skillsExpImmunityStomatch;
    float m_skillsExpImmunityZVirus;
    float m_skillsExpImmunitySepsis;
    float m_skillsExpImmunityOverdose;
	float m_skillsExpAthleticsSprintTime;
	float m_skillsExpAthleticsSprintIncrement;	
	float m_skillsExpAthleticsJogTime;
    float m_skillsExpAthleticsJogIncrement;
	float m_skillsExpStrengthLightAttackValue;
    float m_skillsExpStrengthLightAttackChance;
    float m_skillsExpStrengthHeavyAttackValue;
    float m_skillsExpStrengthHeavyAttackChance;
	float m_skillsExpStrengthMineWoodOrStone;	
	float m_skillsExpMedicineBandage;
	float m_skillsExpMedicineTablets;
	float m_skillsExpMedicineAmpoule;
	float m_skillsExpMedicineInjector;
    float m_skillsExpMedicineSurgery;		
	float m_skillsExpStealthSilentAttackValue;
    float m_skillsExpStealthSilentAttackChance;
	float m_skillsExpStealthCrouchTime;
    float m_skillsExpStealthCrouchIncrement;
	float m_skillsExpHuntingButchSmall;
    float m_skillsExpHuntingButchCommon;
    float m_skillsExpHuntingButchWolf;
    float m_skillsExpHuntingButchBear;
	float m_skillsExpSurvivalIgniteFireSuccess;
	float m_skillsExpSurvivalIgniteFireFailed;
	
	// World system
	bool m_isWinterMap;
	float m_airDayTemperatureMin;
    float m_airDayTemperatureMax;
    float m_airNightTemperatureMin;
    float m_airNightTemperatureMax;
	float m_temperatureHighConsumeItemsMod;
	float m_temperatureLowConsumeItemsMod;
	float m_temperatureDrinkPondPenalty;
	float m_temperatureDrinkWellPenalty;
	float m_temperatureFillWaterBottleTotal;
	float m_temperatureFillWaterBottleMod;
	float m_temperatureIceDrain;
    float m_temperatureSnowDrain;
	float m_clothingHeatIsolationGlobal;
	float m_additionalTemperatureResistForSurvivalSkill;
	float m_clothingDrenchedHeatDrain;
	float m_clothingWetHeatDrain;
	float m_temperatureHeightReduction;
	
	override void Init()
	{
		super.Init();
		
        string worldName;
		GetGame().GetWorldName(worldName);
        
		// Sleeping
		m_sleepingDecPerSec = ConfigGetInt("CfgSyberia SleepingSystem sleepingDecPerSec");
	    m_sleepingUnconsionEnabled = ConfigGetInt("CfgSyberia SleepingSystem sleepingUnconsionEnabled") == 1;
	    m_sleepingIncPerUnconsionBoostValue = ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostValue");
	    m_sleepingIncPerUnconsionBoostTime = ConfigGetInt("CfgSyberia SleepingSystem sleepingIncPerUnconsionBoostTime");
	    m_sleepingIncPerSleepingLvl1Sec = ConfigGetFloat("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl1Sec");
	    m_sleepingIncPerSleepingLvl2Sec = ConfigGetFloat("CfgSyberia SleepingSystem sleepingIncPerSleepingLvl2Sec");
	    m_sleepingHealPerSec01 = ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealPerSec01");
	    m_sleepingHealInfluenzaChance = ConfigGetFloat("CfgSyberia SleepingSystem sleepingHealInfluenzaChance");
		
		// Medicine
		m_sepsisStage1TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage1TimeSec");
		m_sepsisStage2TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem sepsisStage2TimeSec");
		m_sepsisDeathTimeSec = ConfigGetFloat("CfgSyberia MedicineSystem sepsisDeathTimeSec");
		m_sepsisHighTemperatureValue = ConfigGetFloat("CfgSyberia MedicineSystem sepsisHighTemperatureValue");
		m_sepsisZombieHitChance = ConfigGetFloat("CfgSyberia MedicineSystem sepsisZombieHitChance");
		m_sepsisKnifeHitChance = ConfigGetFloat("CfgSyberia MedicineSystem sepsisKnifeHitChance");
		m_sepsisBulletHitChance = ConfigGetFloat("CfgSyberia MedicineSystem sepsisBulletHitChance");
		m_sepsisDirtySyringeChance = ConfigGetFloat("CfgSyberia MedicineSystem sepsisDirtySyringeChance");
		m_zvirusStage1TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage1TimeSec");
		m_zvirusStage2TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem zvirusStage2TimeSec");
		m_zvirusDeathTimeSec = ConfigGetFloat("CfgSyberia MedicineSystem zvirusDeathTimeSec");
		m_zvirusBloodLosePerSec = ConfigGetFloat("CfgSyberia MedicineSystem zvirusBloodLosePerSec");
	    m_zvirusHealthLosePerSec = ConfigGetFloat("CfgSyberia MedicineSystem zvirusHealthLosePerSec");
		m_zvirusCuthitSpawnChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusCuthitSpawnChance");
		m_zvirusPainSpawnChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusPainSpawnChance");	
		m_zvirusVommitSpawnChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusVommitSpawnChance");
    	m_zvirusBlindnessSpawnChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusBlindnessSpawnChance");
    	m_zvirusFeverblurSpawnChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusFeverblurSpawnChance");	
		m_zvirusZombieHitChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusZombieHitChance");
		m_zvirusZombieBloodTransferChance = ConfigGetFloat("CfgSyberia MedicineSystem zvirusZombieBloodTransferChance");
		m_zvirusAntidotBloodDmg01 = ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotBloodDmg01");
		m_zvirusAntidotHealthDmg01 = ConfigGetFloat("CfgSyberia MedicineSystem zvirusAntidotHealthDmg01");
		m_zvirusOnHitIncrementTimer = ConfigGetFloat("CfgSyberia MedicineSystem zvirusOnHitIncrementTimer");
		m_painLvl1TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem painLvl1TimeSec");
		m_painLvl2TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem painLvl2TimeSec");
		m_painLvl3TimeSec = ConfigGetFloat("CfgSyberia MedicineSystem painLvl3TimeSec");
		m_painMaxDurationSec = ConfigGetFloat("CfgSyberia MedicineSystem painMaxDurationSec");
		m_bleedingBullethitHealthlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitHealthlosePerSecond");
		m_bleedingBullethitOpenPerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitOpenPerSecond");
		m_bleedingBullethitCupd1PerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd1PerSecond");
		m_bleedingBullethitCupd2PerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitCupd2PerSecond");
		m_bleedingBullethitRemoveBandagedTimeSec = ConfigGetFloat("CfgSyberia MedicineSystem bleedingBullethitRemoveBandagedTimeSec");
		m_bleedingKnifehitHealthlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitHealthlosePerSecond");
		m_bleedingKnifehitOpenPerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitOpenPerSecond");
		m_bleedingKnifehitCupd1PerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd1PerSecond");
		m_bleedingKnifehitCupd2PerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitCupd2PerSecond");
		m_bleedingKnifehitRemoveBandagedTimeSec = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitRemoveBandagedTimeSec");
		m_bleedingKnifehitZombieChance = ConfigGetFloat("CfgSyberia MedicineSystem bleedingKnifehitZombieChance");
		m_bleedingVisceraBloodlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraBloodlosePerSecond");
		m_bleedingVisceraHealthlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingVisceraHealthlosePerSecond");
		m_bleedingHematomaBloodlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaBloodlosePerSecond");
		m_bleedingHematomaHealthlosePerSecond = ConfigGetFloat("CfgSyberia MedicineSystem bleedingHematomaHealthlosePerSecond");
		m_bleedingZombieHitChance = ConfigGetFloat("CfgSyberia MedicineSystem bleedingZombieHitChance");
		m_hematomaPlayerhandsHitChance = ConfigGetFloat("CfgSyberia MedicineSystem hematomaPlayerhandsHitChance");
		m_hematomaZombieHitChance = ConfigGetFloat("CfgSyberia MedicineSystem hematomaZombieHitChance");
		m_hematomaRegenTimerSec = ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimerSec");
		m_hematomaRegenTimeBoostOnSalve = ConfigGetFloat("CfgSyberia MedicineSystem hematomaRegenTimeBoostOnSalve");
		m_cuthitRegenTimerSec = ConfigGetFloat("CfgSyberia MedicineSystem cuthitRegenTimerSec");
		m_visceraKnifehitTorsoChance = ConfigGetFloat("CfgSyberia MedicineSystem visceraKnifehitTorsoChance");
		m_visceraBullethitTorsoChance = ConfigGetFloat("CfgSyberia MedicineSystem visceraBullethitTorsoChance");
		m_concussionRegenTimeSec = ConfigGetFloat("CfgSyberia MedicineSystem concussionRegenTimeSec");
		m_concussionZombieHitChance = ConfigGetFloat("CfgSyberia MedicineSystem concussionZombieHitChance");
		m_hemostaticEffectModifier = ConfigGetFloat("CfgSyberia MedicineSystem hemostaticEffectModifier");
		m_hematopoiesisEffectBloodPerSec = ConfigGetFloat("CfgSyberia MedicineSystem hematopoiesisEffectBloodPerSec");	
		m_hematomaPainChance = ConfigGetFloat("CfgSyberia MedicineSystem hematomaPainChance");	
		ConfigGetFloatArray("CfgSyberia MedicineSystem adrenalinEffectShockUpPerSec", m_adrenalinEffectShockUpPerSec, 3);
		ConfigGetFloatArray("CfgSyberia MedicineSystem adrenalinEffectStaminaDepletionMod", m_adrenalinEffectStaminaDepletionMod, 3);
		ConfigGetFloatArray("CfgSyberia MedicineSystem adrenalinEffectStaminaRecoveryMod", m_adrenalinEffectStaminaRecoveryMod, 3);		
		m_overdoseDecrementPerSec = ConfigGetFloat("CfgSyberia MedicineSystem overdoseDecrementPerSec");
		m_overdoseUnconChangePerSec = ConfigGetFloat("CfgSyberia MedicineSystem overdoseUnconChangePerSec");
		m_mindstateHealPerSec = ConfigGetFloat("CfgSyberia MedicineSystem mindstateHealPerSec");
		m_mindstateEmptyDeadtimeSec = ConfigGetInt("CfgSyberia MedicineSystem mindstateEmptyDeadtimeSec");
		ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaIncubatePeriodsSec", m_influenzaIncubatePeriodsSec, 3);
		ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaTemperatureLevels", m_influenzaTemperatureLevels, 3);
		ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaSympthomChance", m_influenzaSympthomChance, 3);
		ConfigGetFloatArray("CfgSyberia MedicineSystem influenzaDeathTime", m_influenzaDeathTime, 3);
		m_influenzaApplyOnColdWarnChance = ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdWarnChance");
		m_influenzaApplyOnColdCritChance = ConfigGetFloat("CfgSyberia MedicineSystem influenzaApplyOnColdCritChance");
		m_antibioticsGlobalEffectivityModifier = ConfigGetFloat("CfgSyberia MedicineSystem antibioticsGlobalEffectivityModifier");
		ConfigGetFloatArray("CfgSyberia MedicineSystem stomatchpoisonVomitChance", m_stomatchpoisonVomitChance, 3);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonWaterDrainFromVomit", m_stomatchpoisonWaterDrainFromVomit, 3);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonEnergyDrainFromVomit", m_stomatchpoisonEnergyDrainFromVomit, 3);
		m_stomatchhealModifier = ConfigGetFloat("CfgSyberia MedicineSystem stomatchhealModifier");
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRawMeat", m_stomatchpoisonRawMeat, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedMeat", m_stomatchpoisonBurnedMeat, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenMeat", m_stomatchpoisonRottenMeat, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonBurnedFood", m_stomatchpoisonBurnedFood, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonRottenFood", m_stomatchpoisonRottenFood, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDirtyHands", m_stomatchpoisonDirtyHands, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonInfection", m_stomatchpoisonInfection, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonGasoline", m_stomatchpoisonGasoline, 2);
		m_stomatchpoisonChanceDrinkPond = ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkPond");
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkPond", m_stomatchpoisonDrinkPond, 2);
		m_stomatchpoisonChanceDrinkWell = ConfigGetFloat("CfgSyberia MedicineSystem stomatchpoisonChanceDrinkWell");
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDrinkWell", m_stomatchpoisonDrinkWell, 2);
		ConfigGetIntArray("CfgSyberia MedicineSystem stomatchpoisonDefaultTimes", m_stomatchpoisonDefaultTimes, 3);	
		m_startSurgeryVisceraChance = ConfigGetFloat("CfgSyberia MedicineSystem startSurgeryVisceraChance");
	    m_startSurgeryBulletChance = ConfigGetFloat("CfgSyberia MedicineSystem startSurgeryBulletChance");
	    m_startSurgeryKnifeChance = ConfigGetFloat("CfgSyberia MedicineSystem startSurgeryKnifeChance");
		m_surgerySideEffectBloodLoseCount = ConfigGetFloat("CfgSyberia MedicineSystem surgerySideEffectBloodLoseCount");
	    m_surgerySideEffectHealthLoseCount = ConfigGetFloat("CfgSyberia MedicineSystem surgerySideEffectHealthLoseCount");
		ConfigGetFloatArray("CfgSyberia MedicineSystem radiationDoseDecrementPerSec", m_radiationDoseDecrementPerSec, 4);	
		ConfigGetFloatArray("CfgSyberia MedicineSystem radiationLevels", m_radiationLevels, 3);	
		ConfigGetFloatArray("CfgSyberia MedicineSystem radiationHealthDamage", m_radiationHealthDamage, 3);
		m_radiationSleepingDec = ConfigGetFloat("CfgSyberia MedicineSystem radiationSleepingDec");
		m_shockDecrementOnLowHealthAndBlood = ConfigGetFloat("CfgSyberia MedicineSystem shockDecrementOnLowHealthAndBlood");
		ConfigGetFloatArray("CfgSyberia MedicineSystem zombieHitDecreaseMind", m_zombieHitDecreaseMind, 2);	
		ConfigGetFloatArray("CfgSyberia MedicineSystem zombieKillDecreaseMind", m_zombieKillDecreaseMind, 2);	
		ConfigGetFloatArray("CfgSyberia MedicineSystem antidepresantMindInc", m_antidepresantMindInc, 3);
		m_disableLaughterSymptom = ConfigGetInt("CfgSyberia MedicineSystem disableLaughterSymptom") == 1;
		
		// Zombies
		m_zombieOpenDoorEnable = ConfigGetInt("CfgSyberia ZombieSystem zombieOpenDoorEnable") == 1;
		m_zombieOpenDoorDistance = ConfigGetFloat("CfgSyberia ZombieSystem zombieOpenDoorDistance");
		m_zombieFindHouseDistance = ConfigGetFloat("CfgSyberia ZombieSystem zombieFindHouseDistance");
		m_zombieBloodHandsChanceOnLooting = ConfigGetFloat("CfgSyberia ZombieSystem zombieBloodHandsChanceOnLooting");
		m_zombieAttackPlayersInUnconscious = ConfigGetInt("CfgSyberia ZombieSystem zombieAttackPlayersInUnconscious") == 1;
		m_zombieDetectPlayerVisibilityMultiplier = ConfigGetFloat("CfgSyberia ZombieSystem zombieDetectPlayerVisibilityMultiplier");
		m_zombieDetectPlayerNoiseMultiplier = ConfigGetFloat("CfgSyberia ZombieSystem zombieDetectPlayerNoiseMultiplier");
		
		// Animals
		m_animalsButchingKnifeDamage = ConfigGetFloat("CfgSyberia AnimalsSystem animalsButchingKnifeDamage");
		
		// Attack system
		m_damageNonWeaponItemInHandsOnAttack = ConfigGetFloat("CfgSyberia AttackSystem damageNonWeaponItemInHandsOnAttack");
		m_damageGlovesOnHandsOnAttack = ConfigGetFloat("CfgSyberia AttackSystem damageGlovesOnHandsOnAttack");
		m_damageHandsOnAttackChance = ConfigGetFloat("CfgSyberia AttackSystem damageHandsOnAttackChance");
		m_bodyGuardModifier = ConfigGetFloat("CfgSyberia AttackSystem bodyGuardModifier");
		m_zombieAttackDistanceChase = ConfigGetFloat("CfgSyberia AttackSystem zombieAttackDistanceChase");
		m_zombieAttackDistanceFight = ConfigGetFloat("CfgSyberia AttackSystem zombieAttackDistanceFight");
		
		// Others
		m_bookPagesCountToCut = ConfigGetInt("CfgSyberia IngameSystem bookPagesCountToCut");
		m_gasMaskFilterDegradationInToxicZone = ConfigGetFloat("CfgSyberia IngameSystem gasMaskFilterDegradationInToxicZone");
		m_gasMaskFilterDegradationInRadZone = ConfigGetFloat("CfgSyberia IngameSystem gasMaskFilterDegradationInRadZone");
		m_gasMaskFilterDegradationDefault = ConfigGetFloat("CfgSyberia IngameSystem gasMaskFilterDegradationDefault");
		m_igniteFireplaceBaseSkillcheckChance = ConfigGetFloat("CfgSyberia IngameSystem igniteFireplaceBaseSkillcheckChance");
		m_igniteFireplaceIndoorAdditionalChance = ConfigGetFloat("CfgSyberia IngameSystem igniteFireplaceIndoorAdditionalChance");
		m_damageClothingInRadiationZone = ConfigGetFloat("CfgSyberia IngameSystem damageClothingInRadiationZone");
		m_defaultStaminaDepletionMultiplier = ConfigGetFloat("CfgSyberia IngameSystem defaultStaminaDepletionMultiplier");
	    m_defaultStaminaRecoveryMultiplier = ConfigGetFloat("CfgSyberia IngameSystem defaultStaminaRecoveryMultiplier");
		m_dirtyFruitsSpawnChance = ConfigGetFloat("CfgSyberia IngameSystem dirtyFruitsSpawnChance");
		m_dirtyMushroomsSpawnChance = ConfigGetFloat("CfgSyberia IngameSystem dirtyMushroomsSpawnChance");
		
		// Skills Experience System
		m_skillsExpImmunityPain = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityPain");
	    m_skillsExpImmunityHematoma = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityHematoma");
	    m_skillsExpImmunityInfluenza = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityInfluenza");
	    m_skillsExpImmunityStomatch = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityStomatch");
	    m_skillsExpImmunityZVirus = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityZVirus");
	    m_skillsExpImmunitySepsis = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunitySepsis");
	    m_skillsExpImmunityOverdose = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpImmunityOverdose");
		m_skillsExpAthleticsSprintTime = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpAthleticsSprintTime");
	    m_skillsExpAthleticsSprintIncrement = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpAthleticsSprintIncrement");	
		m_skillsExpAthleticsJogTime = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpAthleticsJogTime");	
	    m_skillsExpAthleticsJogIncrement = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpAthleticsJogIncrement");	
		m_skillsExpStrengthLightAttackValue = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStrengthLightAttackValue");
	    m_skillsExpStrengthLightAttackChance = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStrengthLightAttackChance");
	    m_skillsExpStrengthHeavyAttackValue = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStrengthHeavyAttackValue");
	    m_skillsExpStrengthHeavyAttackChance = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStrengthHeavyAttackChance");		
		m_skillsExpStrengthMineWoodOrStone = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStrengthMineWoodOrStone");
		m_skillsExpMedicineBandage = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpMedicineBandage");
		m_skillsExpMedicineTablets = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpMedicineTablets");
		m_skillsExpMedicineAmpoule = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpMedicineAmpoule");
		m_skillsExpMedicineInjector = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpMedicineInjector");
    	m_skillsExpMedicineSurgery = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpMedicineSurgery");		
		m_skillsExpStealthSilentAttackValue = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStealthSilentAttackValue");
	    m_skillsExpStealthSilentAttackChance = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStealthSilentAttackChance");
		m_skillsExpStealthCrouchTime = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStealthCrouchTime");	
		m_skillsExpStealthCrouchIncrement = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpStealthCrouchIncrement");		
		m_skillsExpHuntingButchSmall = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpHuntingButchSmall");
	    m_skillsExpHuntingButchCommon = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpHuntingButchCommon");
	    m_skillsExpHuntingButchWolf = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpHuntingButchWolf");
		m_skillsExpHuntingButchBear = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpHuntingButchBear");
		
		m_skillsExpSurvivalIgniteFireSuccess = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpSurvivalIgniteFireSuccess");
		m_skillsExpSurvivalIgniteFireFailed = ConfigGetFloat("CfgSyberia SkillsExperienceSystem skillsExpSurvivalIgniteFireFailed");
		
		// World system
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem winterWorld")) {
			m_isWinterMap = ConfigGetInt("CfgSyberia WorldSystem winterWorld") == 1;
		}
		else {
			m_isWinterMap = ConfigGetInt("CfgWorlds " + worldName + " winterWorld") == 1;
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem airDayTemperatureMin")) {
			m_airDayTemperatureMin = ConfigGetFloat("CfgSyberia WorldSystem airDayTemperatureMin");
		}
		else {
			m_airDayTemperatureMin = ConfigGetFloat("CfgWorlds " + worldName + " airDayTemperatureMin");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem airDayTemperatureMax")) {
			m_airDayTemperatureMax = ConfigGetFloat("CfgSyberia WorldSystem airDayTemperatureMax");
		}
		else {
			m_airDayTemperatureMax = ConfigGetFloat("CfgWorlds " + worldName + " airDayTemperatureMax");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem airNightTemperatureMin")) {
			m_airNightTemperatureMin = ConfigGetFloat("CfgSyberia WorldSystem airNightTemperatureMin");
		}
		else {
			m_airNightTemperatureMin = ConfigGetFloat("CfgWorlds " + worldName + " airNightTemperatureMin");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem airNightTemperatureMax")) {
			m_airNightTemperatureMax = ConfigGetFloat("CfgSyberia WorldSystem airNightTemperatureMax");
		}
		else {
			m_airNightTemperatureMax = ConfigGetFloat("CfgWorlds " + worldName + " airNightTemperatureMax");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem clothingHeatIsolationGlobal")) {
			m_clothingHeatIsolationGlobal = ConfigGetFloat("CfgSyberia WorldSystem clothingHeatIsolationGlobal");
		}
		else {
			m_clothingHeatIsolationGlobal = ConfigGetFloat("CfgWorlds " + worldName + " clothingHeatIsolationGlobal");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureHighConsumeItemsMod")) {
			m_temperatureHighConsumeItemsMod = ConfigGetFloat("CfgSyberia WorldSystem temperatureHighConsumeItemsMod");
		}
		else {
			m_temperatureHighConsumeItemsMod = ConfigGetFloat("CfgWorlds " + worldName + " temperatureHighConsumeItemsMod");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureLowConsumeItemsMod")) {
			m_temperatureLowConsumeItemsMod = ConfigGetFloat("CfgSyberia WorldSystem temperatureLowConsumeItemsMod");
		}
		else {
			m_temperatureLowConsumeItemsMod = ConfigGetFloat("CfgWorlds " + worldName + " temperatureLowConsumeItemsMod");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureDrinkPondPenalty")) {
			m_temperatureDrinkPondPenalty = ConfigGetFloat("CfgSyberia WorldSystem temperatureDrinkPondPenalty");
		}
		else {
			m_temperatureDrinkPondPenalty = ConfigGetFloat("CfgWorlds " + worldName + " temperatureDrinkPondPenalty");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureDrinkWellPenalty")) {
			m_temperatureDrinkWellPenalty = ConfigGetFloat("CfgSyberia WorldSystem temperatureDrinkWellPenalty");
		}
		else {
			m_temperatureDrinkWellPenalty = ConfigGetFloat("CfgWorlds " + worldName + " temperatureDrinkWellPenalty");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureFillWaterBottleTotal")) {
			m_temperatureFillWaterBottleTotal = ConfigGetFloat("CfgSyberia WorldSystem temperatureFillWaterBottleTotal");
		}
		else {
			m_temperatureFillWaterBottleTotal = ConfigGetFloat("CfgWorlds " + worldName + " temperatureFillWaterBottleTotal");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureFillWaterBottleMod")) {
			m_temperatureFillWaterBottleMod = ConfigGetFloat("CfgSyberia WorldSystem temperatureFillWaterBottleMod");
		}
		else {
			m_temperatureFillWaterBottleMod = ConfigGetFloat("CfgWorlds " + worldName + " temperatureFillWaterBottleMod");
		}

		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureIceDrain")) {
			m_temperatureIceDrain = ConfigGetFloat("CfgSyberia WorldSystem temperatureIceDrain");
		}
		else {
			m_temperatureIceDrain = ConfigGetFloat("CfgWorlds " + worldName + " temperatureIceDrain");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureSnowDrain")) {
			m_temperatureSnowDrain = ConfigGetFloat("CfgSyberia WorldSystem temperatureSnowDrain");
		}
		else {
			m_temperatureSnowDrain = ConfigGetFloat("CfgWorlds " + worldName + " temperatureSnowDrain");
		}

		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem additionalTemperatureResistForSurvivalSkill")) {
			m_additionalTemperatureResistForSurvivalSkill = ConfigGetFloat("CfgSyberia WorldSystem additionalTemperatureResistForSurvivalSkill");
		}
		else {
			m_additionalTemperatureResistForSurvivalSkill = ConfigGetFloat("CfgWorlds " + worldName + " additionalTemperatureResistForSurvivalSkill");
		}
			
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem clothingDrenchedHeatDrain")) {
			m_clothingDrenchedHeatDrain = ConfigGetFloat("CfgSyberia WorldSystem clothingDrenchedHeatDrain");
		}
		else {
			m_clothingDrenchedHeatDrain = ConfigGetFloat("CfgWorlds " + worldName + " clothingDrenchedHeatDrain");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem clothingWetHeatDrain")) {
			m_clothingWetHeatDrain = ConfigGetFloat("CfgSyberia WorldSystem clothingWetHeatDrain");
		}
		else {
			m_clothingWetHeatDrain = ConfigGetFloat("CfgWorlds " + worldName + " clothingWetHeatDrain");
		}
		
		if (GetGame().ConfigIsExisting("CfgSyberia WorldSystem temperatureHeightReduction")) {
			m_temperatureHeightReduction = ConfigGetFloat("CfgSyberia WorldSystem temperatureHeightReduction");
		}
		else {
			m_temperatureHeightReduction = ConfigGetFloat("CfgWorlds " + worldName + " temperatureHeightReduction");
		}
	}
};
