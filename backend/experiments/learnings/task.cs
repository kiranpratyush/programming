/* Task completion Source 
Which scenario you can use it 
1. Want to manually handle Task setting up the value when something finishes
2. Caller waits on completion of Task
*/

int value = 0;
Interlocked.Increment(ref value);
Interlocked.Increment(ref value);
Console.WriteLine(value);