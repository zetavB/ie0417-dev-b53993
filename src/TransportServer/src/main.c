#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <czmq.h>
#include <cjson/cJSON.h>
#include "../include/transport_server.h"

int main(int argc, char **argv)
{
    int ret;
    struct server_data rdata = {};
    const uint32_t sleep_ms = 1000;

    ret = pthread_create(&(rdata.tid), NULL, &msg_server_fn, &rdata);
    if (ret != 0) {
        fprintf(stderr, "Failed to create thread :[%s]", strerror(ret));
        return ret;
    }
    /* zsys_interrupted is a global variable from the czmq library that is set
     * when the process receives an interrupt signal (SIGINT). This logic
     * is to stop the program, for example, when the user sends a keyboard
     * interrupt (Ctrl-C).
     */
    while(!zsys_interrupted) {
        zclock_sleep(sleep_ms);
    }
    /* We need some mechanism to stop the server thread that is likely blocked
     * waiting on I/O (new message). Pthread kill sends a signal to a specific thread,
     * which is used to interrupt the blocking zframe_recv call that waits for
     * a new message. When zframe_recv is interrupted it returns a NULL frame, which
     * we detect to break out of the thread's infinite loop.
     *
     * NOTE: An alternative, and maybe safer, approach would be to use a zpoller
     * with a timeout to avoid blocking the server thread indefinitely,
     * plus a stop boolean flag to break out of the while loop. We could
     * also use a high-level CZMQ threaded mechanism (like zloop) to process messages,
     * but that would defeat the purpose of this example (to use pthread) */
    pthread_kill(rdata.tid, SIGINT);
    pthread_join(rdata.tid, NULL);

    return 0;
}
