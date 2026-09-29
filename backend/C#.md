**These all should be second nature**

types (learnt about primitive types int,long,decimal,float,struct,class,records)
variables (done with implict and explicit)
nullable types
operators
loops (done, normal for loop and foreach loop)
methods (inside the function)
overloads (override)
classes (duh)
constructors (duh , primary constructors)
properties (like getters and setters)
access modifiers (public private protected)
static (static methods and classes)
namespaces (organize the code)
Value types vs reference types (assigned in stack vs heap section of memory)
ref, out, in
record, class, struct (Record is a modifier with value equality semantics, use it when you primarily want to store data and two struct is equal if all the properties of it is equal.
record struct is value type means all the properties are copied (not deep copy but shallow copy)
record class is reference type
)
interfaces (Provides a blue print of functionality)
abstract classes (Abstract methods and interfaces)
generics
generic constraints
delegates
Action
Func
lambdas
events
extension methods
attributes
pattern matching
switch expressions
nullable reference types
exceptions
using
async/await
iterators / yield
LINQ
IEnumerable
IAsyncEnumerable
covariance/contravariance
equality semantics
Equals / GetHashCode
records and immutable models
expression trees — later
source generators — much later

## C# concurrency basics (September 1st - 4th)

Implement a ThreadPool in both C# and C++ refer chatgpt to iteratively make it advanced.
[Thread pool implementation.](https://chatgpt.com/c/6a9ed04e-d39c-83ee-bf1a-2a9703bbf09c)
From September 15-20 implement a Thread pool
C# Concurrency topics:
Task / Task<T>
TAP
async / await execution model
I/O-bound vs CPU-bound work
Task.Run
Task.Delay vs Thread.Sleep
Task.WhenAll
Task.WhenAny
Task.WaitAsync
SynchronizationContext
TaskScheduler
ConfigureAwait
CancellationToken
CancellationTokenSource
linked cancellation tokens
timeout cancellation
race conditions
critical sections
atomicity
memory visibility
lock / Monitor
Semaphore
SemaphoreSlim
Mutex
ReaderWriterLockSlim
Interlocked
volatile
deadlock
livelock
starvation
lock ordering
ConcurrentDictionary
ConcurrentQueue
ConcurrentStack
ConcurrentBag
BlockingCollection
Channel<T>
bounded vs unbounded channels
backpressure
producer-consumer pattern
graceful shutdown
exception handling in concurrent workers
bounded parallelism
Parallel.For
Parallel.ForEachAsync
PLINQ
ThreadLocal<T>
AsyncLocal<T>
immutable state
shared mutable state
.NET memory model basics

## Cancellation in Managed Threads - learn by building CancelLab (from https://learn.microsoft.com/en-us/dotnet/standard/threading/cancellation-in-managed-threads)

Mini console app `CancelLab` with menu:
[1] Polling demo (prime finder + IsCancellationRequested)
[2] Throw demo (Task + ThrowIfCancellationRequested + OperationCanceledException)
[3] Callback demo (Register -> CancelPendingRequests / unblock)
[4] WaitHandle demo (ManualResetEventSlim / SemaphoreSlim.Wait(token))
[5] Linked + Timeout demo (CreateLinkedTokenSource + CancelAfter)
[0] Cancel with `c`, quit with `q`

Pattern: `CancellationTokenSource -> Token -> listen (poll/callback/waithandle) -> Cancel() -> Dispose`
- cooperative, not forced - listener decides how to stop gracefully
- requesting distinct from listening - only creator can Cancel()
- one Cancel() call notifies all copies
- linked tokens: `CreateLinkedTokenSource(internal, external)`
- `IsCancellationRequested` can't go back to false - tokens not reusable
- `using var cts` / Dispose required

Phases:
0. Scaffold: `dotnet new console -n CancelLab`, menu loop with `Task.Run(DoWorkAsync(token))`
1. Polling: long loop + SpinWait, check `IsCancellationRequested -> break + cleanup`. Try removing check -> Cancel() does nothing.
2. Throw: `token.ThrowIfCancellationRequested()`, caller `catch (OperationCanceledException ex)` check `ex.CancellationToken`. Task -> Canceled vs Faulted.
3. Register: `token.Register(() => client.CancelPendingRequests())`, fast sync callback, no locks/Dispose-deadlock, no manual thread/SyncContext in callback.
4. WaitHandle: `WaitHandle.WaitAny(new[]{mre, token.WaitHandle}, timeout)`, `mres.Wait(token)`.
5. Linked+Timeout: parent/child `CreateLinkedTokenSource(parentToken)`, `cts.CancelAfter(3000)`, one operation = one token.
6. Library<->User: `MyLibrary.DoWorkAsync(externalToken)` makes internal linkedCts, respects external cancel.

Self-checks: what if never checks token? return/break vs throw? why Cancel() blocks till callbacks finish? how to listen to 2 tokens at once?
