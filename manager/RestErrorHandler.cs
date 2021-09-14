using Nancy;
using Nancy.ErrorHandling;

namespace SyberiaServerManager
{
    public class RestErrorHandler : IStatusCodeHandler
    {
        public void Handle(HttpStatusCode statusCode, NancyContext context)
        {
            context.Response = new Response()
            {
                StatusCode = statusCode
            };
        }

        public bool HandlesStatusCode(HttpStatusCode statusCode, NancyContext context)
        {
            return statusCode != HttpStatusCode.OK;
        }
    }
}
