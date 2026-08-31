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

C# Concurrency topics:

Thread vs ThreadPool
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
