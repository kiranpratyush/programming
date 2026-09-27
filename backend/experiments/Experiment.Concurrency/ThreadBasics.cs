namespace Experiment.Concurrency
{

    public class ThreadBasics
    {

        public void InstanceMethod()
        {
            Console.WriteLine("InstanceMethod running in a separate thread");
            Thread.Sleep(3000);
            Console.WriteLine("The instance method called by the worker thread has ended");
        }

        public static void StaticMethod()
        {

            Console.WriteLine("StaticMethod is running on another thread");
            Thread.Sleep(5000);
            Console.WriteLine("The static method called by the worker thread has ended");
        }

    }
    public class ThreadWithState
    {

        private string _boilerPlate;
        private int _numberValue;

        public ThreadWithState(string text, int number)
        {

            _boilerPlate = text;

            _numberValue = number;

        }

        public void ThreadProc()
        {

            Console.WriteLine(_boilerPlate, _numberValue);

        }

    }

    public class ThreadWithStateAndCallBack{

       public delegate void ExampleCallBack();

        private string _boilerPlate;

        private int _numberValue;

        private ExampleCallBack _callback;

        

        public ThreadWithStateAndCallBack(string text,int number,ExampleCallBack callback){

            _boilerPlate = text;
            _numberValue = number;
            _callback = callback;
        }

        public void ThreadProc(){
            Console.WriteLine("Thread proc is executing");
            _callback.Invoke();

        }
    }
}
