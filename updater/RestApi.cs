using Nancy;
using Nancy.Responses;
using Newtonsoft.Json.Linq;
using NLog;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SyberiaUpdaterServer
{
    public class RestApi : NancyModule
    {
        private static Logger logger = LogManager.GetCurrentClassLogger();

        public RestApi()
        {
            Get("/echo", x =>
            {
                return "Success";
            });

            Get("/whitelist/add", x => {
                var clientAddress = this.Request.UserHostAddress;
                var accessKey = this.Request.Query["accessKey"];
                var addressToAdd = this.Request.Query["address"];
                if (Program.ValidateMasterKey(accessKey, clientAddress))
                {
                    if (Program.AddToWhitelist(addressToAdd))
                    {
                        logger.Info($"[/whitelist/add] Add ip address {addressToAdd} to whitelist requested from {clientAddress}");
                        return "Success";
                    }
                    else
                    {
                        return "IP address already in whitelist";
                    }
                }

                return new NotFoundResponse();
            });

            Get("/whitelist/remove", x => {
                var clientAddress = this.Request.UserHostAddress;
                var accessKey = this.Request.Query["accessKey"];
                var addressToAdd = this.Request.Query["address"];
                if (Program.ValidateMasterKey(accessKey, clientAddress))
                {
                    if (Program.RemoveFromWhitelist(addressToAdd))
                    {
                        logger.Info($"[/whitelist/remove] Remove ip address {addressToAdd} from whitelist requested from {clientAddress}");
                        return "Success";
                    }
                    else
                    {
                        return "IP address not found in whitelist";
                    }
                }

                return new NotFoundResponse();
            });

            Get("/update/database", x => {
                var clientAddress = this.Request.UserHostAddress;
                var accessKey = this.Request.Query["accessKey"];
                if (Program.ValidateAccessKey(accessKey, clientAddress))
                {
                    logger.Info($"[/update/database] Success download from {clientAddress}");
                    string filename = "SyberiaDatabase.zip";
                    string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", filename);
                    return new GenericFileResponse(path, this.Context).AsAttachment(filename);
                }

                return new NotFoundResponse();
            });

            Get("/update/pbo", x => {
                var clientAddress = this.Request.UserHostAddress;
                var accessKey = this.Request.Query["accessKey"];
                if (Program.ValidateAccessKey(accessKey, clientAddress))
                {
                    logger.Info($"[/update/pbo] Success download from {clientAddress}");
                    string filename = "SyberiaServer.pbo";
                    string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", filename);
                    return new GenericFileResponse(path, this.Context).AsAttachment(filename);
                }

                return new NotFoundResponse();
            });

            Post("/access/check", x =>
            {
                var clientAddress = this.Request.UserHostAddress;
                if (Program.ValidateWhitelistAddress(clientAddress))
                {
                    var bodyData = new StreamReader(this.Request.Body).ReadToEnd();
                    var bodyObj = Newtonsoft.Json.JsonConvert.DeserializeObject(bodyData) as JObject;
                    if (bodyObj != null)
                    {
                        Program.AddServiceStartToStatistic(clientAddress, bodyObj);
                        logger.Info($"[/access/check] Server start check successfull from {clientAddress}:{bodyObj["dbPort"].Value<int>()}");
                        return "Allow";
                    }
                }

                logger.Warn($"[/access/check] Server start check FAILED from {clientAddress}");
                return "Deny";
            });

            Get("/statistic", x =>
            {
                var accessKey = this.Request.Query["accessKey"];
                var date = this.Request.Query["date"];
                if (Program.ValidateStatisticKey(accessKey) && !string.IsNullOrEmpty(date))
                {
                    string[] lines = Program.GetServiceStatistic(date);
                    if (lines != null)
                    {
                        var uniqueLines = new HashSet<string>();
                        foreach (var line in lines)
                        {
                            var parts = line.Split('|');
                            var htmlLine = $"<tr><td>{parts[1]}</td><td>{parts[2]}</td><td>{parts[3]}</td><td>{parts[4]}</td><td>{parts[5]}</td></tr>";
                            if (!uniqueLines.Contains(htmlLine))
                            {
                                uniqueLines.Add(htmlLine);
                            }
                        }

                        var result = new StringBuilder();
                        result.AppendLine("<!DOCTYPE html>");
                        result.AppendLine("<html>");
                        result.AppendLine("<head>");
                        result.AppendLine("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">");
                        result.AppendLine("</head>");
                        result.AppendLine("<body>");
                        result.AppendLine($"<h1>Running servers at {date}</h1>");
                        result.AppendLine("<br>");
                        result.AppendLine("<table class=\"table\">");
                        result.AppendLine("<thead><tr><th scope=\"col\">IP</th><th scope=\"col\">DB Port</th><th scope=\"col\">Web Panel Port</th><th scope=\"col\">DayZ Server Path</th><th scope=\"col\">Service Path</th></tr></thead>");
                        result.AppendLine("<tbody>");
                        foreach (var line in uniqueLines)
                        {
                            result.AppendLine(line);
                        }
                        result.AppendLine("</tbody>");
                        result.AppendLine("</table>");
                        result.AppendLine("</body>");
                        result.AppendLine("</html>");
                        return new TextResponse(result.ToString(), "text/html", Encoding.UTF8);
                    }
                }

                return new NotFoundResponse();
            });
        }
    }
}
