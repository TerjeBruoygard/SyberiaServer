using Nancy;
using SyberiaDatabase;
using System.IO;
using System.Text;

namespace SyberiaServerManager
{
    public class RestApi : NancyModule
    {
        public RestApi()
        {
            Post("/{dbName}/query", x => {
                var body = new StreamReader(this.Request.Body, Encoding.UTF8).ReadToEnd();
                return DatabasePool.Instance.GetOrCreate(x.dbName).Query(body);
            });

            Post("/{dbName}/queryNoStrict", x => {
                var body = new StreamReader(this.Request.Body, Encoding.UTF8).ReadToEnd();
                return DatabasePool.Instance.GetOrCreate(x.dbName).QueryNoStrict(body);
            });

            Post("/{dbName}/transaction", x => {
                var body = new StreamReader(this.Request.Body, Encoding.UTF8).ReadToEnd();
                return DatabasePool.Instance.GetOrCreate(x.dbName).Transaction(body);
            });
        }
    }
}
