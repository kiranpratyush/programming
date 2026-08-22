using Experiment.Auth.Repository;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddDbContext<ApplicationDbContext>(
    options => options.UseInMemoryDatabase("AppDb")
);
builder.Services.AddAuthorization();
builder.Services.AddIdentityApiEndpoints<IdentityUser>(
)
       .AddEntityFrameworkStores<ApplicationDbContext>();

var app = builder.Build();

app.MapIdentityApi<IdentityUser>();

app.MapGet("/", () => "Hello World!").RequireAuthorization();

app.MapPost("/logout", async (SignInManager<IdentityUser> signInManger, [FromBodyAttribute] object? empty) =>
{
    if(empty != null)
    {
        await signInManger.SignOutAsync();
        return Results.Ok(empty);
    }
    return Results.Unauthorized();
});

app.Run();
