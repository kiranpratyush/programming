
namespace Experiment.Middleware
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.

            builder.Services.AddControllers();
            // Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
            builder.Services.AddOpenApi();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.MapOpenApi();
            }
            app.Map("/map1", HandleMap1);
            app.Map("/map2", HandleMap2);
            app.UseExceptionHandler((configure) =>
            {

            });
            app.Run(async context =>
            {
                await context.Response.WriteAsync("Hello from the non-Map delegate!");
            });

            app.UseRouting();

            app.UseHttpsRedirection();

            app.UseAuthorization();

            

            app.MapControllers();


            app.Run();
        }
        private static void HandleMap1(IApplicationBuilder app)
        {
            app.Run(async context =>
            {
                await context.Response.WriteAsync("Map 1");
            });
        }

        private static void HandleMap2(IApplicationBuilder app)
        {
            app.Run(async context =>
            {
                await context.Response.WriteAsync("Map 2");
            });
        }
    }
}
