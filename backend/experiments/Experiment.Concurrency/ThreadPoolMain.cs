/*using(var tp = new ThreadPoolImplementation(2))
{

tp.Start();

var task1 = () => { Console.WriteLine("I am one task"); };

var task2 = () => { Console.WriteLine("I am another task"); };

var task3 = () =>
{
    var currentThreadState = Thread.CurrentThread.ThreadState;
    Console.WriteLine($"Inside the thread, the state is {currentThreadState}");
    Console.WriteLine("I am third task");
};

tp.AddWork(task1);
tp.AddWork(task2);
tp.AddWork(task3);
Console.WriteLine("Sleeping for 10 second");
Thread.Sleep(10000);
}*/
