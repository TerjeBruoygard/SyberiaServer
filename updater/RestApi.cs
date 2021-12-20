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
            Get("/update/database", x => {
                var accessKey = this.Request.Query["accessKey"];
                if (Program.ValidateAccessKey(accessKey))
                {
                    logger.Warn("[/update/database] Success download from " + this.Context.Request.UserHostAddress);
                    string filename = "SyberiaDatabase.zip";
                    string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", filename);
                    return new GenericFileResponse(path, this.Context).AsAttachment(filename);
                }

                logger.Warn("[/update/database] Invalid AccessKey from " + this.Context.Request.UserHostAddress);
                return new NotFoundResponse();
            });

            Get("/update/pbo", x => {
                var accessKey = this.Request.Query["accessKey"];
                if (Program.ValidateAccessKey(accessKey))
                {
                    logger.Warn("[/update/pbo] Success download from " + this.Context.Request.UserHostAddress);
                    string filename = "SyberiaServer.pbo";
                    string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", filename);
                    return new GenericFileResponse(path, this.Context).AsAttachment(filename);
                }

                logger.Warn("[/update/pbo] Invalid AccessKey from " + this.Context.Request.UserHostAddress);
                return new NotFoundResponse();
            });
        }
    }
}
