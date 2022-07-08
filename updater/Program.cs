using Nancy.Hosting.Self;
using Newtonsoft.Json.Linq;
using NLog;
using NLog.Config;
using NLog.Layouts;
using NLog.Targets;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SyberiaUpdaterServer
{
    class Program
    {
        private static string accessKey = null;

        private static string masterKey = null;

        private static string statisticKey = null;

        private static string[] masterAddress = null;

        private static List<string> whitelist = null;

        private static Uri[] baseUris = null;

        private static NancyHost host = null;

        private static Logger logger = null;

        public static bool ValidateWhitelistAddress(string ipAddress)
        {
            return whitelist.Contains(ipAddress);
        }

        public static bool ValidateStatisticKey(string value)
        {
            return statisticKey.Equals(value, StringComparison.InvariantCulture);
        }

        public static bool ValidateAccessKey(string value, string ipAddress)
        {
            return accessKey.Equals(value, StringComparison.InvariantCulture) && whitelist.Contains(ipAddress);
        }

        public static bool ValidateMasterKey(string value, string ipAddress)
        {
            return masterKey.Equals(value, StringComparison.InvariantCulture) && masterAddress.Contains(ipAddress);
        }

        public static bool AddToWhitelist(string ipAddress)
        {
            if (!whitelist.Contains(ipAddress))
            {
                whitelist.Add(ipAddress);
                File.WriteAllLines(GetWhitelistPath(), whitelist);
                return true;
            }

            return false;
        }

        public static bool RemoveFromWhitelist(string ipAddress)
        {
            if (whitelist.Contains(ipAddress))
            {
                whitelist.Remove(ipAddress);
                File.WriteAllLines(GetWhitelistPath(), whitelist);
                return true;
            }

            return false;
        }

        private static string GetWhitelistPath()
        {
            return Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "whitelist.txt");
        }

        public static string[] GetServiceStatistic(DateTime date)
        {
            var statisticFile = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "statistic", $"stat_{date.Year}_{date.Month}_{date.Day}.txt");
            if (File.Exists(statisticFile))
            {
                return File.ReadAllLines(statisticFile);
            }

            return null;
        }

        public static Dictionary<DateTime, string[]> GetServiceStatistic(string from, string to)
        {
            var parts = from.Split('.');
            if (parts.Length != 3)
            {
                return null;
            }

            int fromYear, fromMonth, fromDay;
            if (!int.TryParse(parts[2], out fromYear) || !int.TryParse(parts[1], out fromMonth) || !int.TryParse(parts[0], out fromDay))
            {
                return null;
            }

            parts = to.Split('.');
            if (parts.Length != 3)
            {
                return null;
            }

            int toYear, toMonth, toDay;
            if (!int.TryParse(parts[2], out toYear) || !int.TryParse(parts[1], out toMonth) || !int.TryParse(parts[0], out toDay))
            {
                return null;
            }

            var fromDate = new DateTime(fromYear, fromMonth, fromDay);
            var toDate = new DateTime(toYear, toMonth, toDay);
            var result = new Dictionary<DateTime, string[]>();
            do
            {
                var lines = GetServiceStatistic(fromDate);
                if (lines != null)
                {
                    result.Add(fromDate, lines);
                }

                fromDate = fromDate.AddDays(1);
            } while (fromDate <= toDate);

            return result;
        }

        public static void AddServiceStartToStatistic(string ip, JToken data)
        {
            var statisticDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "statistic");
            if (!Directory.Exists(statisticDir))
            {
                Directory.CreateDirectory(statisticDir);
            }

            var datetime = DateTime.Now;
            var statisticFile = Path.Combine(statisticDir, $"stat_{datetime.Year}_{datetime.Month}_{datetime.Day}.txt");
            var statisticData = $"{datetime.Hour}:{datetime.Minute}:{datetime.Second}|{ip}|{data["dbPort"].Value<int>()}|{data["webPort"].Value<int>()}|{data["dayzDir"].Value<string>()}|{data["serviceDir"].Value<string>()}";
            File.AppendAllLines(statisticFile, new string[] { statisticData });
        }

        private static void ConfigureLogger()
        {
            var config = new LoggingConfiguration();
            var minLoglevel = LogLevel.Info;
            var maxLogLevel = LogLevel.Fatal;
            config.AddRule(minLoglevel, maxLogLevel, new FileTarget("logfile")
            {
                FileName = "${basedir}/logs/${date:format=yyyy-MM-dd}.log",
                Layout = Layout.FromString("${longdate} ${level:uppercase=true:padding=5} - ${logger:shortName=true} - ${message} ${exception:format=tostring}"),
                KeepFileOpen = true
            });
            config.AddRule(minLoglevel, maxLogLevel, new ConsoleTarget("logconsole")
            {
                Layout = Layout.FromString("${level:uppercase=true:padding=5} - ${message} ${exception:format=tostring}")
            });
            LogManager.Configuration = config;
            logger = LogManager.GetCurrentClassLogger();
        }

        private static void ReadConfiguration()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "credentials.json");
            if (!File.Exists(path))
            {
                logger.Fatal($"Failed to open file '{path}'.");
                throw new ApplicationException("Credentials file does not exists.");
            }

            var result = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, object>>(File.ReadAllText(path));
            accessKey = result["accessKey"] as string;
            masterKey = result["masterKey"] as string;
            statisticKey = result["statisticKey"] as string;
            masterAddress = (result["masterAddress"] as JArray).Select(x => x.Value<string>()).ToArray();
            baseUris = (result["baseUris"] as JArray).Select(x => new Uri(x.Value<string>())).ToArray();
            if (accessKey == null || 
                masterKey == null ||
                statisticKey == null ||
                masterAddress == null || masterAddress.Length == 0 ||
                baseUris == null || baseUris.Length == 0)
            {
                throw new ApplicationException("Invalid credentials format.");
            }

            path = GetWhitelistPath();
            if (!File.Exists(path))
            {
                File.WriteAllText(path, string.Empty);
            }

            whitelist = File.ReadAllLines(path).Where(x => !string.IsNullOrWhiteSpace(x)).ToList();
        }

        private static void ConfigureRestApi()
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true },
            };

            host = new NancyHost(hostConfigs, baseUris);
            host.Start();
            logger.Info($"Syberia master server started");
        }

        static void Main(string[] args)
        {
            var thread = new Thread(new ThreadStart(Logic));
            thread.Start();

            while (thread.IsAlive)
            {
                Thread.Sleep(100);
            }
        }

        static void Logic()
        {
            ConfigureLogger();
            ReadConfiguration();
            ConfigureRestApi();

            // Configure app exit
            AppDomain.CurrentDomain.ProcessExit += new EventHandler(CurrentDomain_ProcessExit);
            Console.CancelKeyPress += CurrentDomain_ProcessExit;

            while (true)
            {
                Thread.Sleep(100);
            }
        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            if (host != null)
            {
                host.Stop();
            }

            logger.Info("Server stopped");
        }
    }
}
