using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace Experiment.Auth.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class SampleController : ControllerBase
    { 

        [HttpGet()]
        [Authorize]
        public async  Task<IActionResult> Hello()
        {
            return Ok("Hello");
        }
    }
}
