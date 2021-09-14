using Nancy.Hosting.Self;
using NLog;
using NLog.Config;
using NLog.Layouts;
using NLog.Targets;
using System;
using System.Threading;

namespace SyberiaServerManager
{
    static class EntryPoint
    {
        private static NancyHost host = null;

        private static Logger logger = null;

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

                // Configure REST API
                HostConfiguration hostConfigs = new HostConfiguration()
                {
                    UrlReservations = new UrlReservations() { CreateAutomatically = true }
                };
                host = new NancyHost(hostConfigs, new Uri("http://localhost:" + options.Port));
                host.Start();
                logger.Info("Database server listening on port: " + options.Port);

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
            if (host != null)
            {
                host.Stop();
            }

            logger.Info("Server stopped");
        }
    }
}
