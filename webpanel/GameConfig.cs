using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace SyberiaWebPanel
{
    public class GameConfig
    {
        public string m_configDir { private set; get; }

        public MainConfig m_mainConfig { private set; get; }

        public ClientConfig m_clientConfig { private set; get; }

        public AdminToolOptions m_adminToolOptions { private set; get; }

        [ScriptObjectSerializableAttribute]
        public GroupDefault m_groupDefault { private set; get; }

        [ScriptObjectSerializableAttribute]
        public List<CustomLoadout> m_customLoadouts { private set; get; }

        [ScriptObjectSerializableAttribute]
        public List<GroupFaction> m_groupFactions { private set; get; }

        public PdaConfig m_pdaConfig { private set; get; }

        [ScriptObjectSerializableAttribute]
        public TradingConfig m_tradingConfig { private set; get; }

        [ScriptObjectSerializableAttribute]
        public ZonesConfig m_zonesConfig { private set; get; }

        [ScriptObjectSerializableAttribute]
        public CraftingConfig m_craftingConfig {
            get
            {
                if (m_crafting_instance != null)
                {
                    return m_crafting_instance;
                }

                var craftingConfigPath = Path.Combine(m_configDir, "CraftingSettings.json");
                if (File.Exists(craftingConfigPath))
                {
                    m_crafting_instance = JsonConvert.DeserializeObject<CraftingConfig>(File.ReadAllText(craftingConfigPath));
                    return m_crafting_instance;
                }
                
                throw new ApplicationException("Crafting settings not found.");
            }
        }
        private CraftingConfig m_crafting_instance = null;

        public GameConfig(string gameDir)
        {
            m_configDir = Path.Combine(gameDir, "profiles", "Syberia");
            if (!Directory.Exists(m_configDir))
            {
                Directory.CreateDirectory(m_configDir);
            }

            var adminToolOptionsPath = Path.Combine(m_configDir, "AdminToolOptions.json");
            if (File.Exists(adminToolOptionsPath)) m_adminToolOptions = JsonConvert.DeserializeObject<AdminToolOptions>(File.ReadAllText(adminToolOptionsPath));
            else m_adminToolOptions = new AdminToolOptions().InitializeDefault();

            var clientConfigPath = Path.Combine(m_configDir, "ClientConfig.json");
            if (File.Exists(clientConfigPath)) m_clientConfig = JsonConvert.DeserializeObject<ClientConfig>(File.ReadAllText(clientConfigPath));
            else m_clientConfig = new ClientConfig().InitializeDefault();

            var customLoadoutsPath = Path.Combine(m_configDir, "CustomLoadouts.json");
            if (File.Exists(customLoadoutsPath)) m_customLoadouts = JsonConvert.DeserializeObject<List<CustomLoadout>>(File.ReadAllText(customLoadoutsPath));
            else m_customLoadouts = new List<CustomLoadout>();

            var groupDefaultPath = Path.Combine(m_configDir, "Group_Default.json");
            if (File.Exists(groupDefaultPath)) m_groupDefault = JsonConvert.DeserializeObject<GroupDefault>(File.ReadAllText(groupDefaultPath));
            else m_groupDefault = new GroupDefault().InitializeDefault();

            var mainConfigPath = Path.Combine(m_configDir, "MainConfig.json");
            if (File.Exists(mainConfigPath)) m_mainConfig = JsonConvert.DeserializeObject<MainConfig>(File.ReadAllText(mainConfigPath));
            else m_mainConfig = new MainConfig().InitializeDefault();

            var pdaConfigPath = Path.Combine(m_configDir, "PdaConfig.json");
            if (File.Exists(pdaConfigPath)) m_pdaConfig = JsonConvert.DeserializeObject<PdaConfig>(File.ReadAllText(pdaConfigPath));
            else m_pdaConfig = new PdaConfig().InitializeDefault();

            var tradingConfigPath = Path.Combine(m_configDir, "TradingConfig.json");
            if (File.Exists(tradingConfigPath)) m_tradingConfig = JsonConvert.DeserializeObject<TradingConfig>(File.ReadAllText(tradingConfigPath));
            else m_tradingConfig = new TradingConfig().InitializeDefault();

            var zonesConfigPath = Path.Combine(m_configDir, "ZonesConfig.json");
            if (File.Exists(zonesConfigPath)) m_zonesConfig = JsonConvert.DeserializeObject<ZonesConfig>(File.ReadAllText(zonesConfigPath));
            else m_zonesConfig = new ZonesConfig().InitializeDefault();

            m_groupFactions = new List<GroupFaction>();
            foreach (var groupName in m_mainConfig.m_groups)
            {
                GroupFaction groupFaction = null;
                var groupFactionPath = Path.Combine(m_configDir, $"Group_{groupName}.json");
                if (File.Exists(groupFactionPath)) groupFaction = JsonConvert.DeserializeObject<GroupFaction>(File.ReadAllText(groupFactionPath));
                else groupFaction = new GroupFaction().InitializeDefault();
                m_groupFactions.Add(groupFaction);
            }

            this.UpdateVersioncheck();
            this.Save();
        }

        private void UpdateVersioncheck()
        {
            foreach (var trader in m_tradingConfig.m_traders)
            {
                if (string.IsNullOrWhiteSpace(trader.m_displayName))
                {
                    trader.m_displayName = "Trader " + trader.m_traderId;
                }
            }

            if (string.IsNullOrWhiteSpace(m_zonesConfig.m_defaultZone.m_displayName))
            {
                m_zonesConfig.m_defaultZone.m_displayName = "Default Zone";
            }
            foreach (var zone in m_zonesConfig.m_customZones)
            {
                if (string.IsNullOrWhiteSpace(zone.m_displayName))
                {
                    zone.m_displayName = "Zone " + zone.m_id;
                }
            }
        }

        public void Save()
        {
            var saveSettings = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented
            };
            File.WriteAllText(Path.Combine(m_configDir, "AdminToolOptions.json"), JsonConvert.SerializeObject(m_adminToolOptions, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "ClientConfig.json"), JsonConvert.SerializeObject(m_clientConfig, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "CustomLoadouts.json"), JsonConvert.SerializeObject(m_customLoadouts, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "Group_Default.json"), JsonConvert.SerializeObject(m_groupDefault, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "MainConfig.json"), JsonConvert.SerializeObject(m_mainConfig, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "PdaConfig.json"), JsonConvert.SerializeObject(m_pdaConfig, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "TradingConfig.json"), JsonConvert.SerializeObject(m_tradingConfig, saveSettings));
            File.WriteAllText(Path.Combine(m_configDir, "ZonesConfig.json"), JsonConvert.SerializeObject(m_zonesConfig, saveSettings));

            for (int i = 0; i < m_mainConfig.m_groups.Count; i++)
            {
                var name = m_mainConfig.m_groups[i];
                var faction = m_groupFactions[i];

                if (faction.m_allowDefaultSpawnpoints == 0)
                {
                    if (!faction.m_spawnpoints.Any(x => x.m_positions.Any()))
                    {
                        faction.m_allowDefaultSpawnpoints = 1;
                    }
                }

                File.WriteAllText(Path.Combine(m_configDir, $"Group_{name}.json"), JsonConvert.SerializeObject(faction, saveSettings));
            }

            if (m_crafting_instance != null)
            {
                File.WriteAllText(Path.Combine(m_configDir, "CraftingSettings.json"), JsonConvert.SerializeObject(m_crafting_instance, saveSettings));
            }
        }

        public void Apply(JToken token)
        {
            MergeObject(this, token);
        }

        private void MergeObject(object obj, JToken token)
        {
            foreach (var jprop in token.Children<JProperty>())
            {
                var prop = obj.GetType().GetProperty(jprop.Name);
                if (prop.PropertyType == typeof(int))
                {
                    prop.SetValue(obj, jprop.Value.Value<int>());
                }
                else if (prop.PropertyType == typeof(float))
                {
                    prop.SetValue(obj, jprop.Value.Value<float>());
                }
                else if (prop.PropertyType == typeof(string))
                {
                    prop.SetValue(obj, jprop.Value.Value<string>());
                }
                else if (prop.PropertyType.IsGenericType && prop.PropertyType.GetGenericTypeDefinition() == typeof(List<>))
                {
                    if (prop.PropertyType == typeof(List<string>))
                    {
                        prop.SetValue(obj, jprop.Value.Values<string>().ToList());
                    }
                    else if (prop.PropertyType == typeof(List<int>))
                    {
                        prop.SetValue(obj, jprop.Value.Values<int>().ToList());
                    }
                    else if (prop.PropertyType == typeof(List<float>))
                    {
                        prop.SetValue(obj, jprop.Value.Values<float>().ToList());
                    }
                    else
                    {
                        var jarray = jprop.Value as JArray;
                        var listIfc = prop.GetValue(obj) as IList;
                        listIfc.Clear();

                        foreach (var jelement in jarray) 
                        {
                            var method = typeof(Newtonsoft.Json.JsonConvert).GetMethods().First(x => x.IsGenericMethod && x.GetParameters().Length == 1 && x.Name.Equals("DeserializeObject"));
                            var generic = method.MakeGenericMethod(prop.PropertyType.GetGenericArguments()[0]);
                            var result = generic.Invoke(null, new object[] { jelement.ToString(Formatting.None) });
                            listIfc.Add(result);
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    if (prop.PropertyType.GetElementType() == typeof(int))
                    {
                        prop.SetValue(obj, jprop.Value.Values<int>().ToArray());
                    }
                    else if (prop.PropertyType.GetElementType() == typeof(float))
                    {
                        prop.SetValue(obj, jprop.Value.Values<float>().ToArray());
                    }
                    else if (prop.PropertyType.GetElementType() == typeof(string))
                    {
                        prop.SetValue(obj, jprop.Value.Values<string>().ToArray());
                    }
                    else if (prop.PropertyType.GetElementType().IsClass)
                    {
                        var array = (Array)prop.GetValue(obj);
                        var jrray = jprop.Value as JArray;
                        if (jrray != null && array.Length != jrray.Count)
                        {
                            throw new ApplicationException("Missmatch array length.");
                        }

                        for (int i = 0; i < array.Length; i++)
                        {
                            MergeObject(array.GetValue(i), jrray[i]);
                        }
                    }
                }
                else if (prop.PropertyType.IsClass && prop.PropertyType.Assembly == GetType().Assembly)
                {
                    MergeObject(prop.GetValue(obj), jprop.Value);
                }
                else
                {
                    throw new ApplicationException("Undefined type to remap.");
                }
            }
        }

        public void ReplaceValues(StringBuilder sb)
        {
            ReplaceValues(sb, this, string.Empty);
        }

        private void ReplaceValues(StringBuilder sb, object obj, string path)
        {
            foreach (var prop in obj.GetType().GetProperties())
            {
                var mainPattern = "{";
                if (path.Length > 0)
                {
                    mainPattern += path + ".";
                }
                mainPattern += prop.Name + "}";
                var singlePattern = "$" + mainPattern;

                if (prop.GetCustomAttributes(typeof(ScriptObjectSerializableAttribute), true)?.Length == 1)
                {
                    sb.ReplaceScriptObject("#" + mainPattern, prop.GetValue(obj));
                    continue;
                }

                if (prop.PropertyType == typeof(int))
                {
                    sb.Replace(singlePattern, (int)prop.GetValue(obj));
                }
                else if (prop.PropertyType == typeof(float))
                {
                    sb.Replace(singlePattern, (float)prop.GetValue(obj));
                }
                else if (prop.PropertyType == typeof(string))
                {
                    sb.Replace(singlePattern, (string)prop.GetValue(obj));
                }
                else if (prop.PropertyType.IsGenericType && prop.PropertyType.GetGenericTypeDefinition() == typeof(List<>))
                {
                    if (prop.PropertyType == typeof(List<string>))
                    {
                        sb.Replace(singlePattern, (List<string>)prop.GetValue(obj));
                    }
                    else if (prop.PropertyType == typeof(List<float>))
                    {
                        sb.Replace(singlePattern, (List<float>)prop.GetValue(obj));
                    }
                    else if (prop.PropertyType == typeof(List<int>))
                    {
                        sb.Replace(singlePattern, (List<int>)prop.GetValue(obj));
                    }
                    else
                    {
                        int index = 0;
                        foreach (var elem in (IEnumerable)prop.GetValue(obj))
                        {
                            var newPath = path;
                            if (newPath.Length > 0) newPath += ".";
                            newPath += prop.Name + "[" + index + "]";
                            ReplaceValues(sb, elem, newPath);
                            index++;
                        }
                    }
                }
                else if (prop.PropertyType.IsArray)
                {
                    if (prop.PropertyType.GetElementType() == typeof(string))
                    {
                        sb.Replace(singlePattern, (string[])prop.GetValue(obj));
                    }
                    else if (prop.PropertyType.GetElementType() == typeof(float))
                    {
                        sb.Replace(singlePattern, (float[])prop.GetValue(obj));
                    }
                    else if (prop.PropertyType.GetElementType() == typeof(int))
                    {
                        sb.Replace(singlePattern, (int[])prop.GetValue(obj));
                    }
                    else
                    {
                        int index = 0;
                        foreach (var elem in (IEnumerable)prop.GetValue(obj))
                        {
                            var newPath = path;
                            if (newPath.Length > 0) newPath += ".";
                            newPath += prop.Name + "[" + index + "]";
                            ReplaceValues(sb, elem, newPath);
                            index++;
                        }
                    }
                }
                else if (prop.PropertyType.IsClass && prop.PropertyType.Assembly == GetType().Assembly)
                {
                    var newPath = path;
                    if (newPath.Length > 0) newPath += ".";
                    newPath += prop.Name;
                    ReplaceValues(sb, prop.GetValue(obj), newPath);
                }
                else
                {
                    throw new ApplicationException("Undefined type to remap.");
                }
            }
        }

        public class AdminToolOptions
        {
            public List<string> m_adminUids { set; get; }

            public List<SpawnerCategory> m_spawnerCategories { set; get; }

            public class SpawnerCategory
            {
                public string m_name { set; get; }
                public string m_preffix { set; get; }
                public string m_class { set; get; }
            }

            public AdminToolOptions InitializeDefault()
            {
                m_adminUids = new List<string>();
                m_spawnerCategories = new List<SpawnerCategory>()
                {
                    new SpawnerCategory() { m_name = "Items", m_preffix = "CfgVehicles", m_class = "Inventory_Base" },
                    new SpawnerCategory() { m_name = "Weapons", m_preffix = "CfgWeapons", m_class = "*" },
                    new SpawnerCategory() { m_name = "Magazines", m_preffix = "CfgMagazines", m_class = "*" },
                    new SpawnerCategory() { m_name = "Clothes", m_preffix = "CfgVehicles", m_class = "Clothing" },
                    new SpawnerCategory() { m_name = "Transport", m_preffix = "CfgVehicles", m_class = "Car" },
                    new SpawnerCategory() { m_name = "Creatures", m_preffix = "CfgVehicles", m_class = "DZ_LightAI" },
                    new SpawnerCategory() { m_name = "Buildings", m_preffix = "CfgVehicles", m_class = "House" }
                };
                return this;
            }
        }

        public class ClientConfig
        {
            public int m_checkIdentityMode { set; get; }
            public int m_operateVisceraHimself { set; get; }
            public int m_skillLevelSize { set; get; }
            public int m_skillLevelModifier { set; get; }

            public ClientConfig InitializeDefault()
            {
                m_checkIdentityMode = 1;
                m_operateVisceraHimself = 1;
                m_skillLevelSize = 1000;
                m_skillLevelModifier = 200;
                return this;
            }
        }

        public class MainConfig
        {
            public int m_startSoulsCount { set; get; }
            public int m_respawnSoulsPrice { set; get; }
            public int m_newchar_points { set; get; }
            public int m_roleplay_mode { set; get; }
            public List<string> m_groups { set; get; }
            public SkillModifier[] m_skillModifiers { set; get; }
            public List<string> m_startMedicKit { set; get; }
            public List<string> m_startStealthKit { set; get; }
            public List<string> m_startSurvivorKit { set; get; }

            public MainConfig InitializeDefault()
            {
                m_startSoulsCount = 1;
                m_respawnSoulsPrice = 0;
                m_newchar_points = 30;
                m_roleplay_mode = 0;
                m_groups = new List<string>();
                m_skillModifiers = Enumerable.Range(0, 18).Select(x => new SkillModifier().InitializeDefault(x)).ToArray();
                m_startMedicKit = new List<string>()
                {
                    "BandageDressing",
                    "AmpouleAlcohol",
                    "TabletsAnalgin"
                };
                m_startStealthKit = new List<string>()
                {
                    "GhillieBushrag_Mossy",
                    "MKII",
                    "AmmoBox_22_50Rnd"
                };
                m_startSurvivorKit = new List<string>()
                {
                    "Matchbox",
                    "Canteen",
                    "Crackers"
                };
                return this;
            }

            public class SkillModifier
            {
                public int m_id { set; get; }
                public float m_mod { set; get; }
                public float m_decreaseOnDeath { set; get; }

                public SkillModifier InitializeDefault(int id)
                {
                    m_id = id;
                    m_mod = 1;
                    m_decreaseOnDeath = 0;
                    return this;
                }
            }
        }

        public class GroupDefault
        {
            public List<SpawnpointInfo> m_spawnpoints { set; get; }
            public List<string> m_gearBody { set; get; }
            public List<string> m_gearPants { set; get; }
            public List<string> m_gearFoot { set; get; }
            public List<string> m_gearHead { set; get; }
            public List<string> m_gearMask { set; get; }
            public List<string> m_gearGloves { set; get; }
            public List<string> m_gearVest { set; get; }
            public List<string> m_gearBackpack { set; get; }
            public List<string> m_gearWeapon { set; get; }
            public List<ItemsLoadout> m_gearItems { set; get; }

            public GroupDefault InitializeDefault()
            {
                m_spawnpoints = new List<SpawnpointInfo>()
                {
                    new SpawnpointInfo().InitializeDefault()
                };

                m_gearBody = new List<string>()
                {
                    "Hoodie_Blue",
                    "Hoodie_Black",
                    "Hoodie_Brown",
                    "Hoodie_Green",
                    "Hoodie_Grey",
                    "Hoodie_Red",
                };

                m_gearPants = new List<string>()
                {
                    "Jeans_Black",
                    "Jeans_BlueDark",
                    "Jeans_Blue",
                    "Jeans_Brown",
                    "Jeans_Green",
                    "Jeans_Grey",
                };

                m_gearFoot = new List<string>()
                {
                    "AthleticShoes_Black; H0.5",
                    "AthleticShoes_Blue; H0.5",
                    "AthleticShoes_Brown; H0.5",
                    "AthleticShoes_Green; H0.5",
                    "AthleticShoes_Grey; H0.5",
                };

                m_gearHead = new List<string>();
                m_gearMask = new List<string>();
                m_gearGloves = new List<string>();
                m_gearVest = new List<string>();
                m_gearBackpack = new List<string>();
                m_gearWeapon = new List<string>();
                m_gearItems = new List<ItemsLoadout>()
                {
                    new ItemsLoadout()
                    {
                        m_name = "Default starter pack",
                        m_items = new List<string>()
                        {
                            "Heatpack", "Roadflare", "Rag; Q4"
                        }
                    }
                };

                return this;
            }

            public class SpawnpointInfo
            {
                public string m_name { set; get; }
                public List<SpawnpointPos> m_positions { set; get; }

                public SpawnpointInfo InitializeDefault()
                {
                    m_name = "Chernarus";
                    m_positions = new List<SpawnpointPos>() {
                        new SpawnpointPos { m_pos = new float[] { 9107.57f, 0, 1918.01f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9621.27f, 0, 1722.75f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8572.02f, 0, 2302.69f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6800.94f, 0, 2900.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 7475.94f, 0, 3505.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8333.94f, 0, 3166.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6273.94f, 0, 3533.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 5853.94f, 0, 2853.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6050.94f, 0, 2300.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6655.94f, 0, 3777.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8294.81f, 0, 2915.43f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6052.60f, 0, 1868.23f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8212.17f, 0, 2781.53f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6133.28f, 0, 2021.43f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9691.70f, 0, 1750.40f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9529.70f, 0, 1791.20f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9193.70f, 0, 1935.70f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 6218.78f, 0, 2113.35f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10875.80f, 0, 2518.90f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10427.39f, 0, 1921.14f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10820.40f, 0, 2257.40f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8491.93f, 0, 2458.95f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8391.22f, 0, 2412.53f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8285.94f, 0, 2404.75f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8057.56f, 0, 2807.67f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10917.01f, 0, 2615.05f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8340.27f, 0, 2558.35f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8659.26f, 0, 2220.42f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10567.46f, 0, 2031.55f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 7080.94f, 0, 2482.06f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 8830.18f, 0, 2145.05f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 7430.73f, 0, 2589.35f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10862.03f, 0, 2380.13f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9814.53f, 0, 1781.71f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 10647.47f, 0, 2100.81f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 9875.13f, 0, 1763.90f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12480.87f, 0, 3567.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13439.89f, 0, 5374.18f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11846.80f, 0, 3477.80f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11914.30f, 0, 3402.00f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11049.30f, 0, 2801.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11824.08f, 0, 3381.01f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13374.40f, 0, 6454.30f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13426.60f, 0, 5747.30f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13440.40f, 0, 5624.23f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13429.40f, 0, 6679.20f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13441.60f, 0, 5262.20f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13485.64f, 0, 5894.26f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12004.95f, 0, 3422.10f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13262.80f, 0, 7225.80f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12354.50f, 0, 3480.00f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13084.90f, 0, 7938.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13169.18f, 0, 7491.12f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13542.07f, 0, 6088.35f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13311.50f, 0, 7028.93f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13076.36f, 0, 8045.23f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12215.67f, 0, 3425.20f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 7173.87f, 0, 2555.93f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13427.16f, 0, 5547.37f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13485.84f, 0, 5996.81f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11085.15f, 0, 2918.78f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13227.35f, 0, 7366.54f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11083.43f, 0, 2731.88f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13528.04f, 0, 6464.11f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13531.71f, 0, 6155.58f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13300.82f, 0, 7123.74f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13488.46f, 0, 6538.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13536.13f, 0, 6229.02f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13587.80f, 0, 6026.50f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 11860.246094f, 0, 12046.235352f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13762.574219f, 0, 13034.944336f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14047.80f, 0, 11338.82f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13879.289063f, 0, 12931.032227f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14048.921875f, 0, 12980.696289f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14410.37f, 0, 13316.80f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 15135.10f, 0, 13901.10f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14749.70f, 0, 13248.70f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14586.68f, 0, 13343.90f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14367.738281f, 0, 13317.543945f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 15017.80f, 0, 13892.40f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13516.61f, 0, 11061.47f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13207.05f, 0, 10428.54f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12991.96f, 0, 8494.20f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13289.101563f, 0, 12777.155273f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12978.90f, 0, 9727.80f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12868.70f, 0, 9054.50f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12929.70f, 0, 8578.30f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13666.357422f, 0, 13429.699219f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12917.30f, 0, 9356.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12889.90f, 0, 8792.80f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13048.10f, 0, 8357.60f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13607.30f, 0, 11232.61f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14326.70f, 0, 13012.22f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14040.663086f, 0, 14066.592773f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12899.06f, 0, 9182.89f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12507.900391f, 0, 15093.877930f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13190.13f, 0, 10227.21f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13053.61f, 0, 9536.82f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14263.27f, 0, 13033.67f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13402.337891f, 0, 13310.053711f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13082.70f, 0, 8112.25f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13084.31f, 0, 9648.43f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13246.81f, 0, 10595.75f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13261.393555f, 0, 15001.236328f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13306.58f, 0, 10703.12f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14499.25f, 0, 13309.64f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12954.36f, 0, 8721.21f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13001.47f, 0, 9497.56f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13439.176758f, 0, 13171.535156f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13000.41f, 0, 9809.74f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12997.78f, 0, 9380.88f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13572.62f, 0, 11127.85f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13227.64f, 0, 10298.00f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14339.32f, 0, 12861.29f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14606.92f, 0, 13275.15f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13915.767578f, 0, 14138.711914f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13127.29f, 0, 8337.39f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12930.02f, 0, 9276.75f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 13244.69f, 0, 10138.84f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14296.240234f, 0, 14935.049805f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14326.32f, 0, 13333.03f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 12447.718750f, 0, 13427.550781f }, m_radius = 10 },
                        new SpawnpointPos { m_pos = new float[] { 14043.812500f, 0, 14796.046875f }, m_radius = 10 }
                    };
                    return this;
                }
            }

            public class SpawnpointPos
            {
                public float[] m_pos { set; get; }
                public float m_radius { set; get; }
            }

            public class ItemsLoadout
            {
                public string m_name { set; get; }
                public List<string> m_items { set; get; }
            }
        }

        public class GroupFaction : GroupDefault
        {
            public string m_displayName { set; get; }
            public int m_maxMembers { set; get; }
            public int m_allowDefaultSpawnpoints { set; get; }
            public int m_allowDefaultLoadout { set; get; }
            public List<string> m_leaders { set; get; }

            public new GroupFaction InitializeDefault()
            {
                base.InitializeDefault();
                m_displayName = string.Empty;
                m_maxMembers = 100;
                m_allowDefaultLoadout = 1;
                m_allowDefaultSpawnpoints = 1;
                m_leaders = new List<string>();
                return this;
            }
        }

        public class CustomLoadout : GroupDefault
        {
            public string m_displayName { set; get; }
            public string m_uid { set; get; }
        }

        public class PdaConfig
        {
            public int m_enableGlobalChat { set; get; }
            public int m_enableMapPage { set; get; }
            public string m_serverInstanceName { set; get; }

            public PdaConfig InitializeDefault()
            {
                m_enableGlobalChat = 1;
                m_enableMapPage = 1;
                m_serverInstanceName = Guid.NewGuid().ToString();
                return this;
            }
        }

        public class TradingConfig
        {
            public List<TraderInfo> m_traders { set; get; }

            public TradingConfig InitializeDefault()
            {
                m_traders = new List<TraderInfo>()
                {
                    new TraderInfo().InitializeDefault(0)
                };

                return this;
            }

            public class TraderInfo
            {
                public int m_traderId { set; get; }
                public string m_displayName { set; get; }
                public string m_classname { set; get; }
                public List<string> m_attachments { set; get; }
                public float[] m_position { set; get; }
                public float m_rotation { set; get; }
                public List<string> m_buyFilter { set; get; }
                public List<string> m_sellFilter { set; get; }
                public int m_storageMaxSize { set; get; }
                public float m_storageCommission { set; get; }
                public string m_dumpingByAmountAlgorithm { set; get; }
                public float m_dumpingByAmountModifier { set; get; }
                public float m_dumpingByBadQuality { set; get; }
                public float m_sellMaxQuantityPercent { set; get; }
                public float m_buyMaxQuantityPercent { set; get; }

                public TraderInfo InitializeDefault(int id)
                {
                    m_traderId = id;
                    m_displayName = "Trader " + id;
                    m_classname = "SurvivorM_Boris";
                    m_attachments = new List<string>() 
                    {
                        "FlatCap_Brown", "OMNOGloves_Gray", "RidersJacket_Black", "JungleBoots_Black", "Breeches_Black", "ThickFramesGlasses"
                    };
                    m_position = new float[3] { 8176.6f, 474.0f, 9122.6f };
                    m_rotation = 160f;
                    m_buyFilter = new List<string>() { "Inventory_Base", "DefaultWeapon", "DefaultMagazine" };
                    m_sellFilter = new List<string>() { "Inventory_Base", "DefaultWeapon", "DefaultMagazine", "!RDG2SmokeGrenade_ColorBase", "!M18SmokeGrenade_ColorBase" };
                    m_storageMaxSize = 1000;
                    m_storageCommission = 0.25f;
                    m_dumpingByAmountAlgorithm = "linear";
                    m_dumpingByAmountModifier = 0.01f;
                    m_dumpingByBadQuality = 0.5f;
                    m_sellMaxQuantityPercent = 0.1f;
                    m_buyMaxQuantityPercent = 0.1f;
                    return this;
                }
            }
        }

        public class ZonesConfig
        {
            public Zone m_defaultZone { set; get; }
            public List<Zone> m_customZones { set; get; }

            public ZonesConfig InitializeDefault()
            {
                m_defaultZone = new Zone().InitializeDefault(0);
                m_customZones = new List<Zone>();
                return this;
            }

            public class Zone
            {
                public int m_id { set; get; }
                public string m_displayName { set; get; }
                public float[] m_position { set; get; }
                public float m_radius { set; get; }
                public float m_height { set; get; }
                public string m_enterMessage { set; get; }
                public string m_leaveMessage { set; get; }
                public float m_leaveTime { set; get; }
                public int m_godMode { set; get; }
                public int m_inverseDammage { set; get; }
                public int m_blockBuildingMode { set; get; }
                public int m_blockInteractionWithPlayers { set; get; }
                public float m_radiation { set; get; }
                public float m_psi { set; get; }
                public float m_gas { set; get; }
                public float m_spawnChance { set; get; }

                public Zone InitializeDefault(int id)
                {
                    m_id = id;
                    m_position = new float[3] { 0, 0, 0 };
                    m_radius = 0;
                    m_height = 0;
                    m_enterMessage = string.Empty;
                    m_leaveMessage = string.Empty;
                    m_leaveTime = 0;
                    m_godMode = 0;
                    m_inverseDammage = 0;
                    m_blockBuildingMode = 0;
                    m_blockInteractionWithPlayers = 0;
                    m_radiation = 0;
                    m_psi = 0;
                    m_radiation = 0;
                    m_gas = 0;
                    m_spawnChance = 1;
                    return this;
                }
            }
        }

        public class CraftingConfig
        {
            public List<string> m_allDefaultRecipes { set; get; }
            public List<string> m_disabledDefaultRecipes { set; get; }
            public List<CustomRecipeData> m_customRecipes { set; get; }

            public CraftingConfig InitializeDefault()
            {
                m_allDefaultRecipes = new List<string>();
                m_disabledDefaultRecipes = new List<string>();
                m_customRecipes = new List<CustomRecipeData>();
                return this;
            }

            public class CustomRecipeData
            {
                public string m_Name { set; get; }
                public int m_IsInstaRecipe { set; get; }
                public float m_AnimationLength { set; get; }

                public List<string>[] m_Ingredients { set; get; }
                public float[] m_MinQuantityIngredient { set; get; }
                public float[] m_MaxQuantityIngredient { set; get; }
                public int[] m_MinDamageIngredient { set; get; }
                public int[] m_MaxDamageIngredient { set; get; }
                public float[] m_IngredientAddHealth { set; get; }
                public float[] m_IngredientAddQuantity { set; get; }
                public float[] m_IngredientSetHealth { set; get; }
                public int[] m_IngredientDestroy { set; get; }

                public List<int> m_ResultSetFullQuantity { set; get; }
                public List<float> m_ResultSetQuantity { set; get; }
                public List<float> m_ResultSetHealth { set; get; }
                public List<int> m_ResultToInventory { set; get; }
                public List<int> m_ResultInheritsHealth { set; get; }
                public List<string> m_ItemsToCreate { set; get; }
            }
        };
    }
}
