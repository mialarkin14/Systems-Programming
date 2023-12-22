#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child;
    pid_t child1;
    int exitStatus;

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }
    
    // TODO
    child = fork();

    if (child < 0)
    {
        // If the fork was unsuccessful print out an error and exit
        perror("fork()");
        exit(EXIT_FAILURE); 
    }
    if (child == 0)
    {
        // If we are in the child, run command and exit
        int status;
        status = execlp(argv[1], argv[1], argv[2], NULL);
        if (status < 0)
        {
            // If execlp fails, print out an error
            perror("execlp()");
        }
        exit(-1);
    }

    else
    {
         if (waitpid(child, &exitStatus, 0) > 0) 
		{ 
            if (WIFEXITED(exitStatus) && !WEXITSTATUS(exitStatus)) 
            {
                printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WEXITSTATUS(exitStatus));
            }
        }
    }

    child1 = fork();
    
    if (child1 < 0)
    {
        // If the fork was unsuccessful print out an error and exit
        perror("fork()");
        exit(EXIT_FAILURE); 
    }
    if (child1 == 0)
    {
        // If we are in the child, run command and exit
        int status1;
        status1 = execvp(argv[3], &argv[3]);
        if (status1 < 0)
        {
            // If execlp fails, print out an error
            perror("execvp()");
        }
        exit(-1);
    }

    else
    {
         if (waitpid(child1, &exitStatus, 0) > 0) 
		{ 
            if (WIFEXITED(exitStatus) && !WEXITSTATUS(exitStatus)) 
            {
                printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WEXITSTATUS(exitStatus));
            }
        }
    }
    
    //execvp(argv[3], )
    return 0;
}