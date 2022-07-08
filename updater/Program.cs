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

        private static string[] masterAddress = null;

        private static List<string> whitelist = null;

        private static Uri[] baseUris = null;

        private static NancyHost host = null;

        private static Logger logger = null;

        public static bool ValidateWhitelistAddress(string ipAddress)
        {
            return whitelist.Contains(ipAddress);
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
            masterAddress = (result["masterAddress"] as JArray).Select(x => x.Value<string>()).ToArray();
            baseUris = (result["baseUris"] as JArray).Select(x => new Uri(x.Value<string>())).ToArray();
            if (accessKey == null || 
                masterKey == null || 
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
