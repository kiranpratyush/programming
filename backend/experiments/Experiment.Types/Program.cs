using System.Diagnostics;

namespace Experiment.Types
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var person = new Person("Pratyush", "Kiran");
            var temperature = new Temperature(12.0);
            // No destructive mutation
            var modified = temperature with { Fahrenheit = 20 };

            var (first, last) = person;
            var celcium = modified;
        }
    }
}
