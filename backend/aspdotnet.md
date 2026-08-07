TODO: For each concept write a programms and update here
Request pipeline and middleware (done)
Use, Run, Map (done)
middleware execution order (done)
HttpContext(some what done)
request/response lifecycle(some what done)
exception-handling middleware (done)
custom middleware (done)
when the response has started
Routing and endpoints
MapGet, MapPost, etc.
controllers
attribute routing
route parameters
route constraints
MapGroup
endpoint metadata
UseRouting
endpoint selection/execution
URL generation

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
[FromBody]
[FromQuery]
[FromRoute]
[FromHeader]
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