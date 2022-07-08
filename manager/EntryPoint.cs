using Nancy.Hosting.Self;
using NLog;
using NLog.Config;
using NLog.Layouts;
using NLog.Targets;
using SyberiaDatabase;
using SyberiaWebPanel;
using System;
using System.IO;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;

namespace SyberiaServerManager
{
    static class EntryPoint
    {
        private static NancyHost host = null;

        private static Logger logger = null;

        private static string dayzServerDir = null;

        private static WebPanel webPanel = null;

        private static void Main(string[] args)
        {
            CommandLineOptions.Parse(args, options =>
            {
                // Configure logging
                if (options.NoLogs)
                {
                    LogManager.DisableLogging();
                }
                else
                {
                    var config = new LoggingConfiguration();
                    var minLoglevel = options.Debug ? LogLevel.Debug : LogLevel.Info;
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
                }
                logger = LogManager.GetCurrentClassLogger();

                // Read database server options
                dayzServerDir = options.ServerDir;
                if (!Directory.Exists(dayzServerDir))
                {
                    logger.Fatal("DayZ Server directory not exists (Please check 'serverdir' argument).");
                    Environment.Exit(255);
                    return;
                }

                var databaseOptionsPath = Path.Combine(dayzServerDir, "profiles", "DatabaseOptions.json");
                if (!File.Exists(databaseOptionsPath))
                {
                    logger.Fatal("'DatabaseOptions.json' file not exists.");
                    Environment.Exit(255);
                    return;
                }

                var databaseOptions = Newtonsoft.Json.JsonConvert.DeserializeObject<DatabaseOptions>(File.ReadAllText(databaseOptionsPath));

                // Configure REST API
                HostConfiguration hostConfigs = new HostConfiguration()
                {
                    UrlReservations = new UrlReservations() { CreateAutomatically = true },
                    RewriteLocalhost = false,
                };
                host = new NancyHost(hostConfigs, new Uri("http://localhost:" + databaseOptions.DatabaseServerPort));
                host.Start();
                logger.Info("Database server listening on port: " + databaseOptions.DatabaseServerPort);

                // Run WebPanel
                webPanel = new WebPanel(dayzServerDir);
                webPanel.Start(options.WebServerPort);

                // Configure app exit
                AppDomain.CurrentDomain.ProcessExit += new EventHandler(CurrentDomain_ProcessExit);
                Console.CancelKeyPress += CurrentDomain_ProcessExit;
            });

            while (true)
            {
                Thread.Sleep(100);
            }
        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            host?.Stop();
            webPanel?.Stop();
            logger.Info("Server stopped");
        }

        public static string GetDayzServerDir()
        {
            return dayzServerDir;
        }
    }
}
