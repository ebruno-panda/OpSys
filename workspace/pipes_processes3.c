#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char **argv) {
  int pipefd[2];
	int pipefd2[2];
  int pid, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
	char *sort_args[] = {"sort", NULL};

  // make a pipe
	if (pipe(pipefd) == -1) { 
			fprintf(stderr, "Pipe Failed" ); 
			return 1; 
	} 
	if (pipe(pipefd2) == -1) { 
			fprintf(stderr, "Pipe Failed" ); 
			return 1; 
	} 

	pid = fork();

  if (pid == 0) {
		  
			pid2 = fork();
			if (pid2 == 0) {
				// sorting via child 2 
				// replace regular input with input part of pipe
				dup2(pipefd2[0], 0);

				// close the pipes
				close(pipefd[0]);
				close(pipefd[1]);
				close(pipefd2[0]);
				close(pipefd2[1]);

				// sort
				execvp(sort_args[0], sort_args);

			} else {
				// child handles grep here

				dup2(pipefd[0], 0); // replace regular input with input part of pipe
				dup2(pipefd2[1], 1); // replace regular output with output part of pipe
				
				// close all the pipes
				close(pipefd[0]);
				close(pipefd[1]);
				close(pipefd2[0]);
				close(pipefd2[1]);

				// execute grep
				execvp(grep_args[0], grep_args);
			}
    }
  else {
      // parent gets here and handles "cat scores"
      // replace standard output with output part of pipe
      dup2(pipefd[1], 1);

      // close pipes
			close(pipefd[0]);
			close(pipefd[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);

      // cat happens here
      execvp(cat_args[0], cat_args);
    }
}