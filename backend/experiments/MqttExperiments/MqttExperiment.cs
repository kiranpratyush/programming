namespace MqttExperiments
{
    using MqttExperiments.Models;
    using ARS.Providers;
    using ARS.Services.Connectivity;
    using Microsoft.Extensions.Logging;
    using System.Text.Json;
    public static class Experiment
    {
        private readonly static int NumberOfThreads = 5;
        private readonly static string region = "us-east-1";

        private readonly static string iotEndpoint = "a1zblse2j9hjhm-ats.iot.us-east-1.amazonaws.com";

        public static void SendStatusToUI(int totalJobsToSend, PubSubServiceFactory factory)
        {
            int jobsPerThread = totalJobsToSend / NumberOfThreads;
            int remainder = totalJobsToSend % NumberOfThreads;

            var tasks = new List<Task>();

            for (int i = 0; i < NumberOfThreads; i++)
            {
                int jobCount = jobsPerThread + (i < remainder ? 1 : 0);
                var task = Task.Run(() =>
                {
                    var pubSubService = factory.GetPubSubService();
                    SendJobsOnThread(jobCount, pubSubService);
                });
                tasks.Add(task);
            }

            Task.WaitAll(tasks.ToArray());
        }

        private static void SendJobsOnThread(int jobCount, IPubSubService pubSubService)
        {
            var jobs = GenerateRandomJobs(jobCount);

            foreach (var job in jobs)
            {
                var jobDataInJson = JsonSerializer.Serialize(job);
                pubSubService.UpdateStatusToUI(jobDataInJson);
            }
        }
        private static void SendIncrementingActiveOnThread(string jobId, int iterationCount, IPubSubService pubSubService)
        {
            var job = new JobInfo
            {
                JobId = jobId,
                OrderId = "1",
                uiNumber = "0",
                SerialNumber = "0",
                MaterialNumber = "0",
            };

            for (int iteration = 1; iteration <= iterationCount; iteration++)
            {
                job.uiNumber = iteration.ToString();
                job.SerialNumber = iteration.ToString();
                job.MaterialNumber = iteration.ToString();
                var jobDataInJson = JsonSerializer.Serialize(job);
                pubSubService.UpdateStatusToUI(jobDataInJson);
            }
        }
        private static IEnumerable<JobInfo> GenerateRandomJobs(int count)
        {
            var jobs = new List<JobInfo>();
            for (int i = 0; i < count; i++)
            {
                jobs.Add(new JobInfo
                {
                    JobId = "1",
                    OrderId = "1",
                    uiNumber = "true",
                    SerialNumber = Guid.NewGuid().ToString(),
                    MaterialNumber = Guid.NewGuid().ToString(),
                });
            }
            return jobs;
        }
        public static async Task Run()
        {
            using ILoggerFactory factory = LoggerFactory.Create(builder => builder.AddConsole());
            var pubsubservicelogger = factory.CreateLogger<PublishSubscribeCommunication>();
            var credentialProviderlogger = factory.CreateLogger<CredentialsProvider>();
            var iotpubsublogger = factory.CreateLogger<IOTPubSub>();

            /* Initialize Credential provider */
            ICredentialsProvider credentialsProvider = new CredentialsProvider(credentialProviderlogger);

            var pubsubFactory = new PublishSubscribeFactory();
            pubsubFactory.Initialize(iotpubsublogger, credentialsProvider, region, iotEndpoint, 443);

            var pubsubservicefactory = new PubSubServiceFactory();
            pubsubservicefactory.Initialize(pubsubFactory, pubsubservicelogger, 2, 2, 1);

            var incrementingTasks = new List<Task>
            {
                Task.Run(() =>
                {
                    var pubSubService = pubsubservicefactory.GetPubSubService();
                    SendIncrementingActiveOnThread("1", 2000, pubSubService);
                }),
                Task.Run(() =>
                {
                    var pubSubService = pubsubservicefactory.GetPubSubService();
                    SendIncrementingActiveOnThread("2", 2000, pubSubService);
                }),
            };

            await Task.WhenAll(incrementingTasks);
        }
    }
}