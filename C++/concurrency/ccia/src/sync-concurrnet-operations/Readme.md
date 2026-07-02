# Synchronizing the states between the threads 
- One thread waits for another thread to complete.
- Conditional variables and futures.
- The waiting thread will not only wake up upon receiving notify from other thread, but it can also wake up and check the condition sometimes , this is called a spurious wake up .
if the condition checking has some side effects it will lead to indeterminate side effects.
