using Nancy.Hosting.Self;
using NLog;
using System;
using System.Collections.Generic;
using System.IO;
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

        private InternalVariables internalVariables = null;

        private Tuple<string, string> credentials = null;

        private Dictionary<string, Tuple<DateTime, int>> authSpamFilter = new Dictionary<string, Tuple<DateTime, int>>();

        private string serverDir;

        public WebPanel(string gamedir)
        {
            this.serverDir = gamedir;
            this.gameConfig = new GameConfig(gamedir);
            this.internalVariables = new InternalVariables();

            var cfgData = ReadCfg(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "credentials.cfg"));
            if (cfgData.ContainsKey("login") && cfgData.ContainsKey("pass"))
            {
                var login = cfgData["login"];
                var md5Cryptor = System.Security.Cryptography.MD5.Create();
                byte[] md5Hash = md5Cryptor.ComputeHash(Encoding.UTF8.GetBytes(cfgData["pass"]));
                string base64Hex = string.Empty;
                foreach (byte bt in md5Hash)
                {
                    base64Hex += bt.ToString("X2").ToLowerInvariant();
                }

                credentials = new Tuple<string, string>(login, base64Hex);
            }
            else
            {
                logger.Error("Failed to configure WebPanel. 'credentials.cfg' not found or has invalid format.");
            }

            instance = this;
        }

        public void Start(int port)
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
#if DEBUG
                UrlReservations = new UrlReservations() { CreateAutomatically = true },
#else
                UrlReservations = new UrlReservations() { CreateAutomatically = false },
#endif
                RewriteLocalhost = true,
            };
            host = new NancyHost(hostConfigs, new Uri("http://localhost:" + port));
            host.Start();
            logger.Info("WebPanel server listening on port: " + port);
        }

        private Dictionary<string, string> ReadCfg(string path)
        {
            var result = new Dictionary<string, string>();

            if (File.Exists(path))
            {
                var content = File.ReadAllLines(path);
                foreach (var raw in content)
                {
                    var line = raw.Trim();
                    if (line.StartsWith(";") || !line.Contains("="))
                    {
                        continue;
                    }

                    var parts = line.Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries);
                    if (parts.Length != 2)
                    {
                        continue;
                    }

                    result.Add(parts[0].Trim(), parts[1].Trim());
                }
            }

            return result;
        }

        public GameConfig GetGameConfig()
        {
            return gameConfig;
        }

        public InternalVariables GetInternalVariables()
        {
            return internalVariables;
        }

        public IEnumerable<string> ReadBanList()
        {
            var bans = new List<string>();
            var path = Path.Combine(serverDir, "ban.txt");
            if (File.Exists(path))
            {
                var lines = File.ReadAllLines(path);
                foreach (var line in lines)
                {
                    var uid = line;
                    var index = uid.IndexOf("//");
                    if (index != -1)
                    {
                        uid = uid.Substring(0, index);
                    }

                    uid = uid.Trim();
                    if (!string.IsNullOrEmpty(uid))
                    {
                        bans.Add(uid);
                    }
                }
            }

            return bans;
        }

        public void ProccessBanUnban(string uid, bool ban)
        {
            var lines = new List<string>();
            var path = Path.Combine(serverDir, "ban.txt");
            if (File.Exists(path))
            {
                lines = File.ReadAllLines(path).ToList();
            }

            if (ban)
            {
                lines.Add($"{uid} // Ban added from syberia webpanel");
            }
            else
            {
                lines.RemoveAll(line =>
                {
                    var value = line;
                    var index = value.IndexOf("//");
                    if (index != -1)
                    {
                        value = value.Substring(0, index);
                    }

                    return value.Trim().Equals(uid);
                });
            }

            File.WriteAllLines(path, lines);
        }

        public bool CheckCredentials(string login, string passMd5)
        {
            if (credentials == null)
                return false;

            return credentials.Item1.Equals(login) && credentials.Item2.Equals(passMd5);
        }

        public bool CheckSpamFilter(string ip)
        {
            Tuple<DateTime, int> tuple;
            if (authSpamFilter.TryGetValue(ip, out tuple))
            {
                if ( (DateTime.Now - tuple.Item1).TotalHours < 2.0 )
                {
                    if (tuple.Item2 >= 3)
                    {
                        return false;
                    }
                }
                else
                {
                    authSpamFilter.Remove(ip);
                }
            }

            return true;
        }

        public void ClearSpamFilter(string ip)
        {
            authSpamFilter.Remove(ip);
        }

        public void IncrementSpamFilter(string ip)
        {
            Tuple<DateTime, int> tuple;
            if (authSpamFilter.TryGetValue(ip, out tuple))
            {
                tuple = new Tuple<DateTime, int>(DateTime.Now, tuple.Item2 + 1);
            }
            else
            {
                tuple = new Tuple<DateTime, int>(DateTime.Now, 1);
            }

            authSpamFilter[ip] = tuple;
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
