#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#define MAX_COUNT 200
#define BUF_SIZE  100

void  main(void)
{
     pid_t  pid;
     int procid;
     char buf[BUF_SIZE];

     fork();
     pid = getpid();
     for (procid = 1; procid <= MAX_COUNT; procid++){
       sprintf(buf, "This line is from child, value = %d\n", pid, procid);
       write(1, buf, strlen(buf));

     }
          
}
