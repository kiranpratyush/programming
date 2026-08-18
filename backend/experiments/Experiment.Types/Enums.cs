using System;
using System.Collections.Generic;
using System.Text;

namespace Experiment.Types
{
   public enum Season
    {
        Sprint,
        Summer,
        Autumn,
        Winter
    }
    enum HttpStatus: ushort
    {
        OK = 0,
        NotFound = 404,
        InternalServerError = 500
    }

    public static class EnumLearning
    {
        public static string DescribeSeason(Season season)
        {
            return season switch
            {
                Season.Sprint => "blah"
            };
        }
    }
    

}
