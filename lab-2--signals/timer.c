#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int takencareof;
int alarmCount = 0;
time_t empezar;
time_t finito;

void handler(int sig)
{ //Ctrl-C at keyboard
  alarmCount += 1;
  takencareof = 1;
  printf("Hello World!\n"); 
  sleep(3);

//   exit(1); //exit after printing
}

void finalizer(int sig){
  time_t timerec;
  finito = time(NULL);
  timerec = finito - empezar;
  printf("Alarms occurrred: %d\n", alarmCount);
  printf("Run time is: %d\n", sig), (int)timerec;
  exit(0);
}

int main(int argc, char * argv[])
{
  empezar = time(NULL);
  SIGNAL(SIGALARM,handler); //register handler to handle SIGALRM
  signal(SIGINT,finalizer); 
  while(1){ //busy wait for signal to be delivered
    takencareof = 0;
    alarm(1);
    while(takencareof == 0);
    printf("Turing was right!\n");        
  } 
  return 0; //never reached
}