#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

/*************  macros related to random numbers for MT ****************/
// Use functions in POSIX.1-2001.

// Use the following macro in your thread function before any calls to get random values.
#define RANDOM_INIT(x)  (g_random_buffer[0]=3100,g_random_buffer[1]=(x), g_random_buffer[2] = (x) >> 16)

// RANDOM_INT() returns a random number uniformly distributed between [0, 2^31)
#define RANDOM_INT() (nrand48(g_random_buffer))

#define THINKING() my_msleep(RANDOM_INT()%10)

// MAKE_CHOICE() returns a random number in [0, 1, 2]
#define MAKE_CHOICE() (nrand48(g_random_buffer) % 3)

// the meaning of values 0, 1, 2
enum {C_ROCK, C_PAPER, C_SCISSORS};

// search TODO to find the place you need to work on

static void die(char *s)
{
    if (errno)
        perror(s);
    else 
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

static void check_pthread_return_f(int rv, int lineno)
{
    char buf[80];
    if (rv == 0)
        return;
    strerror_r(rv, buf, sizeof(buf));
    fprintf(stderr, "Error: %s(line %d): %s", __FILE__, lineno, buf); 
    exit(EXIT_FAILURE);
}

#define check_pthread_return(rv)    check_pthread_return_f((rv), __LINE__)

#define NUM_PLAYERS  2

void my_msleep(int r)
{
    // limit to 100ms
    struct timespec req = {0, (r % 100 + 1) * 1000000};

    nanosleep(&req, NULL);
}

// status of the value
// you can add more states if you like
// S_INIT: not ready
// S_READY: value is ready
enum {S_INIT, S_READY};

typedef struct {
    int     value;
    // three fields for syncing between producer and consumer
    // only one producer and one consumer
    int     status;
    pthread_mutex_t mutex;
    pthread_cond_t cond;        
} shared_int_t;

// we use barrier here
typedef struct {
    int     value;
    pthread_barrier_t barrier;
} result_t;

// Data for referee 
typedef struct {
    int n_rounds;           // number of rounds
    shared_int_t *choice1, *choice2; 
    result_t *result;
    int opt_quiet;
} referee_arg_t;

// Data for player
typedef struct {
    int id;                 // id is 1 or 2
    int seed;
    int n_rounds;           // number of rounds
    int opt_quiet;

    shared_int_t    *choice;
    result_t *result;

    int n_wins;
    int n_ties;
} player_arg_t;

// Player thread main function.
void * thread_player(void *arg_in)
{
    unsigned short g_random_buffer[3]; // buffer for random numbers
    player_arg_t *arg = arg_in;

    shared_int_t * choice = arg->choice;
    result_t * result = arg->result;

    // seed is different for threads
    RANDOM_INIT(arg->seed);

    // reset the counter
    arg->n_ties = arg->n_wins = 0;
    
    for (int i = 0; i < arg->n_rounds; i ++ ) {
        // Add THINKING() at different locations to test
        // THINKING();

        int r = MAKE_CHOICE(); 
        int outcome = 0;

        if (! arg->opt_quiet)
            printf("Player %d: round %d: %d\n", arg->id, i, r);

        // TODO
        //  send choice r to referee
        // Lock the mutex
        pthread_mutex_lock(&choice->mutex);
        // Set the choice to r
        choice->value = r;
        // Signal to the referee that the choices are ready
        choice->status = S_READY;
        pthread_cond_signal(&choice->cond);
        // Unlock the mutex
        pthread_mutex_unlock(&choice->mutex);

        //Wait for all players to get here
        pthread_barrier_wait(&result->barrier);

        // There is now a result from the referee so save it in outcome
        outcome = result->value;

        if (outcome == 0) 
            arg->n_ties ++;
        else if (outcome == arg->id)
            arg->n_wins ++;
        }

    return NULL;
}

// compare the choices of two players
// Return values:
// 0: tie
// 1: play 1 won
// 2: play 2 won
int compare_choices(int choice1, int choice2)
{
    int r;

    if (choice1 == choice2) 
        r= 0;
    else if ((choice1 + 1) % 3 == choice2)
        r= 2;
    else
        r= 1;
    return r;
}

// Referee thread main function.
void * thread_referee(void *arg_in)
{
    referee_arg_t *arg = arg_in;

    shared_int_t * p1 = arg->choice1;
    shared_int_t * p2 = arg->choice2;
    result_t * result = arg->result;

    // Use the following statement to print the guess
    for (int i = 0; i < arg->n_rounds; i ++) {
        // TODO
        //     wait for choices from players
        //     find out who wins
        //     announce result
        //
        // Call compare_choices() to compare choices:
        //
        //      outcome = compare_choices(choice1, choice2);
        //
        // The big challenge is synchronization.

        int choice1, choice2, outcome;
        // Handle player 1
        // Lock the mutex
        pthread_mutex_lock(&p1->mutex);
        // Wait for a choice
        while (p1->status != S_READY)
        {
            pthread_cond_wait(&p1->cond, &p1->mutex);
        }
        // We now have a choice from player1 so grab that choice and change status
        // Set the choice 
        choice1 = p1->value;
        // Set the status so we are back to playing the game again
        p1->status = S_INIT;
        // Unlock mutex
        pthread_mutex_unlock(&p1->mutex);

        // Handle player 2
        // Lock the mutex
        pthread_mutex_lock(&p2->mutex);
        // Wait for a choice
        while (p2->status != S_READY)
        {
            pthread_cond_wait(&p2->cond, &p2->mutex);
        }
        // We now have a choice from player2 so grab that choice and change status
        // Set the choice 
        choice2 = p2->value;
        // Set the status so we are back to playing the game again
        p2->status = S_INIT;
        // Unlock mutex
        pthread_mutex_unlock(&p2->mutex);

        // Now we have the two choices so compare them 
        outcome = compare_choices(choice1, choice2);

        // Save the in the result of each player
        result->value = outcome;

        //Wait for all players to get here
        pthread_barrier_wait(&result->barrier);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int seed = 5050;
    int n_rounds = 10;
    int opt_quiet = 0;

    // parse the command line arguments

    for (int i = 1; i < argc; i ++) {
        if (!strncmp(argv[i], "-s", 2) && isdigit(argv[i][2])) {
            seed = atoi(&argv[i][2]);
            if (seed <= 0)
                die("seed is 0 or it is too large.");
        } else if (!strncmp(argv[i], "-n", 2) && isdigit(argv[i][2])) {
            n_rounds = atoi(&argv[i][2]);
            if (n_rounds <= 0)
                die("number of rounds is 0 or it is too large.");
        } else if (! strcmp(argv[i], "-q")) { // quiet mode
            opt_quiet = 1;
        } else {
            fprintf(stderr, "Usage: %s [-s<seed>] [-n<num-rounds>] [-q]\n", argv[0]);
            return 1;
        }
    }

    shared_int_t    data[2];     // three shared data
    result_t        result;
    player_arg_t    pargs[NUM_PLAYERS];
    pthread_t       tids[NUM_PLAYERS];
    referee_arg_t   rarg;

    int rv;

    // initailze data[2] and result
    for (int i = 0; i < NUM_PLAYERS; i ++) {
        data[i].status = S_INIT;
        rv = pthread_mutex_init(&data[i].mutex, NULL);
        check_pthread_return(rv);
        rv = pthread_cond_init(&data[i].cond, NULL);
        check_pthread_return(rv);
    }

    // think about why the count parameter is NUM_PLAYERS + 1
    rv = pthread_barrier_init(&result.barrier, NULL, NUM_PLAYERS + 1);
    check_pthread_return(rv);

    // create threads 
    for (int i = 0; i < NUM_PLAYERS; i ++) {
        pargs[i].id = i+1;
        pargs[i].seed = (seed << 2) + i;
        pargs[i].n_rounds = n_rounds;
        pargs[i].choice = &data[i];
        pargs[i].result = &result;
        pargs[i].opt_quiet = opt_quiet;
        rv = pthread_create(&tids[i], NULL, thread_player, &pargs[i]);
        check_pthread_return(rv);
    }

    // call referee thread, instead of create another thread
    rarg.choice1 = &data[0];
    rarg.choice2 = &data[1];
    rarg.result  = &result;
    rarg.n_rounds = n_rounds;
    rarg.opt_quiet = opt_quiet;
    thread_referee(&rarg);

    // wait for players 
    for (int i = 0; i < NUM_PLAYERS; i ++) {
        rv = pthread_join(tids[i], NULL);
        check_pthread_return(rv);
    }

    // clean up
    for (int i = 0; i < NUM_PLAYERS; i ++) {
        rv = pthread_mutex_destroy(&data[i].mutex);
        check_pthread_return(rv);
        rv = pthread_cond_destroy(&data[i].cond);
        check_pthread_return(rv);
    }

    rv = pthread_barrier_destroy(&result.barrier);
    check_pthread_return(rv);
    
    for (int i = 0; i < NUM_PLAYERS; i ++) {
        printf("Player %d won %3d times, lost %3d times, and tied %3d times.\n", 
                pargs[i].id, pargs[i].n_wins, 
                pargs[i].n_rounds - pargs[i].n_wins - pargs[i].n_ties,
                pargs[i].n_ties);
    }
    
    return 0;
}
