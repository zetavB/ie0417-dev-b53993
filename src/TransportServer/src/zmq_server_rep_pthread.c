/*
 * This is the REP socket server program for the zmq_req_client script (@ examples/python/zmq_demo).
 * It relies on a pthread to process the socket messages.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <czmq.h>
#include <cjson/cJSON.h>

/* Message types to set in the test_msg_req type field */
enum test_msg_type {
  TEST_MSG_TYPE_A,
  TEST_MSG_TYPE_B,
  TEST_MSG_TYPE_C,
  TEST_MSG_TYPE_MAX,
};

/* Message request structure */
struct cmd_request_hdr {
    char cmd_name [100];
    uint32_t payload_size;
    char buff[100];
} __attribute__((packed));
/* Message response structure */
struct test_msg_rep {
    char resp_name [100];
    uint32_t resp_size;
    char resp_buff[100];

} __attribute__((packed));

/*struct mem_pointers {
    char * request_msg;
    char * payload_data;
};*/

/* Server thread data */
struct server_data {
    pthread_t tid;
    zsock_t *server;
};

/*static cJSON *parse_cjson(char *buffer, size_t *buffer_length){
  cJSON *cjson = NULL;
  buffer = malloc(*buffer_length);
  cjson = cJSON_ParseWithLength(buffer, *buffer_length);
  if (cjson == NULL) {
      fprintf(stderr, "Failed to parse json file\n");
      return NULL;
  }
  return cjson;
}*/
void* msg_server_fn(void *arg)
{
    //cJSON *cjson = NULL;
    int ret;
    struct server_data *rdata = arg;
    printf("Thread %ld started\n", rdata->tid);

    rdata->server = zsock_new(ZMQ_REP);
    zsock_bind(rdata->server, "tcp://*:5555");

    /* Loop processing messages while CZMQ is not interrupted */
    while (!zsys_interrupted) {
        zframe_t *req_frame, *rep_frame;
        struct cmd_request_hdr *header;
        //struct payload *payload;
        struct test_msg_rep *rep;
        //struct mem_pointers *mp;

        // Block waiting for a new message frame
        req_frame = zframe_recv(rdata->server);
        if (!req_frame) {
            fprintf(stderr, "req_frame is NULL\n");
            goto out;
        }
        header = (struct cmd_request_hdr *)zframe_data(req_frame);
        //header->buff = (char*)malloc(header->payload_size*sizeof(char));
        //mp->request_msg = &header->cmd_name[0];
        //mp->payload_data = &mp->request_msg + sizeof(struct cmd_request_hdr);
        //payload = (struct payload *)zframe_data(req_frame);
        //payload = (struct payload*)zframe_data(req_frame);
        //cjson = parse_cjson(payload->buffer, size_t(header->payload_size));
        printf("Received request [command_name: %s, size: %u payload: %s]\n",
               header->cmd_name, header->payload_size, header->buff);

        rep_frame = zframe_new(NULL, sizeof(struct test_msg_rep));
        rep = (struct test_msg_rep *)zframe_data(rep_frame);
        // Write response data
        for (int i = 0; i<=strlen(header->cmd_name); i++){
          rep->resp_name[i] = header->cmd_name[i];
        }
        for (int i = 0; i<=strlen(header->buff); i++){

        }
        //rep->resp_name = header->cmd_name;
        rep->resp_size = header->payload_size;

        // No longer need request frame
        zframe_destroy(&req_frame);

        // Sending destroys the response frame
        ret = zframe_send(&rep_frame, rdata->server, 0);
        if (ret) {
            fprintf(stderr, "Failed to send msg with: %d\n", ret);
            goto out;
        }
    }

out:
    zsock_destroy(&rdata->server);
    zsys_interrupted = 1;
    printf("Thread %ld finished\n", rdata->tid);
    return NULL;
}

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
