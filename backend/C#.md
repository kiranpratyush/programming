# ▶️ Next (resume here)

Last update: 2026-10-12

## How I learn now: build first, read on demand, write it back

The topic lists below are a **backlog**, not a curriculum. Pick an item only when the thing you are building needs it.

Every session:

1. **Recall (10 min, notes closed):** answer the previous session's questions in writing, then check them.
   A pool of thread which takes some action to perform and perform it and returns back the result to caller.The current implementation of threadpool has multiple states : Running,Started,stopping etc. It uses locker to wait if the task is full and then with Monitor.PulseAll it wakes up all the threads. one draw back here is that Monitor.PulseAll here wakes up all the thread.
2. **Build:** one small task. **Write your prediction before you run anything.**
3. **Read only when stuck:** read just the doc section that unblocks you. 20 minutes at most.
4. **Break it:** cause one failure on purpose and explain why it happened.
5. **Write (15 min):** add a session log at the bottom of this file: _expected / happened / why_, in your own words. Claude checks it and doesn't write it.

## Status

| Item                                    | State                                             |
| --------------------------------------- | ------------------------------------------------- |
| Thread basics, ThreadPool               | Done (`experiments/Experiment.Concurrency`)       |
| CancelLab (cancellation)                | ▶️ **now**. `CancellationToken.cs` is still empty |
| Channel<T> producer-consumer + shutdown | Next                                              |
| `yield return` by hand                  | After that                                        |

## ▶️ Session 1 (2026-10-12): CancelLab phases 0–3

Build in a new console project, `experiments/CancelLab` (`dotnet new console -n CancelLab`). The phase details are in the "Cancellation in Managed Threads" section at the bottom of this file. **Don't copy from the docs page. Open it only when you're stuck.**

**0. Recall (notes closed):** write 3 lines from memory. How does your ThreadPool hand work to a worker thread? How does it shut down? What happens to a job that's still in the queue when you shut down?

**1. Phase 0, scaffold:** a menu loop. `c` cancels the running demo and `q` quits. Each demo runs as `Task.Run(() => Demo(token))`. Create a new `CancellationTokenSource` for every demo run.

- Predict first: if you press `c` and then start a second demo with the **same** CTS, what happens?

**2. Phase 1, polling:** a prime finder that checks `token.IsCancellationRequested` and then does `break` plus cleanup (print how many primes it found).

- Predict: how long after you press `c` does the loop actually stop? What sets that delay?
- **Break it:** remove the check. Does `Cancel()` stop anything? Write down why ("cooperative" in one sentence of your own).

**3. Phase 2, throw:** switch to `token.ThrowIfCancellationRequested()`. The caller `await`s the task inside `try/catch (OperationCanceledException)`.

- Predict: what is `task.Status` after cancel? Write `Canceled` or `Faulted` **before** you print it.
- **Break it (a):** throw `new OperationCanceledException()` **without** passing the token. Predict, then check the status.
- **Break it (b):** pass a _different_ token to `Task.Run(..., otherToken)` than the one you throw with. Predict, then check the status.

**4. Phase 3, Register callback:** call `token.Register(...)` to cancel a slow `HttpClient` call (`https://httpbin.org/delay/10`) with `client.CancelPendingRequests()`. Then do the same thing more simply by passing the token straight into `GetAsync`.

- Predict: which thread does the callback run on? Print `Environment.CurrentManagedThreadId` in the callback and in the code that calls `Cancel()`.
- **Break it:** put `Thread.Sleep(3000)` inside the callback. What happens to the code that called `cts.Cancel()`? Why?

**5. Write (in the session log at the bottom), 5–8 sentences:**

- Polling vs throwing: when would you use each one in a backend worker?
- Why a cancelled task ends up `Canceled` instead of `Faulted`, and what decides it.
- Why `Cancel()` waits for the callbacks to finish, and what that means for the code inside a callback.
- One thing that surprised you.

**Done when:** all three demos work from the menu, every prediction is written down next to its result, and the log entry has been checked.

## Coming sessions

- **Session 2:** CancelLab phases 4–6: WaitHandle, linked + timeout (`CreateLinkedTokenSource`, `CancelAfter`), and a library that respects the caller's token. Write an answer to each self-check question.
- **Session 3:** a bounded `Channel<T>` producer-consumer. Predict and observe backpressure when the queue is full. Add graceful shutdown with a token: finish jobs already in the queue vs drop them. Add exception handling in the worker. (This becomes the Kafka consumer worker later.)
- **Session 4:** write an `IEnumerable`/`IEnumerator` state machine by hand for a `yield return` method, then compare with the compiler's output in SharpLab.

---

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

## Cancellation in Managed Threads - learn by building CancelLab (from https://learn.microsoft.com/en-us/dotnet/standard/threading/cancellation-in-managed-threads) (Done)

Cancel is a request, not a kill. The task must notice it and throw OperationCanceledException(token) to end as Canceled. Register callbacks should only signal. Dispose the CTS only after everything using it has finished.
“don’t dispose a registration while holding a lock the callback needs”, but you don’t need to reproduce it now.

Mini console app `CancelLab` with menu: ea

- cooperative, not forced - listener decides how to stop gracefully
- requesting distinct from listening - only creator can Cancel()
- one Cancel() call notifies all copies
- linked tokens: `CreateLinkedTokenSource(internal, external)`
- `IsCancellationRequested` can't go back to false - tokens not reusable
- `using var cts` / Dispose required

Phases: 0. Scaffold: `dotnet new console -n CancelLab`, menu loop with `Task.Run(DoWorkAsync(token))`

1. Polling: long loop + SpinWait, check `IsCancellationRequested -> break + cleanup`. Try removing check -> Cancel() does nothing.
2. Throw: `token.ThrowIfCancellationRequested()`, caller `catch (OperationCanceledException ex)` check `ex.CancellationToken`. Task -> Canceled vs Faulted.
3. Register: `token.Register(() => client.CancelPendingRequests())`, fast sync callback, no locks/Dispose-deadlock, no manual thread/SyncContext in callback.
4. WaitHandle: `WaitHandle.WaitAny(new[]{mre, token.WaitHandle}, timeout)`, `mres.Wait(token)`.
5. Linked+Timeout: parent/child `CreateLinkedTokenSource(parentToken)`, `cts.CancelAfter(3000)`, one operation = one token.
6. Library<->User: `MyLibrary.DoWorkAsync(externalToken)` makes internal linkedCts, respects external cancel.

Self-checks: what if never checks token? return/break vs throw? why Cancel() blocks till callbacks finish? how to listen to 2 tokens at once?

## Session log

### Session 1 (2026-10-12): CancelLab phases 0–3

_Recall answers:_

_Predictions → results:_

_Explain back (own words):_
