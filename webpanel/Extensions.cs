using Nancy;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SyberiaWebPanel
{
    public static class Extensions
    {
        public static Response NoCache(this Response response)
        {
            response.Headers.Add("Cache-Control", "no-cache, no-store, must-revalidate");
            response.Headers.Add("Pragma", "no-cache");
            response.Headers.Add("Expires", "0");
            return response;
        }

        public static void  Replace(this StringBuilder builder, string from, int to)
        {
            builder.Replace(from, to.ToString());
        }

        public static void Replace(this StringBuilder builder, string from, float to)
        {
            builder.Replace(from, to.ToString().Replace(',', '.'));
        }

        public static void Replace(this StringBuilder builder, string from, IEnumerable<string> to)
        {
            builder.Replace(from, string.Join("\r\n", to));
        }

        public static void Replace(this StringBuilder builder, string from, IEnumerable<float> to)
        {
            builder.Replace(from, string.Join("; ", to.ToString().Replace(',', '.')));
        }

        public static void Replace(this StringBuilder builder, string from, IEnumerable<int> to)
        {
            builder.Replace(from, string.Join("; ", to.ToString()));
        }

        public static void ReplaceScriptObject(this StringBuilder builder, string from, object to)
        {
            var jsObject = Newtonsoft.Json.JsonConvert.SerializeObject(to, Newtonsoft.Json.Formatting.None);
            builder.Replace(from, jsObject);
        }
    }
}

public class ScriptObjectSerializableAttribute : Attribute
{

}
