using Nancy;
using System.IO;
using System.Text;

namespace SyberiaWebPanel
{
    public class RestApi : NancyModule
    {
        public RestApi()
        {
            Get("/", x => {
                return "Web panel now in development";
            });
        }
    }
}
