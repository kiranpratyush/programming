#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include<stdlib.h>

void signal_handler(int signal_number)
{
    printf("Handled the signal");
    return;
}

int main() {
    signal(SIGINT,signal_handler);

    // block the sigint signal
    // create a signal
    sigset_t mask,prev_mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);
    // do some large work here
    printf("Blocking of CNTRL+C started \n");
    for(int i=0;i<10;i++)
    {
        printf("sleeping for 1 second\n");
        sleep(1);
        printf("Finshed sleeping for 1 second\n");
    }
    printf("End of CNTRL+C\n");
    // restore the previous mask
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);
    return 0;
}