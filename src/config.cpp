class CfgPatches {
	class SyberiaServer {
        units[] = {};
		requiredAddons[] = {"DZ_Data", "DZ_Scripts", "SyberiaScripts"};
	};
};

class CfgMods {
	class SyberiaServer {
		type = "mod";

		class defs {
			class gameScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/3_Game"};
			};

			class worldScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/4_World"};
			};

			class missionScriptModule {
				value = "";
				files[] = {"SyberiaServer/scripts/5_Mission"};
			};

			class imageSets {
				files[] = {};
			};
		};
	};
};

class CfgSyberia
{
    class SleepingSystem
    {
        sleepingDecPerSec = 1;
        sleepingUnconsionEnabled = 0;
        sleepingIncPerUnconsionBoostValue = 5;
        sleepingIncPerUnconsionBoostTime = 30;
        sleepingIncPerSleepingLvl1Sec = 10;
        sleepingIncPerSleepingLvl2Sec = 20;
        sleepingHealPerSec01 = 0.002;
        sleepingHealInfluenzaChance = 0.01;
    };
    
    class MedicineSystem
    {
        sepsisStage1TimeSec = 600;
        sepsisStage2TimeSec = 2400;
        sepsisDeathTimeSec = 1200;
        sepsisHighTemperatureValue = 42;
        sepsisZombieHitChance = 0.01;
        sepsisKnifeHitChance = 0.1;
        sepsisBulletHitChance = 0.05;
        sepsisDirtySyringeChance = 0.75;
        zvirusStage1TimeSec = 600; // 20 min
        zvirusStage2TimeSec = 2400; // zvirusStage1TimeSec + 40 min
        zvirusDeathTimeSec = 1200;
        zvirusBloodRegenPerSec = 4;
        zvirusZombieHitChance = 0.2;
        zvirusZombieBloodTransferChance = 0.1;
        zvirusAntidotBloodDmg01 = 0.15;
        zvirusAntidotHealthDmg01 = 0.2;
        painLvl1TimeSec = 300;
        painLvl2TimeSec = 900;
        painLvl3TimeSec = 1800;
        painMaxDurationSec = 2400;
        bleedingBullethitHealthlosePerSecond = 0.003;
        bleedingBullethitOpenPerSecond = 5;
        bleedingBullethitCupd1PerSecond = 0.3;
        bleedingBullethitCupd2PerSecond = 0.1;
        bleedingBullethitRemoveBandagedTimeSec = -1; // DISABLED
        bleedingKnifehitHealthlosePerSecond = 0.003;
        bleedingKnifehitOpenPerSecond = 6;
        bleedingKnifehitCupd1PerSecond = 0.35;
        bleedingKnifehitCupd2PerSecond = 0.1;
        bleedingKnifehitRemoveBandagedTimeSec = 3600;
        bleedingVisceraBloodlosePerSecond = 3;
        bleedingVisceraHealthlosePerSecond = 0.005;
        bleedingHematomaBloodlosePerSecond = 0;
        bleedingHematomaHealthlosePerSecond = 0.002;
        bleedingZombieHitChance = 0.3;
        hematomaPlayerhandsHitChance = 0.6;
        hematomaZombieHitChance = 0.7;
        hematomaRegenTimerSec = 600;
        hematomaRegenTimeBoostOnSalve = 8;
        hematomaPainChance = 0.5;
        cuthitRegenTimerSec = 300;
        visceraKnifehitTorsoChance = 0.4;
        visceraBullethitTorsoChance = 0.3;
        concussionRegenTimeSec = 1800; //30 min
        hemostaticEffectModifier = 0.4; // 40% of total bloodlose
        hematopoiesisEffectBloodPerSec = 5;
        adrenalinEffectShockUpPerSec[] = { 0.01666, 0.03333, 0.2 };
        adrenalinEffectStaminaDepletionMod[] = { 0.5, 0.25, 0.1 };
        overdoseDecrementPerSec = 0.001;
        overdoseUnconChangePerSec = 0.025;
        mindstateHealPerSec = 0.04;
        mindstateEmptyDeadtimeSec = 600;
        influenzaIncubatePeriodsSec[] = { 300, 1500, 1800 };
        influenzaTemperatureLevels[] = { -1.0, 37.0, 40.0 };
        influenzaSympthomChance[] = { 0.02, 0.04, 0.08 };
        influenzaDeathTime[] = {0, 0, 900};
        influenzaApplyOnColdWarnChance = 0.0005;
        influenzaApplyOnColdCritChance = 0.002;
        antibioticsGlobalEffectivityModifier = 0.006;
        stomatchpoisonVomitChance[] = { 0.008333, 0.016666, 0.033333 };
        stomatchpoisonWaterDrainFromVomit[] = { 100, 300, 500 };
        stomatchpoisonEnergyDrainFromVomit[] = { 150, 400, 1000 };
        stomatchhealModifier = 10;
        stomatchpoisonRawMeat[] = { 1, 300 }; // level, seconds * amount
        stomatchpoisonBurnedMeat[] = { 1, 300 }; // level, seconds * amount
        stomatchpoisonRottenMeat[] = { 2, 300 }; // level, seconds * amount
        stomatchpoisonBurnedFood[] = { 1, 300 }; // level, seconds * amount
        stomatchpoisonRottenFood[] = { 2, 300 }; // level, seconds * amount
        stomatchpoisonDirtyHands[] = { 1, 300 }; // level, seconds * amount
        stomatchpoisonInfection[] = { 1, 300 }; // level, seconds * amount
        stomatchpoisonChanceDrinkPond = 0.8;
        stomatchpoisonDrinkPond[] = { 1, 120 };
        stomatchpoisonChanceDrinkWell = 0.01;
        stomatchpoisonDrinkWell[] = { 1, 60 };
        stomatchpoisonDefaultTimes[] = { 1800, 900, 600 }; // lvl1, lvl2, lvl3
        glovesDamageOnApplyBandage = -5;
        glovesDamageOnSurgery = -15;
        startSurgeryVisceraChance = 0.1;
        startSurgeryBulletChance = 0.2;
        startSurgeryKnifeChance = 0.3;
        surgerySideEffectBloodLoseCount = 500;
        surgerySideEffectHealthLoseCount = 20;
        radiationDoseDecrementPerSec[] = { -0.02, -0.08, -0.2, -0.5 };
        radiationLevels[] = { 20, 60, 120 };
        radiationHealthDamage[] = { 0.09, 0.2, 0.8 };
        radiationSleepingDec = 3;
        shockDecrementOnLowHealthAndBlood = 1.5;
    };

    class AttackSystem
    {
        damageNonWeaponItemInHandsOnAttack = 3.0;
        damageGlovesOnHandsOnAttack = 2.0;
        damageHandsOnAttackChance = 0.20;
        bodyGuardModifier = 1;
    };

    class ZombieSystem
    {
        zombieOpenDoorEnable = 1;
        zombieOpenDoorDistance = 1.8;
        zombieFindHouseDistance = 20.0;
    };
    
    class AnimalsSystem
    {
        animalsButchingKnifeDamage = 0.4;
    };

    class IngameSystem
    {
        bookPagesCountToCut = 10;
        gasMaskFilterDegradationInToxicZone = -0.33;
        gasMaskFilterDegradationInRadZone = -0.2;
        gasMaskFilterDegradationDefault = -0.05;
    };

    class SkillsExperienceSystem
    {
        skillsExpImmunityPain = 50;
        skillsExpImmunityHematoma = 10;
        skillsExpImmunityInfluenza = 200;
        skillsExpImmunityStomatch = 50;
        skillsExpImmunityZVirus = 800;
        skillsExpImmunitySepsis = 200;
        skillsExpImmunityOverdose = 50;
        
        skillsExpAthleticsSprintTime = 60;
        skillsExpAthleticsSprintIncrement = 80;
        skillsExpAthleticsJogTime = 60;
        skillsExpAthleticsJogIncrement = 15;
        
        skillsExpStrengthLightAttackValue = 30;
        skillsExpStrengthLightAttackChance = 0.3;
        skillsExpStrengthHeavyAttackValue = 50;
        skillsExpStrengthHeavyAttackChance = 0.5;
        skillsExpStrengthMineWoodOrStone = 5;
        
        skillsExpMedicineBandage = 50;
        skillsExpMedicineTablets = 20;
        skillsExpMedicineAmpoule = 100;
        skillsExpMedicineInjector = 100;
        skillsExpMedicineSurgery = 1000;
        
        skillsExpHuntingButchSmall = 100;
        skillsExpHuntingButchCommon = 250;
        skillsExpHuntingButchWolf = 500;
        skillsExpHuntingButchBear = 3000;
    };
};