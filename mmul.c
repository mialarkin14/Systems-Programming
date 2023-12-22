#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    // TODO

    // Set up a thread structure called my_data with the passed arguments
    thread_arg_t* my_data = (thread_arg_t*) p_arg;
    // Set the start of the loop to be the thread's id times half the number of rows
    int start = my_data->id*(my_data->m->nrows/2);
    // Set the end of the loop to be half the number of rows plus the start
    int end = start + (my_data->m->nrows/2);
    // If we have an odd number of rows 
    if (my_data->m->nrows%2 != 0)
    {
        // Add 1 to the number of rows
        end += 1;
    }

     if (my_data->m == NULL ||my_data-> n == NULL || my_data->m->ncols != my_data->n->nrows)
     {
        return NULL;
     }
        
    TMatrix * t = newMatrix(my_data->m->nrows, my_data->n->ncols);
    if (t == NULL)
        return t;
    for (unsigned int i = start; i < end; i++)  
    {
        for (unsigned int j = 0; j < my_data->n->ncols; j++) 
        {
            TElement sum = (TElement)0;
            for (unsigned int k = 0; k < my_data->m->ncols; k++)
            {
                sum += my_data->m->data[i][k] * my_data->n->data[k][j];
            }
            my_data->t->data[i][j] = sum;
        }
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    
    // TODO

    // Create an array to hold the threads
    pthread_t threads[NUM_THREADS];
    // Create an array that holds each thread's data
    thread_arg_t threads_data[NUM_THREADS];

    // Create the threads and set up each thread's data
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Set up the id of each thread
        threads_data[i].id = i;
        // Set up the matrix m
        threads_data[i].m = m;
        // Set up the matrix n
        threads_data[i].n = n;
        // Set up the resulting matrix
        threads_data[i].t = t;
        // Create threads by passing each thread to the thread_main function
        int x = pthread_create(&threads[i], NULL, thread_main, &threads_data[i]);
        // If something goes wrong when creating the thread, raise an error
        if (x)
        {
            printf("ERROR; return code from pthread_create() is %d\n", x);
                exit(-1);
        }
    }
    // Joining the threads
    int x;
    for(int i = 0; i < NUM_THREADS; i++ ) 
    {
        x = pthread_join(threads[i], NULL );
        if(x)
        {
            printf("ERROR; return code from pthread_join() is %d\n", x);
            exit(-1);
        }
    }

    return t;
}
