using Microsoft.AspNetCore.Identity;

namespace Experiment.Auth.Models
{
    public class User:IdentityUser
    {
        public string FirstName { get; set; } = string.Empty;

        public string LastName { get; set; } = string.Empty;

    }
    public class RegisterDto
    {
        public required string Email { get; set; }

        public  required string Password { get; set; }

        public required string FirstName { get; set; }


        public required string LastName { get; set; }


    }
}
