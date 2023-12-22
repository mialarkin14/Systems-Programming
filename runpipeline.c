#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

// Search TODO to find the functions you need to work on

#define MAX_NUM_PROGRAMS 10
#define FD_STDIN    0
#define FD_STDOUT   1
#define PIPEFD_READ     0
#define PIPEFD_WRITE    1

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
} 

void check_pointer(void *p)
{
    if (p == NULL)
       die("Pointer is NULL.");
}

// structure for storing program arguments
typedef struct program_tag {
    char**   argv;      // array of pointers to arguments
    int      argc;      // number of arguments
    int	     pid;	    // process ID of the program
    int      fd_in;     // FD for stdin
    int      fd_out;    // FD for stdout
} Program;

/* start a program. This function does not wait for the child process to 
 * finish.
 *
 * This function needs to fork, redirect stdin/stdout, and then upgrade
 * the child process into the program specified.
 *
 * If pipes are not created earlier, create pipes.
 *
 * Close file descriptors that are not needed, in both parent and child processes.
 *
 * If a child process is created successfully, save its pid in the
 * corresponding Program structure. 
 *
 * If any system call fails, call die() to exit.  
 * For example, die("fork() failed."); 
 * It will be messy if some programs are already started.
 *
 * Parameters:
 *      programs: an array of Program structures.
 *      num_programs: number of programs in the array.
 *      cur: the index of the program that needs to be started in this call.
 *
 * Return value: 
 *      This function does not return a value.  
 *
 * The child process never returns from this function.
 */

void start_program(Program *programs, int num_programs, int cur) 
{
    // Fork 
    pid_t process1 = fork();
    // If the fork failed:
    if (process1 < 0)
    {
        die("fork() failed");
    }
    // If we are in the child
    if (process1 == 0)
    {
        // If the program's stdin does not equal a negative number
        if (programs[cur].fd_in > 0)
        {
            // Set the program's input to the stdin
            int y = dup2(programs[cur].fd_in, 0);
            // If dup2 fails:
            if (y < 0)
            {
            die("dup2() program's input to stdin failed");
            }
            
        }
        
        // If the program's stdout does not equal a negative number
        if (programs[cur].fd_out > 0)
        {
            // Set the program's output to stdout in
            int x = dup2(programs[cur].fd_out, 1);
            // If dup2 fails:
            if (x < 0)
            {
            die("dup2() program's output to stdout failed");
            }
        }
        // Run the program's arguements
        execvp(programs[cur].argv[0], programs[cur].argv);
        // If we get here, there is an error so return an error and exit
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Set the program's pid to the pid from forking
        programs[cur].pid = process1;
        // Close all uneeded FDs
        close(programs[cur].fd_in);
        close(programs[cur].fd_out);
    }
}

/* Wait on a program. 
 *
 * Parameter:
 *  prog:   A pointer to a Program, which has the pid of the program,
 *          which is set when the child process is created.
 *
 * Return value:
 *  -1:     prog->pid is less than 0, or waitpid() returns error.
 *  0-255:  exit value of the program (after WEXITSTATUS()).
 *
 * */
int wait_on_program(Program *prog)
{
    int exitStatus;

    if (prog->pid < 0)
        return -1;

    // TODO
    // Need to get the pid of the program
    pid_t child = prog->pid;
    // Wait on the child process 
    waitpid(child, &exitStatus, 0);
    return WEXITSTATUS(exitStatus);
}

/* This function creates pipes to be used for connecting two pipeline stages.
 * You can create all pipes here,
 * or you can create pipes when they are needed in start_program().
 * If you decide to create pipes in start_program(),
 * leave this function empty.
 *
 * Paramters:
 *  programs: an array of Program structures.
 *  num_programs: number of programs in the array.
 *
 * Return value:
 *  This function does not return a value. If pipe() fails, 
 *  call the following function to exit from the program.
 *       die("pipe() failed.");
 */
void prepare_pipes(Program *programs, int num_programs)
{
    // TODO
    // There are multiple programs so we need to use a for loop
    // Set up a pipe
    int pipe_start[2];

    // For each program, we need to create a pipe for them
    for (int i = 0; i < num_programs; i ++)
    {
        // Create a variable for each pipe and test if it fails
        int each_pipe = pipe(pipe_start);
        if (each_pipe < 0)
        {
            die("pipe() failed");
        }
        // If the pipe did not fail, get each programs stdin and stdout and set it to the pipe
        // Setting the programs stdout to the write end of the pipe 
        programs[i].fd_out = pipe_start[1]; 
        // Setting the next programs stdin to the read end of the pipe
        programs[i+1].fd_in = pipe_start[0];
    }
    // Set the first program's in to stdin
    programs[0].fd_in = 0;
    // Set the last program's out to stdout
    programs[num_programs-1].fd_out = 1;
    }

/*********************************************************/
/* Do not change code below                              */
/*********************************************************/
/* initialize a Program structure that allows argc arguments */
void init_program(Program *prog, int argc)
{
    // allocate memory for array of arguments
    prog->argv = malloc((argc + 1) * sizeof(char *));
    check_pointer(prog->argv);
    prog->argc= 0;
    prog->argv[0] = NULL;
    prog->pid = prog->fd_in = prog->fd_out = -1;
}

/* free the memory used by prog */
void free_programs(Program *programs, int num_programs)
{
    for (int i = 0; i <  num_programs; i ++) {
        free(programs[i].argv);
        programs[i].argv = NULL;
    }
}

// parse the command line arguments
// split them into programs
// return the number of programs found
int parse_command_line(Program *progs, int max_num_progs, int argc, char **argv)
{
    int     cur_arg = 1;
    int     prog_count = 0;

    while (cur_arg < argc) {
        // get arguments for a new program

        if (prog_count == max_num_progs) 
            die("Too many programs.");

        // allocate space for argv for progs[prog_count]
        // Could have scanned the arguments and find out how many arguments
        // the current program has. 
        init_program(&progs[prog_count], argc);

        int     ia = 0;  // index of arguments for progs[prog_count]
        while (cur_arg < argc && strcmp(argv[cur_arg], "--")) {
            progs[prog_count].argv[ia++] = argv[cur_arg++];
        }
        progs[prog_count].argv[ia] = NULL;
        progs[prog_count].argc = ia;
        prog_count ++;
        if (ia == 0) 
            die("Empty program.");

        cur_arg ++; // skip "--"
        if (cur_arg == argc) 
            die("Last program is empty."); 
    }

    return prog_count;
}

int main(int argc, char **argv)
{
    Program programs[MAX_NUM_PROGRAMS];
    int     num_programs;

    if (argc <= 1) 
        die ("Specify at least one program to run. Multiple programs are separated by --");

    // Prepare programs and their arguments
    num_programs = parse_command_line(programs, MAX_NUM_PROGRAMS, argc, argv); 

    // Prepare pipes
    prepare_pipes(programs, num_programs);

    // spawn children
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Starting program %d:%s\n", i, programs[i].argv[0]);
        start_program(programs, num_programs, i);
    }

    // wait for children
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Waiting for program %d:%s\n", i, programs[i].argv[0]);
        int status = wait_on_program(&programs[i]);
        fprintf(stderr, "Program %d:%s exited with %d\n", i, programs[i].argv[0], status);
    }

    // free memory
    free_programs(programs, num_programs);

    // You should see the message only once
    fprintf(stderr, "Parent: Everything is good.\n");
    return 0;
}
