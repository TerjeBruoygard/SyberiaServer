using Nancy.Hosting.Self;
using NLog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SyberiaWebPanel
{
    public class WebPanel
    {
        private static Logger logger = LogManager.GetCurrentClassLogger();

        private static WebPanel instance = null;

        private NancyHost host = null;

        private GameConfig gameConfig = null;

        public WebPanel(string gamedir)
        {
            gameConfig = new GameConfig(gamedir);
            instance = this;
        }

        public void Start(int port)
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true }
            };
            host = new NancyHost(hostConfigs, new Uri("http://localhost:" + port));
            host.Start();
            logger.Info("WebPanel server listening on port: " + port);
        }

        public GameConfig GetGameConfig()
        {
            return gameConfig;
        }

        public void Stop()
        {
            host?.Stop();
        }

        public static WebPanel GetInstance()
        {
            return instance;
        }
    }
}
