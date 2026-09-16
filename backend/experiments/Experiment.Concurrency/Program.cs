namespace Experiment.Concurrency{

    public class Simple{

        public static void Main(){

            var instance = new ThreadBasics();
            Thread  thread = new(new ThreadStart(instance.InstanceMethod));
            thread.Start();
            Console.WriteLine("Main thread calls this after starting instance method");

            Thread staticThread = new(new ThreadStart(ThreadBasics.StaticMethod));

            staticThread.Start();

            Console.WriteLine("The Main thread calls this after starting the static method");

            ThreadWithState ts = new ThreadWithState("Hello World",2);

            Thread threadWithState = new(new ThreadStart(ts.ThreadProc));
            threadWithState.Start();
            threadWithState.Join();
            Console.WriteLine("Independent task is completed, Main thread ends");

            var x = ()=>{Console.WriteLine("I am callback");};
            var threadWithStateAndCallback = new ThreadWithStateAndCallBack("Hello",5,new ThreadWithStateAndCallBack.ExampleCallBack(x));
            var threadWithCallback = new Thread(new ThreadStart(threadWithStateAndCallback
                        .ThreadProc));
            threadWithCallback.Start();
            threadWithCallback.Join();
        }

    }


}
