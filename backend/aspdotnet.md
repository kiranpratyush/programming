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
status codes (This is just mapping the status code Typed Status code)
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
- During compile time or by using reflection the raw delegate type is converted to the RequestDelegate Type.
- Security Sensitive operations (Password change,email change) needs to invalidate the existing cookies and token.
- SecurityStampValidator checks every 30 mins (configurable) checks if the security stamp matches the cookie

What I understood:
Asp.net core identity provides endpoints and api configuration to handle token and cookie based authentication and authorization.
I have integrated with EFCore the implementation to support register,login and then protect the end points.
I am going to ignore the Scaffold Identity (because it provides the screen of the login etc.)

Add custom userdata to identity to register (Done)
You can create a custom User inheriting from IdentityUser, and then use it with userManager to do the register with the backing store . Same as the curn flow.

Identity and EF core migrations : This is just keeping your database models with the code models in sync.

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

Without ASP.NET Core Identity, AddAuthentication() gives me the authentication mechanism, but I must build my own user store, user/role management, credential validation, and logic for constructing the ClaimsPrincipal. With ASP.NET Core Identity, Microsoft provides that user/role/credential infrastructure and integrates it with ASP.NET Core authentication.


### Understand about the Cookie and then create a simple endpoint for login with out ASP.NET core identity. (Done)
Server adds the cookie data in the response header, and browser stores it and sends it in subsequent requests.
Cookie can be a session cookie or persistent cookie (which is defined my Expires or Max-Age)
Server can define Domain which tells that on which domain the cookies will be attached.
Some attributes secure,httponly are there to make sure where cookies are available.
cookies prefixes are browser level constraints embedded in cookie name.

Authentication produces a `ClaimsPrincipal`.

Authorization evaluates that `ClaimsPrincipal` against policies/requirements.

**`ClaimsPrincipal` is the central authentication representation**

  * Contains one or more `ClaimsIdentity` objects.
  * Claims can represent name, role, permissions, tenant, etc.
  * `HttpContext.User` is the `ClaimsPrincipal` available to the application.

* **Cookie Authentication**

  * `SignInAsync()` takes your `ClaimsPrincipal`.
  * The cookie authentication handler creates an `AuthenticationTicket`.
  * The ticket contains the principal and authentication properties.
  * The ticket is protected and written into the authentication cookie.

* **The browser stores the authentication state**

  * You don't need to store the `ClaimsPrincipal` in your database for basic cookie authentication.
  * The browser sends the cookie on subsequent applicable requests.
  * The server reconstructs the `ClaimsPrincipal` from the cookie.

* **ASP.NET Core Data Protection**

  * Provides cryptographic protection for data.
  * Cookie authentication uses it to protect the authentication ticket.
  * It uses a **key ring** containing cryptographic keys.
  * The server needs access to the appropriate keys to unprotect cookies.
  * In multi-instance production deployments, instances generally need a shared/persistent key ring.

* **Cookie validation is not the same as user validation**

  * Data Protection establishes that the protected ticket can be successfully unprotected and has not been tampered with.
  * It does **not** inherently ask the database whether the user is still allowed to exist.
  * Therefore, a cookie can be cryptographically valid while the user's account state has changed.
  * One implementation is Security stamp check to invalidate the cookie

* **Cookie expiration**

  * The authentication ticket can have an expiration time.
  * Once expired, the cookie should no longer authenticate the request.

* **Logout**

  * `SignOutAsync()` tells the browser to remove the authentication cookie.
  * This handles normal logout.
  * It does not necessarily invalidate a stolen/copied cookie that is still cryptographically valid.

* **Immediate revocation is a different problem**

  * To revoke an already-issued cookie, the server needs some way to know that the cookie/user/session has been revoked.
  * Examples: security stamps, session IDs, token/session versions, revocation stores, etc.

* **ASP.NET Core Identity**

  * Identity is much larger than cookie authentication.
  * It provides infrastructure for:

    * Users
    * Passwords/password hashing
    * Roles
    * Claims
    * User stores
    * Lockout
    * Email confirmation
    * Password reset
    * Security stamps
    * Sign-in management
  * Identity ultimately still works with `ClaimsPrincipal` and authentication mechanisms.

* **Without Identity**

  * You are responsible for things such as:

    * Finding the user.
    * Validating credentials.
    * Constructing claims.
    * Managing roles/claims.
    * Deciding how users are stored.
    * Implementing revocation if required.
  * Then you can give the resulting `ClaimsPrincipal` to cookie authentication.

* **With Identity**

  * `UserManager` handles user-related operations.
  * `SignInManager` handles sign-in-related operations.
  * `RoleManager` handles roles.
  * Identity can construct/manage the authentication state instead of you implementing all of that infrastructure yourself.

* **`[Authorize]`**

  * Runs during the authorization stage, after authentication has established `HttpContext.User`.
  * It normally evaluates claims/roles/policies against that principal.
  * `[Authorize(Roles = "Administrator")]` can therefore work directly from the role claim.

* **Database-backed authorization**

  * If authorization depends on current database state, you can implement a custom authorization requirement/handler.
  * Example:

    ```text
    Is authenticated
        +
    Has Administrator role
        +
    Does database say user can modify Order 123?
    ```
  * The custom `AuthorizationHandler` can query the database and call `context.Succeed(...)` when the requirement is satisfied.

* **Security stamp**

  * Identity maintains a security stamp representing relevant security state for a user.
  * Authentication state can contain the stamp.
  * Identity's security-stamp validator can compare the authenticated state with the user's current security stamp from the Identity store.
  * The default validation interval is typically **30 minutes**, so this is periodic rather than a database check on every request.
  * Changing the security stamp can therefore invalidate existing authentication state when the validator detects the mismatch.

* **The overall architecture to remember**

```text
                    ASP.NET Core Identity
                 ┌─────────────────────────┐
                 │ Users                   │
                 │ Passwords               │
                 │ Roles                   │
                 │ Claims                  │
                 │ Security stamps         │
                 │ SignInManager           │
                 │ UserManager             │
                 └───────────┬─────────────┘
                             ↓
                       ClaimsPrincipal
                             ↓
                    Authentication
                             ↓
                 CookieAuthenticationHandler
                             ↓
                    AuthenticationTicket
                             ↓
                    Data Protection
                             ↓
                       Key Ring
                             ↓
                          Cookie
                             ↓
                         Browser
```

The most useful distinction to retain is:

**Identity manages the user's security/account state.**

**Authentication establishes the `ClaimsPrincipal`.**

**Cookie Authentication transports that authentication state between requests.**

**Data Protection cryptographically protects the authentication ticket.**

**Authorization decides whether the authenticated principal is allowed to perform an operation.**

**Custom authorization handlers are where application-specific authorization logic, including database-backed rules, can be plugged in.**




0. Configure identity for SPA and first with inmemory set up (Done)
1. Configure identity with a postgres database (Once have a basic understanding of EF CORE)
2. TODO check EFCORE and inspect what is the actual tables are getting created.
3. TODO configure cookie authentication first and understand where the identity core fits into (Done)

- Identity core handles the infrastructure with the database like users , available roles , claims etc.
- Cookie handler can only parse the cookie using the dataprotection api so that it is not tampered . Remember in multi instance application when load balancer is presnet, a set of key ring needs to be deployed so that verification can be done.
- From the cookie it can parse the claims . later point of time we can use the claims or data and do something with database to handle all these things.

Understand about OpenID Connect:
what is OpenID connect and OAuth ?

Resource owner :
Client:
Authorization server/Resource server:
Back to redirect URI
Exchange authorization code for access token
Scope and consent
why code not just direct token :
AccessToken
Id Token always JWT

How the authentication process works here ?

Understand about JWT Tokens:
JWT contains three parts
Header.Payload.Signature
Header: encoded in base64 format, contains metadata about the cryptographic algorithms used to secure its contents.
Claim formats of JWT
different key value pairs of reserved claim are present for oauth
like iss , aud when actually implementing it check what are the fields can be verified. (no need to go deep into this right now as it won't provide much return here.)

Delegated Authorization : When a user specific token is used to call between application API it is called delegated authorization.
for example I get a token and call api A with that token , api A uses this token to call api B (Api A is acting on behalf of myself).

Token Types:
Do not use your own token types.
let's say
User -> Application (Client) --> Backend
One way of having token is
Application specific, Application asks for the token from Backend and then stores it. (application token)
Another way is application can asks the token on behalf of user and store it user wise. Use this token to call to backend while doing the call. (This is delegated access token)

Bearer Token VS Sender constrained access tokens

- Bearer token means who ever posses the token can access it
- Sender constrained access tokens: Sender has to provide extra proof along with token to verify that the token is originating from the authorized client , not whoever posseses it.

ID Tokens : Contains claims of user information , it is always in JWT format. Don't ever use this token to authorize the backend.

**WWW-Authenticate header field (Section 11.6.1) containing at least one challenge applicable to the target resource. (When only get time read it no hurry)**

What is PKCE ? (TODO)

https://www.youtube.com/watch?v=5FrA0UzV1Aw

Backend for frontend security architecture (BFF) : Key idea is don't store the access token in browser storage , in one way is to store the jwt token in the backend , and use cookies in browser.

Implement one OIDC/ OAuth flow in ASP.NET core

## Authorization 

Roles: What groups a claims principal is part of 
Claims : What the user principal claims to be 
Policy : What are the requirements to be authorized (multiple claims and Roles)

## Some security topics 
### CSRF attacks (CROSS SITE REQUEST FORGERY ATTACKS)
- First understand how does csrf works and try to build a small prototype of it.














### After all these understand TLS part and try to understand How TCP connections happen with Wireshark.
