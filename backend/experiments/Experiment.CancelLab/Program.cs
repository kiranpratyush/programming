namespace Experiment.CancelLab
{
    internal static class Program
    {
        static async Task Main(string[] args)
        {
            var count = 0;
            var runs = new Dictionary<int, (Task Task, CancellationTokenSource Cts)>();

            Console.WriteLine("d = start task | c <n> = cancel task n | s = status | q = cancel all and quit");

            while (true)
            {
                var line = Console.ReadLine()?.Trim().ToLowerInvariant();
                if (string.IsNullOrEmpty(line)) continue;

                if (line == "d")
                {
                    var id = count++;
                    var cts = new CancellationTokenSource();
                    var task = Task.Run(() => DoSomeWork($"task {id}", cts.Token), cts.Token);
                    runs[id] = (task, cts);
                    Console.WriteLine($"Started task {id}");
                }
                else if (line.StartsWith("c ") && int.TryParse(line[2..], out var n))
                {
                    if (runs.TryGetValue(n, out var run))
                    {
                        run.Cts.Cancel();
                        Console.WriteLine($"Cancel requested for task {n}");
                    }
                    else Console.WriteLine($"No task {n}");
                }
                else if (line == "s")
                {
                    foreach (var (id, run) in runs)
                        Console.WriteLine($"task {id}: {run.Task.Status}");
                }
                else if (line == "q")
                {
                    foreach (var run in runs.Values) run.Cts.Cancel();
                    break;
                }
                else Console.WriteLine("Unknown command");
            }

            try
            {
                await Task.WhenAll(runs.Values.Select(r => r.Task));
            }
            catch (OperationCanceledException)
            {
                Console.WriteLine("At least one task was cancelled");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                foreach (var (id, run) in runs)
                {
                    Console.WriteLine($"final task {id}: {run.Task.Status}");
                    run.Cts.Dispose();   // safe: every task has finished by now
                }
            }
        }

        static void DoSomeWork(string taskName, CancellationToken token)
        {
            try
            {
                token.Register(() =>
                {
                    Console.WriteLine($"callback on thread {Environment.CurrentManagedThreadId}");
                    Thread.Sleep(3000);
                    
                 });
                Console.WriteLine($"[{taskName}] started");
                for (int i = 0; i < 10_000_000; i++)   // longer, so you have time to cancel
                {
                    token.ThrowIfCancellationRequested();
                    Thread.SpinWait(500);
                }
                Console.WriteLine($"[{taskName}] finished normally");
            }
            finally
            {
                Console.WriteLine($"[{taskName}] work-method finally ran");
            }
        }
    }
}