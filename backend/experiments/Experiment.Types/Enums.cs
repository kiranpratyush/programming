using System;
using System.Collections.Generic;
using System.Text;

namespace Experiment.Types
{
   enum Season
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

    public class EnumLearning
    {
        public string DescribeSeason(Season season)
        {
            return season switch
            {
                Season.Sprint => "blah"
            };
        }
    }
    

}
