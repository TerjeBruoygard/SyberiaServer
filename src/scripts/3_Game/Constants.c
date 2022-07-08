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

// ADV MED
const float SEPSIS_STAGE1_TIME_SEC = 60 * 10;
const float SEPSIS_STAGE2_TIME_SEC = 60 * 40;
const float SEPSIS_DEATH_TIME_SEC = 60 * 15;
const float SEPSIS_HIGH_TEMPERATURE_VALUE = 42;
const float SEPSIS_ZOMBIE_HIT_CHANCE = 0.04;
const float SEPSIS_KNIFE_HIT_CHANCE = 0.04;
const float SEPSIS_BULLET_HIT_CHANCE = 0.05;
const float SEPSIS_DIRTY_SYRINGE_CHANCE = 0.6;
const float SEPSIS_DITRY_HANDS_BANDAGE_CHANCE = 0.2;
const float SEPSIS_DITRY_HANDS_SYRGERY_CHANCE = 0.8;

const float ZVIRUS_STAGE1_TIME_SEC = 20 * 60;
const float ZVIRUS_STAGE2_TIME_SEC = ZVIRUS_STAGE1_TIME_SEC + (40 * 60);
const float ZVIRUS_DEATH_TIME_SEC = 60 * 15;
const float ZVIRUS_BLOOD_REGEN_PER_SEC = 4;
const float ZVIRUS_ZOMBIE_HIT_CHANCE = 0.25;
const float ZVIRUS_ANTIDOT_BLOOD_DMG_01 = 0.15;
const float ZVIRUS_ANTIDOT_HEALTH_DMG_01 = 0.2;

const float PAIN_LVL1_TIME_SEC = 60 * 5;
const float PAIN_LVL2_TIME_SEC = 60 * 30;
const float PAIN_LVL3_TIME_SEC = 60 * 60;
const float PAIN_MAX_DURATION_SEC = 60 * 60 * 4;

const float BLEEDING_BULLETHIT_HEALTHLOSE_PER_SECOND = 0.003;
const float BLEEDING_BULLETHIT_OPEN_PER_SECOND = 5;
const float BLEEDING_BULLETHIT_CUPD1_PER_SECOND = 0.3;
const float BLEEDING_BULLETHIT_CUPD2_PER_SECOND = 0.1;
const float BLEEDING_BULLETHIT_REMOVE_BANDAGED_TIME_SEC = -1; // DISABLED

const float BLEEDING_KNIFEHIT_HEALTHLOSE_PER_SECOND = 0.003;
const float BLEEDING_KNIFEHIT_OPEN_PER_SECOND = 6;
const float BLEEDING_KNIFEHIT_CUPD1_PER_SECOND = 0.35;
const float BLEEDING_KNIFEHIT_CUPD2_PER_SECOND = 0.1;
const float BLEEDING_KNIFEHIT_REMOVE_BANDAGED_TIME_SEC = 60 * 20; // 30 min

const float BLEEDING_VISCERA_BLOODLOSE_PER_SECOND = 3;
const float BLEEDING_VISCERA_HEALTHLOSE_PER_SECOND = 0.005;

const float BLEEDING_HEMATOMA_BLOODLOSE_PER_SECOND = 0;
const float BLEEDING_HEMATOMA_HEALTHLOSE_PER_SECOND = 0.002;

const float BLEEDING_ZOMBIE_HIT_CHANCE = 0.3;
const float HEMATOMA_PLAYERHANDS_HIT_CHANCE = 0.4;
const float HEMATOMA_ZOMBIE_HIT_CHANCE = 0.7;
const float HEMATOMA_REGEN_TIMER_SEC = 60 * 10;
const float HEMATOMA_REGEN_TIME_BOOST_ON_SALVE = 5;
const float CUTHIT_REGEN_TIMER_SEC = 60 * 5;
const float VISCERA_KNIFEHIT_TORSO_CHANCE = 0.4;
const float VISCERA_BULLETHIT_TORSO_CHANCE = 0.3;

//const float HEMOLOGIC_SHOCK_DIETIME_SEC = 60; 

const float CONCUSSION_REGEN_TIME_SEC = 60 * 30; //30 min

const float HEMOSTATIC_EFFECT_MODIFIER = 0.4; // 40% of total bloodlose
const float HEMATOPOIESIS_EFFECT_BLOOD_PER_SEC = 5;

const float ADRENALIN_EFFECT_SHOCK_01_LVL1_PER_SEC = 1 / 60;
const float ADRENALIN_EFFECT_SHOCK_01_LVL2_PER_SEC = 1 / 30;
const float ADRENALIN_EFFECT_SHOCK_01_LVL3_PER_SEC = 1 / 5;

const float OVERDOSE_DECREMENT_PER_SEC = 0.001;
const float OVERDOSE_UNCON_CHANGE_PER_SEC = 0.025;

const bool ZOMBIE_OPEN_DOOR_ENABLE = true;
const float ZOMBIE_OPEN_DOOR_DISTANCE = 1.8;
const float ZOMBIE_FIND_HOUSE_DISTANCE = 20.0;

const float MINDSTATE_HEAL_PER_SEC = 0.04;
const int   MINDSTATE_EMPTY_DEADTIME_SEC = 10 * 60;

const float INFLUENZA_INCUBATE_PERIODS_SEC[3] = { 60 * 5, 60 * 25, 60 * 30 };
const float INFLUENZA_TEMPERATURE_LEVELS[3] = { -1, 37.0, 40.0 };
const float INFLUENZA_SYMPTHOM_CHANCE[3] = { 0.02, 0.04, 0.08 };
const float INFLUENZA_DEATH_TIME[3] = {0, 0, 60 * 15};
const float INFLUENZA_APPLY_ON_COLD_WARN_CHANCE = 0.0003;
const float INFLUENZA_APPLY_ON_COLD_CRIT_CHANCE = 0.0006;
const float ANTIBIOTICS_GLOBAL_EFFECTIVITY_MODIFIER = 0.006;

const float STOMATCHPOISON_VOMIT_CHANCE[3] = { 1 / 120, 1 / 60, 1 / 30 };
const int STOMATCHPOISON_WATER_DRAIN_FROM_VOMIT[3] = { 100, 300, 500 };
const int STOMATCHPOISON_ENERGY_DRAIN_FROM_VOMIT[3] = { 150, 400, 1000 };
const float STOMATCHHEAL_MODIFIER = 10;

const int STOMATCHPOISON_RAW_MEAT[2] = { 1, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_BURNED_MEAT[2] = { 1, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_ROTTEN_MEAT[2] = { 2, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_BURNED_FOOD[2] = { 1, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_ROTTEN_FOOD[2] = { 2, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_DIRTY_HANDS[2] = { 1, 5 * 60 }; // level, seconds * amount
const int STOMATCHPOISON_INFECTION[2] = { 1, 5 * 60 }; // level, seconds * amount
const float STOMATCHPOISON_CHANCE_DRINK_POND = 0.8;
const int STOMATCHPOISON_DRINK_POND[2] = { 1, 2 * 60 };
const float STOMATCHPOISON_CHANCE_DRINK_WELL = 0.01;
const int STOMATCHPOISON_DRINK_WELL[2] = { 1, 60 };

const int STOMATCHPOISON_DEFAULT_TIMES[3] = { 30 * 60, 15 * 60, 10 * 60 }; // lvl1, lvl2, lvl3

// Temperature
const float TEMPERATURE_HIGH_CONSUME_ITEMS_MOD = 0.04;
const float TEMPERATURE_LOW_CONSUME_ITEMS_MOD = -0.2;
const float TEMPERATURE_DRINK_POND_PENALTY = -0.2;
const float TEMPERATURE_DRINK_WELL_PENALTY = -0.2;
const float TEMPERATURE_FILL_WATER_BOTTLE_TOTAL = 6;
const float TEMPERATURE_FILL_WATER_BOTTLE_MOD = 0.1;

// Others
const int BOOK_PAGES_COUNT_TO_CUT = 10;
