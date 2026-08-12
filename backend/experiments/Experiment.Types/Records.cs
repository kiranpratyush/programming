using System;
using System.Collections.Generic;
using System.Text;

namespace Experiment.Types
{
    public record Person(string FirstName, string SecondName);
    public record struct Coordinate(double latitude, double longitude);

    public  record class Temperature(double Celcius)
    {
        public double Fahrenheit
        {
            get
            {
                return Celcius * 9.0 / 5.0 + 32.0;
            }
            set;
        }
    }
}
