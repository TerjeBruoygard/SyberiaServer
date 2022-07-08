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
                logger.Info($"[/echo] Success");
                return "Success";
            });

            Get("/whitelist/add", x => {
                try
                {
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
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/whitelist/add] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });

            Get("/whitelist/remove", x => {
                try
                {
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
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/whitelist/remove] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });

            Get("/update/database", x => {
                try
                {
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
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/update/database] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });

            Get("/update/pbo", x => {
                try
                {
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
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/update/pbo] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });

            Post("/access/check", x =>
            {
                try
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
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/access/check] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });

            Get("/statistic", x =>
            {
                try
                {
                    var currentDate = DateTime.Now;
                    var accessKey = this.Request.Query["accessKey"];
                    var from = this.Request.Query["from"]?.Value as string;
                    var to = (this.Request.Query["to"]?.Value as string) ?? $"{currentDate.Day}.{currentDate.Month}.{currentDate.Year}";
                    if (Program.ValidateStatisticKey(accessKey) && !string.IsNullOrEmpty(from))
                    {
                        var statData = Program.GetServiceStatistic(from, to);
                        if (statData != null)
                        {
                            var uniqueLines = new Dictionary<string, Tuple<DateTime, DateTime>>();
                            foreach (var stat in statData)
                            {
                                foreach (var line in stat.Value)
                                {
                                    var parts = line.Split('|');
                                    var htmlLine = $"<td>{parts[1]}</td><td>{{PERIOD}}</td><td>{parts[2]}</td><td>{parts[3]}</td><td>{parts[4]}</td><td>{parts[5]}</td>";
                                    if (uniqueLines.ContainsKey(htmlLine))
                                    {
                                        if (stat.Key < uniqueLines[htmlLine].Item1)
                                        {
                                            uniqueLines[htmlLine] = new Tuple<DateTime, DateTime>(stat.Key, uniqueLines[htmlLine].Item2);
                                        }

                                        if (stat.Key > uniqueLines[htmlLine].Item2)
                                        {
                                            uniqueLines[htmlLine] = new Tuple<DateTime, DateTime>(uniqueLines[htmlLine].Item1, stat.Key);
                                        }
                                    }
                                    else
                                    {
                                        uniqueLines.Add(htmlLine, new Tuple<DateTime, DateTime>(stat.Key, stat.Key));
                                    }
                                }
                            }

                            var result = new StringBuilder();
                            result.AppendLine("<!DOCTYPE html>");
                            result.AppendLine("<html>");
                            result.AppendLine("<head>");
                            result.AppendLine("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">");
                            result.AppendLine("</head>");
                            result.AppendLine("<body>");
                            result.AppendLine($"<h1>Syberia instances list: {from}-{to}</h1>");
                            result.AppendLine("<br>");
                            result.AppendLine("<table class=\"table\">");
                            result.AppendLine("<thead><tr><th scope=\"col\">IP</th><th scope=\"col\">Period</th><th scope=\"col\">DB Port</th><th scope=\"col\">Web Panel Port</th><th scope=\"col\">DayZ Server Path</th><th scope=\"col\">Service Path</th></tr></thead>");
                            result.AppendLine("<tbody>");
                            foreach (var line in uniqueLines)
                            {
                                result.AppendLine($"<tr>{line.Key}</tr>".Replace("{PERIOD}", $"{line.Value.Item1.Day}.{line.Value.Item1.Month}.{line.Value.Item1.Year} - {line.Value.Item2.Day}.{line.Value.Item2.Month}.{line.Value.Item2.Year}"));
                            }
                            result.AppendLine("</tbody>");
                            result.AppendLine("</table>");
                            result.AppendLine("</body>");
                            result.AppendLine("</html>");
                            return new TextResponse(result.ToString(), "text/html", Encoding.UTF8);
                        }
                    }

                    return new NotFoundResponse();
                }
                catch (Exception ex)
                {
                    logger.Error(ex, "[/statistic] exception");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }
            });
        }
    }
}
