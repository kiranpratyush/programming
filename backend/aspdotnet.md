TODO: For each concept write a programms and update here
Request pipeline and middleware (done)
Use, Run, Map (done)
middleware execution order (done)
HttpContext(some what done)
request/response lifecycle(each request goes through the series of middleware where the request and response object
is modified and finally the response is sent)
exception-handling middleware (UseExceptionhandling,here you can provide a path where the path will be called during exception)
custom middleware (You can write custom middleware by implmenting invokeAsync methods)
when the response has started (You can check by Response.HasStarted , in that case you can not add anything more to the response body or header)
Routing and endpoints (Routing finds out the actual method needs to be called and the useEndpoint actually calls the method)
MapGet, MapPost, etc. (This is just way of mapping your request path to endpoint executables)
controllers (controller is also a way of providing endpoints)
attribute routing ([HttpGet],[HttpPost] [HttpHead] etc)
route parameters ({id} can be denoted as route parameters, use routerparameter to identify a particular resource not,incase of choosing some subset of it you can use query params)
route constraints (do not use route constraint for validation , which sends 404 not found in case of validation failure, in this case we should send 400 Bad reqeust)
MapGroup (This just creates a separate branch of mapping the request to a group)
endpoint metadata (You can add extra information to endpoint)
UseRouting (This is a middleware which actually resolves the request to which endpoint needs to be called, remember this does not call the actual callback, useEndpoint calls these)
endpoint selection/execution (This is what useRouting does , useEndpoint calls it )
URL generation ()

Controllers and API design
[ApiController]
ControllerBase
ActionResult<T>
status codes
headers
query/path/body parameters
REST resource design
PUT vs PATCH
idempotency
pagination
filtering/sorting
Model binding and validation
[FromBody] (This binds the payload from Body)
[FromQuery] (This binds the parameter from Query params)
[FromRoute] (This binds the parameter from path params)
[FromHeader] (This binds the parameter from header params)
DataAnnotations
automatic 400 responses
custom validation
DTOs vs entities
Dependency injection
AddSingleton
AddScoped
AddTransient
constructor injection
service lifetimes
captive dependencies
scopes
IServiceProvider
factory registration

This should become almost automatic.

Configuration
appsettings.json
environment-specific configuration
environment variables
IConfiguration
Options pattern
IOptions<T>
IOptionsSnapshot<T>
IOptionsMonitor<T>
secrets
Logging
ILogger<T>
structured logging
logging scopes
log levels
correlation IDs
avoiding sensitive data in logs
Authentication
JWT bearer authentication
claims
authentication schemes
OpenID Connect basics
OAuth2 concepts
access token vs ID token
Authorization
[Authorize]
roles
claims-based authorization
policies
requirements and handlers
resource-based authorization
Error handling
global exception handling
ProblemDetails
validation errors
mapping domain exceptions → HTTP responses

Along with Postgres:

EF Core
DbContext
entities
configuration
migrations
LINQ queries
tracking/no-tracking
relationships
transactions
concurrency
indexes
Include
projections
N+1 problem
HTTP clients
HttpClient
IHttpClientFactory
typed clients
timeouts
cancellation
retries
connection pooling
resilience
Background processing
IHostedService
BackgroundService
worker services
graceful shutdown
cancellation tokens
queues using Channel<T>
Caching
IMemoryCache
distributed cache
Redis concepts
cache invalidation
cache-aside pattern
File handling
streaming uploads
streaming downloads
avoiding loading huge files into RAM
multipart requests
cancellation
API versioning and documentation
OpenAPI
Swagger
versioning strategies
compatibility
Testing ASP.NET
unit tests
integration tests
WebApplicationFactory
test database
mocking external dependencies
testing middleware/controllers/endpoints
Production concerns
health checks
readiness/liveness
rate limiting
CORS
forwarded headers
HTTPS
request limits
graceful shutdown
reverse proxy behavior
estrel basics

## Authentication and Authorization

Authentication:
Authentication is a process of identifying a user.
`IAuthenticationService` handles this process which is used in useAuthentication middleware
Backend service can support multiple ways the authentication can be carried out . In real life a user can show may be aadhar card, or voter id card to prove it's identity . The ways authentication can be carried out is known as schemes.

scheme can refer to a handler and the options to configure that specific instance of handler.

- Authentication Scheme
- Authentication Handler
- Remote Authenticatin Handler
- 
- Authentication Tickets contains the claimsprincipal
- Authenticate method in Authentication Handler
- ChallengeAsync method if a unauthenticated user tries to access a resource
- ForbidAsync() method if an user is not authorized to access a resource

-> write a simple custom authentication handler.

OIDC vs ASP.net core identity

OIDC usually uses token (Jwt) based to do auth
Cookie is generally simple to implement

Explore the Asp.net core identity

- write cookie based first
- jwt based
### Understanding 
- Asp.net core identity provides set of apis to handle authentication and authorization
- It exposes set of api
  - /login
  - /register
- It also provides Cookies,Token (not JWT) for authentication and authorization.
- Delegate is the delegate type, Minimal API can take arbitrary function and it tries to get the value from the DI,Request Body,Header etc.
- During compile time or by using reflection the  raw delegate type is converted to the RequestDelegate Type.
- Security Sensitive operations (Password change,email change) needs to invalidate the existing cookies and token.
- SecurityStampValidator checks every 30 mins (configurable) checks if the security stamp matches the cookie

What I understood: 
Asp.net  core identity provides endpoints and api configuration to handle token and cookie based authentication and authorization.
I have integrated with EFCore the implementation to support register,login and then protect the end points.
I am going to ignore the Scaffold Identity (because it provides the screen of the login etc.)

Add custom userdata to identity to register (Done)
You can create a custom User inheriting from IdentityUser, and then use it with userManager to do the register with the backing store . Same as the curretn flow.

Identity and EF core  migrations : This is just keeping your database models with the code models in sync.

Identity model is not a single giant model(class) it simply tells the umbrella of the models.
Like.
User <--- It can be one table 
Role <--- It can be another table 
UserClaims <--- It can be another table 

Other details can be understood later. (So skipping)

Understand that a claimsPrincipal (User)
can contain multiple claimsIdentity
each claimsIdentity can contain multiple claims (key value pairs)

The SchemeHandler does the parsing,updating the response context etc
The identity core is a full infrastructure which manages the login register etc.


### Using ASP.NET core identity set up initial auth  set up 
0. Configure identity for SPA and first with inmemory set up (Done)
1. Configure identity with a postgres database
2. TODO check EFCORE and inspect what is the actual tables are getting created.
3. TODO configure cookie authentication first and understand where the identity core fits into