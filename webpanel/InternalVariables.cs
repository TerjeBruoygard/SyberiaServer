using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SyberiaWebPanel
{
    public class InternalVariables
    {
        private string pboManagerDir;
        private string pboManagerExecutable;
        private string internalVariablesFolder;
        private string internalVariablesJson;
        private string overridedVariablesJson;
        
        public CfgSyberiaType Default { private set; get; }
        public CfgSyberiaType CfgSyberia { private set; get; }
        public Dictionary<string, bool> OverridedVariables { private set; get; }
        public Dictionary<string, string> VariableTypes { private set; get; }

        public InternalVariables()
        {
            Default = new CfgSyberiaType();
            pboManagerDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "PboManager");
            pboManagerExecutable = Path.Combine(pboManagerDir, "PBOConsole.exe");
            internalVariablesFolder = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "PboManager", "SyberiaInternalVariables");
            if (!Directory.Exists(internalVariablesFolder))
            {
                Directory.CreateDirectory(internalVariablesFolder);
            }

            internalVariablesJson = Path.Combine(internalVariablesFolder, "variables.json");
            if (File.Exists(internalVariablesJson))
            {
                CfgSyberia = Newtonsoft.Json.JsonConvert.DeserializeObject<CfgSyberiaType>(File.ReadAllText(internalVariablesJson));
            }
            else
            {
                CfgSyberia = new CfgSyberiaType();
            }

            overridedVariablesJson = Path.Combine(internalVariablesFolder, "overrides.json");
            if (File.Exists(overridedVariablesJson))
            {
                OverridedVariables = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, bool>>(File.ReadAllText(overridedVariablesJson));
            }
            else
            {
                OverridedVariables = new Dictionary<string, bool>();;
            }

            VariableTypes = new Dictionary<string, string>();
            foreach (var propSection in CfgSyberia.GetType().GetProperties())
            {
                var sectionName = propSection.Name;
                var sectionValue = propSection.GetValue(CfgSyberia);
                var sectionDefault = propSection.GetValue(Default);
                foreach (var propVariable in propSection.PropertyType.GetFields())
                {
                    var varId = $"{propSection.Name}.{propVariable.Name}";
                    VariableTypes.Add(varId, propVariable.FieldType.Name);

                    if (!OverridedVariables.ContainsKey(varId) || !OverridedVariables[varId])
                    {
                        propVariable.SetValue(sectionValue, propVariable.GetValue(sectionDefault));
                    }
                }
            }
        }

        public void Apply(JToken json)
        {
            foreach (var key in OverridedVariables.Keys.ToArray())
            {
                if (!json.Any(x => (x as JProperty).Name.Equals(key)))
                {
                    OverridedVariables.Remove(key);
                }
            }

            foreach (JProperty prop in json)
            {
                if (OverridedVariables.ContainsKey(prop.Name))
                {
                    OverridedVariables[prop.Name] = true;
                }
                else
                {
                    OverridedVariables.Add(prop.Name, true);
                }

                var parts = prop.Name.Split('.');
                var sectionName = parts[0];
                var fieldName = parts[1];
                var sectionRef = CfgSyberia.GetType().GetProperty(sectionName);
                var sectionValue = sectionRef.GetValue(CfgSyberia);
                var fieldRef = sectionRef.PropertyType.GetField(fieldName);
                if (fieldRef.FieldType == typeof(int))
                {
                    fieldRef.SetValue(sectionValue, prop.Value.Value<int>());
                }
                else if (fieldRef.FieldType == typeof(float))
                {
                    fieldRef.SetValue(sectionValue, prop.Value.Value<float>());
                }
                else if (fieldRef.FieldType == typeof(bool))
                {
                    fieldRef.SetValue(sectionValue, prop.Value.Value<bool>());
                }
                else if (fieldRef.FieldType == typeof(int[]))
                {
                    var array = (int[])fieldRef.GetValue(sectionValue);
                    var jarray = (JArray)prop.Value;
                    if (array.Length == jarray.Count)
                    {
                        for (int i = 0; i < array.Length; i++)
                        {
                            array[i] = jarray[i].Value<int>();
                        }
                    }
                }
                else if (fieldRef.FieldType == typeof(float[]))
                {
                    var array = (float[])fieldRef.GetValue(sectionValue);
                    var jarray = (JArray)prop.Value;
                    if (array.Length == jarray.Count)
                    {
                        for (int i = 0; i < array.Length; i++)
                        {
                            array[i] = jarray[i].Value<float>();
                        }
                    }
                }
            }
        }

        public void Save()
        {
            var overridesToDelete = new List<string>();
            foreach (var pair in OverridedVariables)
            {
                if (!pair.Value)
                {
                    overridesToDelete.Add(pair.Key);
                }
            }
            foreach (var key in overridesToDelete)
            {
                OverridedVariables.Remove(key);
            }

            File.WriteAllText(internalVariablesJson, Newtonsoft.Json.JsonConvert.SerializeObject(CfgSyberia));
            File.WriteAllText(overridedVariablesJson, Newtonsoft.Json.JsonConvert.SerializeObject(OverridedVariables));
        }

        public void BuildPbo()
        {
            BuildConfigCpp();

            var modFolder = Path.GetFullPath(Path.Combine(pboManagerDir, "SyberiaInternalVariables"));
            var pboDest = Path.GetFullPath(Path.Combine(pboManagerDir, "SyberiaInternalVariables.pbo"));
            var exec = new Process();
            exec.StartInfo.UseShellExecute = false;
            exec.StartInfo.RedirectStandardOutput = true;
            exec.StartInfo.FileName = pboManagerExecutable;
            exec.StartInfo.WorkingDirectory = pboManagerDir;
            exec.StartInfo.Arguments = $"-pack \"{modFolder}\" \"{pboDest}\"";
            exec.Start();
            exec.WaitForExit();
            if (exec.ExitCode != 0)
            {
                throw new ApplicationException("Failed to build PBO");
            }
        }

        public void BuildConfigCpp()
        {
            var builder = new StringBuilder();
            builder.AppendLine("class CfgPatches { class SyberiaInternalVariables { units[] = {}; requiredAddons[] = {\"SyberiaServer\"}; }; };");
            builder.AppendLine();
            builder.AppendLine("class CfgSyberia {");
            foreach (var propSection in CfgSyberia.GetType().GetProperties())
            {
                var sectionName = propSection.Name;
                var sectionValue = propSection.GetValue(CfgSyberia);

                builder.AppendLine($"    class {sectionName} {{");
                foreach (var fieldRef in propSection.PropertyType.GetFields())
                {
                    var varId = $"{propSection.Name}.{fieldRef.Name}";
                    if (OverridedVariables.ContainsKey(varId) && OverridedVariables[varId])
                    {
                        if (fieldRef.FieldType == typeof(int))
                        {
                            builder.AppendLine($"       {fieldRef.Name} = {fieldRef.GetValue(sectionValue)};");
                        }
                        else if (fieldRef.FieldType == typeof(float))
                        {
                            builder.AppendLine($"       {fieldRef.Name} = {fieldRef.GetValue(sectionValue).ToString().Replace(',', '.')};");
                        }
                        else if (fieldRef.FieldType == typeof(bool))
                        {
                            var value = ((bool)fieldRef.GetValue(sectionValue)) ? 1 : 0;
                            builder.AppendLine($"       {fieldRef.Name} = {value};");
                        }
                        else if (fieldRef.FieldType == typeof(int[]))
                        {
                            var array = (int[])fieldRef.GetValue(sectionValue);
                            builder.AppendLine($"       {fieldRef.Name}[] = {{ {string.Join(", ", array)} }};");
                        }
                        else if (fieldRef.FieldType == typeof(float[]))
                        {
                            var array = (float[])fieldRef.GetValue(sectionValue);
                            builder.AppendLine($"       {fieldRef.Name}[] = {{ {string.Join(", ", array.Select(x => x.ToString().Replace(',', '.')))} }};");
                        }
                    }
                }
                builder.AppendLine("    };");
            }
            builder.AppendLine("};");

            var configPath = Path.Combine(internalVariablesFolder, "config.cpp");
            File.WriteAllText(configPath, builder.ToString());
        }

        public class CfgSyberiaType
        {
            public SleepingSystemType SleepingSystem { set; get; } = new SleepingSystemType();
            public MedicineSystemType MedicineSystem { set; get; } = new MedicineSystemType();
            public AttackSystemType AttackSystem { set; get; } = new AttackSystemType();
            public ZombieSystemType ZombieSystem { set; get; } = new ZombieSystemType();
            public AnimalsSystemType AnimalsSystem { set; get; } = new AnimalsSystemType();
            public IngameSystemType IngameSystem { set; get; } = new IngameSystemType();
            public SkillsExperienceSystemType SkillsExperienceSystem { set; get; } = new SkillsExperienceSystemType();
            public WorldSystemType WorldSystem { set; get; } = new WorldSystemType();

            public class SleepingSystemType
            {
                public int sleepingDecPerSec = 1;
                public bool sleepingUnconsionEnabled = false;
                public int sleepingIncPerUnconsionBoostValue = 5;
                public int sleepingIncPerUnconsionBoostTime = 30;
                public int sleepingIncPerSleepingLvl1Sec = 10;
                public int sleepingIncPerSleepingLvl2Sec = 20;
                public float sleepingHealPerSec01 = 0.002f;
                public float sleepingHealInfluenzaChance = 0.01f;
            };

            public class MedicineSystemType
            {
                public int sepsisStage1TimeSec = 600;
                public int sepsisStage2TimeSec = 2400;
                public int sepsisDeathTimeSec = 1200;
                public float sepsisHighTemperatureValue = 42;
                public float sepsisZombieHitChance = 0.01f;
                public float sepsisKnifeHitChance = 0.1f;
                public float sepsisBulletHitChance = 0.05f;
                public float sepsisDirtySyringeChance = 0.75f;
                public int zvirusStage1TimeSec = 600; // 10 min
                public int zvirusStage2TimeSec = 2400; // zvirusStage1TimeSec + 40 min
                public int zvirusDeathTimeSec = 1200;
                public float zvirusBloodLosePerSec = 0.05f;
                public float zvirusHealthLosePerSec = 0.01f;
                public float zvirusCuthitSpawnChance = 0.003f;
                public float zvirusPainSpawnChance = 0.001f;
                public float zvirusVommitSpawnChance = 0.0002f;
                public float zvirusBlindnessSpawnChance = 0.01f;
                public float zvirusFeverblurSpawnChance = 0.01f;
                public int zvirusOnHitIncrementTimer = 300;
                public float zvirusZombieHitChance = 0.2f;
                public float zvirusZombieBloodTransferChance = 0.1f;
                public float zvirusAntidotBloodDmg01 = 0.15f;
                public float zvirusAntidotHealthDmg01 = 0.2f;
                public int painLvl1TimeSec = 300;
                public int painLvl2TimeSec = 900;
                public int painLvl3TimeSec = 1800;
                public int painMaxDurationSec = 2400;
                public float bleedingBullethitHealthlosePerSecond = 0.003f;
                public float bleedingBullethitOpenPerSecond = 5;
                public float bleedingBullethitCupd1PerSecond = 0.3f;
                public float bleedingBullethitCupd2PerSecond = 0.1f;
                public float bleedingBullethitRemoveBandagedTimeSec = -1; // DISABLED
                public float bleedingKnifehitZombieChance = 0.005f;
                public float bleedingKnifehitHealthlosePerSecond = 0.003f;
                public float bleedingKnifehitOpenPerSecond = 6;
                public float bleedingKnifehitCupd1PerSecond = 0.35f;
                public float bleedingKnifehitCupd2PerSecond = 0.1f;
                public float bleedingKnifehitRemoveBandagedTimeSec = 3600;
                public float bleedingVisceraBloodlosePerSecond = 3;
                public float bleedingVisceraHealthlosePerSecond = 0.005f;
                public float bleedingHematomaBloodlosePerSecond = 0;
                public float bleedingHematomaHealthlosePerSecond = 0.002f;
                public float bleedingZombieHitChance = 0.3f;
                public float hematomaPlayerhandsHitChance = 0.6f;
                public float hematomaZombieHitChance = 0.7f;
                public int hematomaRegenTimerSec = 600;
                public int hematomaRegenTimeBoostOnSalve = 8;
                public float hematomaPainChance = 0.5f;
                public int cuthitRegenTimerSec = 300;
                public float visceraKnifehitTorsoChance = 0.4f;
                public float visceraBullethitTorsoChance = 0.3f;
                public int concussionRegenTimeSec = 1800; //30 min
                public float concussionZombieHitChance = 0.05f;
                public float hemostaticEffectModifier = 0.3f; // 30% of total bloodlose
                public int hematopoiesisEffectBloodPerSec = 2;
                public float[] adrenalinEffectShockUpPerSec = new float[] { 0.01666f, 0.03333f, 0.2f };
                public float[] adrenalinEffectStaminaDepletionMod = new float[] { 0.5f, 0.25f, 0.1f };
                public float[] adrenalinEffectStaminaRecoveryMod = new float[] { 1.5f, 2.0f, 2.5f };
                public float overdoseDecrementPerSec = 0.001f;
                public float overdoseUnconChangePerSec = 0.025f;
                public float mindstateHealPerSec = 0.005f;
                public int mindstateEmptyDeadtimeSec = 600;
                public int[] influenzaIncubatePeriodsSec = new int[] { 300, 1500, 1800 };
                public float[] influenzaTemperatureLevels = new float[] { -1.0f, 37.0f, 40.0f };
                public float[] influenzaSympthomChance = new float[] { 0.02f, 0.04f, 0.08f };
                public int[] influenzaDeathTime = new int[] {0, 0, 900};
                public float influenzaApplyOnColdWarnChance = 0.001f;
                public float influenzaApplyOnColdCritChance = 0.005f;
                public float antibioticsGlobalEffectivityModifier = 0.006f;
                public float[] stomatchpoisonVomitChance = new float[] { 0.01f, 0.02f, 0.04f };
                public int[] stomatchpoisonWaterDrainFromVomit = new int[] { 200, 350, 500 };
                public int[] stomatchpoisonEnergyDrainFromVomit = new int[] { 300, 600, 1000 };
                public int stomatchhealModifier = 5;
                public int[] stomatchpoisonRawMeat = new int[] { 1, 400 }; // level, seconds * amount
                public int[] stomatchpoisonBurnedMeat = new int[] { 1, 500 }; // level, seconds * amount
                public int[] stomatchpoisonRottenMeat = new int[] { 2, 600 }; // level, seconds * amount
                public int[] stomatchpoisonBurnedFood = new int[] { 1, 300 }; // level, seconds * amount
                public int[] stomatchpoisonRottenFood = new int[] { 2, 600 }; // level, seconds * amount
                public int[] stomatchpoisonDirtyHands = new int[] { 1, 300 }; // level, seconds * amount
                public int[] stomatchpoisonInfection = new int[] { 1, 500 }; // level, seconds * amount
                public int[] stomatchpoisonGasoline = new int[] { 2, 900 }; // level, seconds * amount
                public float stomatchpoisonChanceDrinkPond = 0.5f;
                public int[] stomatchpoisonDrinkPond = new int[] { 2, 200 };
                public float stomatchpoisonChanceDrinkWell = 0.01f;
                public int[] stomatchpoisonDrinkWell = new int[] { 1, 100 };
                public int[] stomatchpoisonDefaultTimes = new int[] { 1800, 900, 600 }; // lvl1, lvl2, lvl3
                public float startSurgeryVisceraChance = 0.15f;
                public float startSurgeryBulletChance = 0.3f;
                public float startSurgeryKnifeChance = 0.5f;
                public int surgerySideEffectBloodLoseCount = 500;
                public int surgerySideEffectHealthLoseCount = 20;
                public float[] radiationDoseDecrementPerSec = new float[] { -0.02f, -0.08f, -0.2f, -0.5f };
                public int[] radiationLevels = new int[] { 20, 60, 120 };
                public float[] radiationHealthDamage = new float[] { 0.09f, 0.2f, 0.8f };
                public int radiationSleepingDec = 3;
                public float shockDecrementOnLowHealthAndBlood = 1.5f;
                public float[] zombieHitDecreaseMind = new float[] { 0.4f, 5 }; // amount, time
                public float[] zombieKillDecreaseMind = new float[] { 0.2f, 10 }; // amount, time
                public float[] antidepresantMindInc = new float[] { 0.1f, 0.25f, 0.5f };
                public bool disableLaughterSymptom = false;
            };

            public class AttackSystemType
            {
                public float damageNonWeaponItemInHandsOnAttack = 3.0f;
                public float damageGlovesOnHandsOnAttack = 2.0f;
                public float damageHandsOnAttackChance = 0.20f;
                public float bodyGuardModifier = 1;
                public float zombieAttackDistanceChase = 2.2f;
                public float zombieAttackDistanceFight = 1.8f;
            };

            public class ZombieSystemType
            {
                public bool zombieOpenDoorEnable = true;
                public float zombieOpenDoorDistance = 1.8f;
                public float zombieFindHouseDistance = 20.0f;
                public float zombieBloodHandsChanceOnLooting = 0.25f;
                public bool zombieAttackPlayersInUnconscious = true;
                public float zombieDetectPlayerVisibilityMultiplier = 1.0f;
                public float zombieDetectPlayerNoiseMultiplier = 1.0f;
            };

            public class AnimalsSystemType
            {
                public float animalsButchingKnifeDamage = 100;
            };

            public class IngameSystemType
            {
                public int bookPagesCountToCut = 10;
                public float gasMaskFilterDegradationInToxicZone = -0.33f;
                public float gasMaskFilterDegradationInRadZone = -0.2f;
                public float gasMaskFilterDegradationDefault = -0.05f;
                public float igniteFireplaceBaseSkillcheckChance = 0.2f;
                public float igniteFireplaceIndoorAdditionalChance = 0.3f;
                public float damageClothingInRadiationZone = 0.0f;
                public float defaultStaminaDepletionMultiplier = 1.0f;
                public float defaultStaminaRecoveryMultiplier = 1.0f;
                public float dirtyFruitsSpawnChance = 0.25f;
                public float dirtyMushroomsSpawnChance = 0.6f;
            };

            public class SkillsExperienceSystemType
            {
                public int skillsExpImmunityPain = 50;
                public int skillsExpImmunityHematoma = 10;
                public int skillsExpImmunityInfluenza = 200;
                public int skillsExpImmunityStomatch = 50;
                public int skillsExpImmunityZVirus = 800;
                public int skillsExpImmunitySepsis = 200;
                public int skillsExpImmunityOverdose = 50;

                public int skillsExpAthleticsSprintTime = 60;
                public int skillsExpAthleticsSprintIncrement = 80;
                public int skillsExpAthleticsJogTime = 60;
                public int skillsExpAthleticsJogIncrement = 15;

                public int skillsExpStrengthLightAttackValue = 30;
                public float skillsExpStrengthLightAttackChance = 0.3f;
                public int skillsExpStrengthHeavyAttackValue = 50;
                public float skillsExpStrengthHeavyAttackChance = 0.5f;
                public int skillsExpStrengthMineWoodOrStone = 5;

                public int skillsExpMedicineBandage = 50;
                public int skillsExpMedicineTablets = 20;
                public int skillsExpMedicineAmpoule = 100;
                public int skillsExpMedicineInjector = 100;
                public int skillsExpMedicineSurgery = 1000;

                public int skillsExpStealthSilentAttackValue = 100;
                public float skillsExpStealthSilentAttackChance = 0.5f;
                public int skillsExpStealthCrouchTime = 60;
                public int skillsExpStealthCrouchIncrement = 60;

                public int skillsExpHuntingButchSmall = 100;
                public int skillsExpHuntingButchCommon = 250;
                public int skillsExpHuntingButchWolf = 500;
                public int skillsExpHuntingButchBear = 3000;

                public int skillsExpSurvivalIgniteFireSuccess = 500;
                public int skillsExpSurvivalIgniteFireFailed = 50;
            };

            public class WorldSystemType
            {
                public bool winterWorld = false;
                public float airDayTemperatureMin=10;
                public float airDayTemperatureMax = 15;
                public float airNightTemperatureMin = 8;
                public float airNightTemperatureMax = 12;
                public float clothingHeatIsolationGlobal = 0.8f;

                public float temperatureHighConsumeItemsMod = 0.02f;
                public float temperatureLowConsumeItemsMod = -0.01f;
                public float temperatureDrinkPondPenalty = -0.005f;
                public float temperatureDrinkWellPenalty = -0.01f;
                public float temperatureFillWaterBottleTotal = 15;
                public float temperatureFillWaterBottleMod = 0.1f;
                public float temperatureIceDrain = 0;
                public float temperatureSnowDrain = 0;
                public float temperatureHeightReduction = 0.002f;

                public float clothingDrenchedHeatDrain = -5;
                public float clothingWetHeatDrain = -2;
                public float additionalTemperatureResistForSurvivalSkill = 15;
            };
        };
    }
}
