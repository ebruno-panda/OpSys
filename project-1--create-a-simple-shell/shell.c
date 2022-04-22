#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[256];
char delimiters[] = " \t\r\n";
extern char **environ;
pid_t pid;
int status;

int main() {
  // Stores the string typed into the command line.
  char command_line[MAX_COMMAND_LINE_LEN];
  char cmd_bak[MAX_COMMAND_LINE_LEN];
  
  // Stores the tokenized command line input.
  char *arguments[MAX_COMMAND_LINE_ARGS];
  char *env_string;
  unsigned int i;
  
  while(true){
    do { 
      // 0. Modify the prompt to print the current working directory
      if (getcwd(prompt, sizeof(prompt)) == NULL) {
        perror("getcwd() error");
        exit(1);
      }
      else {
        printf("%s>", prompt);
        fflush(stdout);
      }
      // Read input from stdin and store it in command_line. If there's an
      // error, exit immediately. (If you want to learn more about this line,
      // you can Google "man fgets")
      if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
        fprintf(stderr, "fgets error");
        exit(0);
      }
    }
    
    while(command_line[0] == 0x0A);  // while just ENTER pressed
    // If the user input was EOF (ctrl+d), exit the shell.
    if (feof(stdin)) {
      printf("\n");
      fflush(stdout);
      fflush(stderr);
      return 0;
    }
    // TODO:
    // 1. Tokenize the command line input (split it on whitespace)
    arguments[0] = strtok(command_line, delimiters);
    i=0;
    while (arguments[i] != NULL) {
      i++;
      arguments[i] = strtok(NULL, delimiters);
    }
    // 2. Implement Built-In Commands
    if (strcmp(arguments[0], "cd") == 0) {
      chdir(arguments[1]);
    }
    else if (strcmp(arguments[0], "pwd") == 0) {
      printf("%s\n", prompt);
    }
    else if (strcmp(arguments[0], "echo") == 0) {
      i=1;
      while(arguments[i] != NULL){
        if (*arguments[i] == '$'){
          if ((env_string = getenv(arguments[i] +1)) != NULL) {
            printf("%s\n", env_string);
          }
        }
        else {
          printf("%s ", arguments[i]);
        }
        i++;
      }
      printf("\n");
    }
    else if (strcmp(arguments[0], "exit") == 0) {
      exit(0);
    }
    else if (strcmp(arguments[0], "env") == 0) { 
      if (arguments[1] != NULL){
        if ((env_string = getenv(arguments[1])) != NULL) {
          printf("%s\n", env_string);
        }
      }
      else if (arguments[1] == NULL) { //env
        i=1;
        while (environ[i] != NULL) {
          getenv(environ[i]);
          printf("%s\n", environ[i]);
          i++;
          if (environ[i] == NULL) {
            break;
          } 
        }
      }
    }
    else if (strcmp(arguments[0], "setenv") == 0) {
      //parse the arguments[] string first
      arguments[1] = strtok(arguments[1], "=");
      arguments[2] = strtok(NULL, "=");
      //call setenv
      setenv(arguments[1], arguments[2], 1);
    }
    else {
      pid = fork();
      if (pid == 0){
        execvp(arguments[0], arguments); //change as needed
      }
      else if (pid < 0) {
        perror("error in fork\n");
        exit(1);
      }
      else {
        wait(&status); //pass in time or &status
      }
         
      
       }
      
    }
    
    return -1;
}