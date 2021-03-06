#include "mercury_thread_pool.h"
#include "mercury_thread_mutex.h"

#include "mercury_test_config.h"

#include <stdio.h>
#include <stdlib.h>

#define POOL_NUM_POSTS 32

/*
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

static HG_THREAD_RETURN_TYPE
myfunc(void *args)
{
    pid_t tid;

    tid = syscall(SYS_gettid);

    printf("hello from tid: %d!\n", tid);

    return NULL;
}
*/

static unsigned int ncalls = 0;
static hg_thread_mutex_t mymutex;

static HG_THREAD_RETURN_TYPE
myfunc(void *args)
{
    hg_thread_ret_t ret = 0;
    (void) args;

    hg_thread_mutex_lock(&mymutex);
    ncalls++;
    /* printf("%d\n", ncalls); */
    hg_thread_mutex_unlock(&mymutex);

    return ret;
}

int
main(int argc, char *argv[])
{
    int i;
    hg_thread_pool_t *thread_pool;
    struct hg_thread_work work[POOL_NUM_POSTS];
    int ret = EXIT_SUCCESS;

    (void) argc;
    (void) argv;
    hg_thread_mutex_init(&mymutex);
    hg_thread_pool_init(MERCURY_TESTING_NUM_THREADS, &thread_pool);

    for (i = 0; i < POOL_NUM_POSTS; i++) {
        work[i].func = myfunc;
        work[i].args = NULL;
        hg_thread_pool_post(thread_pool, &work[i]);
    }

    /* printf("Finalizing...\n"); */
    hg_thread_pool_destroy(thread_pool);
    hg_thread_mutex_destroy(&mymutex);

    if (ncalls != POOL_NUM_POSTS) {
        fprintf(stderr, "Did not execute all the operations posted (%u/%d)\n",
                ncalls, POOL_NUM_POSTS);
        ret = EXIT_FAILURE;
    }
    return ret;
}
