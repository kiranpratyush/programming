using System;
using System.Diagnostics;
using OpenTelemetry;
using OpenTelemetry.Resources;
using OpenTelemetry.Trace;
namespace otelConsole
{
    class Program
    {
        private static readonly ActivitySource MyActivitySource = new ActivitySource(
            "otel console simple application");
        static void Main(string[] args)
        {
            // Configure the OpenTelemetry TracerProvider
            using var tracerProvider = Sdk.CreateTracerProviderBuilder()
                .SetResourceBuilder(ResourceBuilder.CreateDefault().AddService("getting-started"))
                .AddSource("MyCompany.MyProduct.MyLibrary")
                .AddConsoleExporter()
                .Build();

            // Start an activity (span) with some tags (attributes)
            using (var activity = MyActivitySource.StartActivity("SayHello"))
            {
                // Set some attributes on the activity
                activity?.SetTag("foo", 1);
                activity?.SetTag("bar", "Hello, World!");
                activity?.SetTag("baz", new int[] { 1, 2, 3 });

                // Set the status of the activity
                activity?.SetStatus(ActivityStatusCode.Ok);

                // Do some work...
                Console.WriteLine("Hello World!");
            }

            Console.WriteLine("Trace has been exported. Press any key to exit.");
            Console.ReadKey();
        }
    }
}
