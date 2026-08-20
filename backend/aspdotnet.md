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
Kestrel basics