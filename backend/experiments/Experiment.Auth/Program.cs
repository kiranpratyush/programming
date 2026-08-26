using Experiment.Auth.Models;
using Experiment.Auth.Repository;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllers();
builder.Services.AddAuthentication(CookieAuthenticationDefaults.AuthenticationScheme).AddCookie();
//builder.Services.AddDbContext<ApplicationDbContext>(
//    options => options.UseInMemoryDatabase("AppDb")
//);
//builder.Services.AddAuthorization();
//builder.Services.AddIdentity<User, IdentityRole>(options =>
//{
//}).AddEntityFrameworkStores<ApplicationDbContext>();

var app = builder.Build();

//app.MapIdentityApi<User>();

app.MapGet("/", () => "Hello World!").RequireAuthorization();

//app.MapPost("/logout", async (SignInManager<User> signInManger, [FromBodyAttribute] object? empty) =>
//{
//    if(empty != null)
//    {
//        await signInManger.SignOutAsync();
//        return Results.Ok(empty);
//    }
//    return Results.Unauthorized();
//});
app.UseAuthentication();
app.UseAuthorization();
app.MapControllers();

app.Run();
