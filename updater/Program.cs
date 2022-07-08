using Nancy.Hosting.Self;
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
        private static String accessKey = null;

        private static NancyHost host = null;

        private static Logger logger = null;

        public static bool ValidateAccessKey(String value)
        {
            return accessKey.Equals(value, StringComparison.InvariantCulture);
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

        private static void ConfigureAccessKey()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "credentials.json");
            if (!File.Exists(path))
            {
                logger.Fatal($"Failed to open file '{path}'.");
                throw new ApplicationException("Credentials file does not exists.");
            }

            var result = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(File.ReadAllText(path));
            accessKey = result["accessKey"];
        }

        private static void ConfigureRestApi()
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true }
            };

            int port = 8055;
            host = new NancyHost(hostConfigs, new Uri($"http://localhost:{port}"));
            host.Start();
            logger.Info($"Database server listening on port: {port}");
        }

        static void Main(string[] args)
        {
            ConfigureLogger();
            ConfigureAccessKey();
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
