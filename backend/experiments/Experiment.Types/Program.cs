using System.Diagnostics;

namespace Experiment.Types
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            var _ = new Channels();
            await _.Run();
        }
    }
}
