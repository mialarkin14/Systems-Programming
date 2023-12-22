#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t* my_data = (thread_arg_t*) p_arg;
    int start = my_data->id*(my_data->m->nrows/2);
    int end = start + (my_data->m->nrows/2);
    // If we have an odd number of rows 
    if (my_data->m->nrows%2 != 0)
    {
        end += my_data->id;
    }
    for (unsigned int i = start; i < end; i++)
    {
        for (unsigned int j = 0; j < my_data->m->ncols; j++)
        {
            my_data->t->data[i][j] = my_data->m->data[i][j] + my_data->n->data[i][j];
        }
    }
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // TODO
    // Create an array of threads
    pthread_t threads[NUM_THREADS];
    // Create an array that holds that thread's data
    thread_arg_t thread_data_array[NUM_THREADS];
    int x;

    // Set up each thread
    for(int i = 0; i < NUM_THREADS; i++)
    {
        thread_data_array[i].id = i;
        thread_data_array[i].m = m;
        thread_data_array[i].n = n;
        thread_data_array[i].t = t;
        x = pthread_create(&threads[i], NULL, thread_main, &thread_data_array[i]);
        if (x)
        {
            printf("ERROR; return code from pthread_create() is %d\n", x);
            exit(-1);
        }
    }
    // Join each thread
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
