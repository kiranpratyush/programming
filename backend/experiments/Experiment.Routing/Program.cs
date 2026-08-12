namespace Experiment.Routing
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);
            builder.Services.AddHealthChecks();
            var app = builder.Build();

            // Location 1: before routing runs, endpoint is always null here.
            app.Use(async (context, next) =>
            {
                Console.WriteLine($"1. Endpoint: {context.GetEndpoint()?.DisplayName ?? "(null)"}");
                await next(context);
            });

            app.UseRouting();

            // Location 3: runs when this endpoint matches
            app.MapGet("/", (HttpContext context) =>
            {
                Console.WriteLine($"3. Endpoint: {context.GetEndpoint()?.DisplayName ?? "(null)"}");
                return "Hello World!";
            }).WithDisplayName("Hello");

            app.UseEndpoints(_ => { });

            // Location 4: runs after UseEndpoints - will only run if there was no match.
            app.Use(async (context, next) =>
            {
                Console.WriteLine($"4. Endpoint: {context.GetEndpoint()?.DisplayName ?? "(null)"}");
                await next(context);
            });
            app.Run();
        }
    }
}
