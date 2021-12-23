using Nancy;
using Nancy.Responses;
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

            Get("/access/check", x =>
            {
                var clientAddress = this.Request.UserHostAddress;
                if (Program.ValidateWhitelistAddress(clientAddress))
                {
                    logger.Info($"[/access/check] Server start check successfull from {clientAddress}");
                    return "Allow";
                }
                else
                {
                    logger.Warn($"[/access/check] Server start check FAILED from {clientAddress}");
                    return "Deny";
                }
            });
        }
    }
}
