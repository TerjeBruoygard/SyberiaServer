using Nancy;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SyberiaUpdaterServer
{
    public static class Extensions
    {
        public static string GetSenderIp(this Request request)
        {
            var realIp = request.Headers.FirstOrDefault(x => x.Key.Equals("X-Real-IP"));
            if (realIp.Value != null && realIp.Value.Count() == 1)
            {
                return realIp.Value.First();
            }

            return request.UserHostAddress;
        }

        public static string HtmlEscape(this string str)
        {
            return str.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("\"", "&quot;").Replace("'", "&apos;");
        }
    }
}
