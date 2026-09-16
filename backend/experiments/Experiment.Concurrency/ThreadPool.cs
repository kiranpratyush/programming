namespace Experiment.Concurrency
{
    public class ThreadPoolImplementation : IDisposable
    {
        private enum LifecycleState
        {
            Created,
            Running,
            Stopping,
            Stopped,
            Disposed
        }

        private readonly int _capacity;
        private readonly Thread[] _threads;
        private readonly Queue<Action> _tasks = new();
        private readonly object locker = new();
        private LifecycleState _state = LifecycleState.Created;

        public ThreadPoolImplementation(int numThreads,int capacity)
        {
            _threads = new Thread[numThreads];
            _capacity = capacity;
            for (int i = 0; i < numThreads; i++)
            {
                var _thread = new Thread(Worker) { IsBackground = true };
                _threads[i] = _thread;
            }
        }
        public bool AddWork(Action work)
        {
            
            lock (locker)
            {
                if (_state != LifecycleState.Running) return false;

                while(_tasks.Count == _capacity){
                    Monitor.Wait(locker);
                }
                _tasks.Enqueue(work);
                Monitor.PulseAll(locker);
            }
            return true;
        }
        public void Start()
        {
            lock (locker)
            {
                ObjectDisposedException.ThrowIf(_state == LifecycleState.Disposed, this);
                if (_state != LifecycleState.Created)
                {
                    throw new InvalidOperationException("The thread pool can only be started once.");
                }

                _state = LifecycleState.Running;
                foreach (var thread in _threads)
                {
                    thread.Start();
                }
            }
        }
        public void Stop()
        {
            bool shouldJoin;
            lock (locker)
            {
                shouldJoin = _state is LifecycleState.Running or LifecycleState.Stopping;

                if (_state == LifecycleState.Created)
                {
                    _state = LifecycleState.Stopped;
                }
                else if (_state == LifecycleState.Running)
                {
                    _state = LifecycleState.Stopping;
                }

                Monitor.PulseAll(locker);
            }

            if (shouldJoin)
            {
                JoinWorkers();

                lock (locker)
                {
                    if (_state == LifecycleState.Stopping)
                    {
                        _state = LifecycleState.Stopped;
                    }
                }
            }
        }

        private void Worker()
        {
            while (true)
            {
                Action task;
                lock (locker)
                {
                    while (_tasks.Count == 0 && _state == LifecycleState.Running)
                    {
                        Monitor.Wait(locker);
                    }
                    if (_state != LifecycleState.Running && _tasks.Count == 0)
                    {
                        return;
                    }
                    task = _tasks.Dequeue();
                    Monitor.PulseAll(locker);
                }
                try
                {

                    task();

                }
                catch (Exception)
                {
                    Console.WriteLine("Exception happenned");
                }
            }
        }
        public void Dispose()
        {
            bool shouldJoin;
            lock (locker)
            {
                if (_state == LifecycleState.Disposed)
                {
                    return;
                }

                shouldJoin = _state is LifecycleState.Running or LifecycleState.Stopping;
                _state = LifecycleState.Disposed;
                Monitor.PulseAll(locker);
            }

            if (shouldJoin)
            {
                JoinWorkers();
            }

            GC.SuppressFinalize(this);
        }

        private void JoinWorkers()
        {
            foreach (var thread in _threads)
            {
                if (thread != Thread.CurrentThread)
                {
                    thread.Join();
                }
            }
        }
    }
}
