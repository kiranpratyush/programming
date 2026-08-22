using Experiment.Auth.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;

namespace Experiment.Auth.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class SampleController : ControllerBase
    {
        private readonly UserManager<User> _userManager;
        public SampleController(UserManager<User> userManager)
        {
            _userManager = userManager;

        }

        [HttpGet()]
        [Authorize]
        public async  Task<IActionResult> Hello()
        {
            return Ok("Hello");
        }
        [HttpPost]
        public async Task<IActionResult> Register([FromBody] RegisterDto payload)
        {
            if (!ModelState.IsValid) { return BadRequest(ModelState); }
            var user = new User
            {
                UserName = payload.Email,
                Email = payload.Email,
                FirstName = payload.FirstName,
                LastName = payload.LastName
            };
            var result = await _userManager.CreateAsync(user, payload.Password);
            if (result.Succeeded) { return Ok(new { message = "User Registered Successfully" }); }
            return BadRequest(result.Errors);
        }
    }
}
