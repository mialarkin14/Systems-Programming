#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

// search TODO to find the place you need to work on

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

#define MAX_VALUE       1000
#define MSG_BUF_SIZE    100
#define PIPEFD_READ 0
#define PIPEFD_WRITE 1

typedef  struct {
    int v;
    int num_attempts;
    char message[MSG_BUF_SIZE]; 
} gmn_t;

void gmn_init(gmn_t *pg, int seed)
{
    srand(seed);
    pg->v = rand() % MAX_VALUE + 1;
    pg->num_attempts = 0;
    strcpy(pg->message, "You have not got the right number yet.\n");
} 

int gmn_check(gmn_t *pg, int guess)
{
    pg->num_attempts ++;
    if (pg->v == guess) {
        snprintf(pg->message, MSG_BUF_SIZE, 
                "It took you %d attempt(s) to guess the number %d.\n", 
                pg->num_attempts, pg->v);  
        return 0;
    }
    if (pg->v > guess)
        return 1;
    return -1;
}

char * gmn_get_message(gmn_t *pg)
{
    return pg->message;
}

int    gmn_get_max()
{
    return MAX_VALUE;
}

// this function runs the demo session
// all gmn_ functions should be called in child process 
// and then send the result to the parent process
void guess_my_number(int seed)
{
    gmn_t gmn;

    // initialize the game
    gmn_init(&gmn, seed);

    int min = 1;
    int max = gmn_get_max();
    int result;

    do {
        // make a guess
        int guess = (min + max)/2;
        printf("My guess: %d\n", guess);

        // check
        result = gmn_check(&gmn, guess);

        // if not correct, prepare for the next guess
        if(result > 0) 
            min = guess + 1;
        else if(result < 0)
            max = guess - 1;
    } while (result != 0);

    // print out the final message
    fputs(gmn_get_message(&gmn), stdout);
}


// Tasks for the child process. 
// 
// Parameters:
//  fdp:    the pipe for parent to write and for child to read 
//  fdc:    the pipe for child to write and for parent to read
//  seed:   seed for the random numer
//
// This function should not return.
// This function does not print any characters, except for error messages.
void    child_main(int fdp[], int fdc[], int seed)
{
    gmn_t   gmn;

    gmn_init(&gmn, seed);

    // TODO
    //  close unused file descriptors
    close(fdp[1]);      // Close the writing for the child
    close(fdc[0]);      // Close the reading for the child 
    //  send max value to the parent 
    int max = gmn_get_max();
    int a = write(fdc[1], &max, sizeof(int));
    if (a < 0)
    {
        die("write failed");
    }
    //  repeat the following until guess from parent is correct 
    //      wait for a guess from parent 
    //      call gmn_check() 
    //      send the result to parent
    int x = 0;
    int guess = 0;
    do
    {
        int b = read(fdp[0], &guess, sizeof(int));
        if (b < 0)
        {
        die("read failed");
        }
        x = gmn_check(&gmn, guess);
        int c = write(fdc[1], &x, sizeof(int));
        if (c < 0)
        {
        die("write failed");
        }
    } while(x != 0);
    //  send the final message back (as a string) 
    char message1[MSG_BUF_SIZE];
    strcpy(message1, gmn_get_message(&gmn));
    int d = write(fdc[1], message1, MSG_BUF_SIZE);
    if (d < 0)
    {
    die("write failed");
    }

    //  close all pipe file descriptors
    close(fdp[0]);
    close(fdc[1]);

    exit(EXIT_SUCCESS);
}

void    print_file_descriptors(int child);

int main(int argc, char *argv[])
{
    int seed = 3100;
    int demo = 0;

    // parse the command line arguments

    for (int i = 1; i < argc; i ++) {
        if (! strcmp(argv[i], "demo")) {
            demo = 1;
        } else if (! strcmp(argv[i], "-h")) {
            fprintf(stderr, "Usage: %s [<seed>] [demo]\n", argv[0]);
            return 1;
        } else {
            seed = atoi(argv[i]);
            if (seed <= 0)
                die("seed must be a postive number.");
        }
    }

    if (demo) {
        guess_my_number(seed);
        exit(0);
    }

    // Now, we do it using two processes
    // The child generates a random number
    // The parent tries to guess the number.
    // The child tell the parent

    // two pipes
    // fdp : parent writes
    // fdc : child writes
    
    int fdp[2], fdc[2];

    //pipe creation
    if (pipe(fdp) == -1)
        die("pipe() failed.");

    if (pipe(fdc) == -1)
        die("pipe() failed.");

    pid_t pid;
    pid = fork();

    if (pid < 0)
        die("fork() failed.");

    if(pid == 0)
        child_main(fdp, fdc, seed); // never returns
    
    // parent continues
    
    int min = 1;
    int max;
    int guess;
    int result;

    // TODO
    //      close unused pipe file descriptor
    close(fdp[0]);
    close(fdc[1]);
    //      get max from the child
    read(fdc[0], &max, sizeof(int));
    
    do { 
        guess = (min + max)/2;
        printf("My guess: %d\n", guess);

        // TODO
        //     send guess to the child
        int y = write(fdp[1], &guess, sizeof(int));
        if (y < 0)
        {
            die("write failed");
        }
        //     wait for the result from the child
        int x = read(fdc[0], &result, sizeof(int));
        if (x < 0)
        {
            die("read failed");
        }
        
        if (result > 0)
            min = guess + 1;
        else if (result < 0)
            max = guess - 1;
    } while (result != 0);

    // flush stdout buffer
    fflush(stdout);

    // TODO
    //      receive the final message and print it to stdout
    char message[MSG_BUF_SIZE];
    read(fdc[0], message, MSG_BUF_SIZE);
    printf("%s", message);
    
    //      close all pipe file descriptors
    close(fdp[1]);
    close(fdc[0]);
    //wait for the child process to finish
    wait(NULL);
    return 0;
}

