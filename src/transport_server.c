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
#include <eie_device/command.h>
#include <eie_device/command_manager.h>
#include "eie_device/transport_server.h"

/* Message types to set in the test_msg_req type field */
enum test_msg_type {
  TEST_MSG_TYPE_A,
  TEST_MSG_TYPE_B,
  TEST_MSG_TYPE_C,
  TEST_MSG_TYPE_MAX,
};

void* msg_server_fn(void *arg)
{
  int ret;
    struct server_data *rdata = arg;
    printf("Thread %ld started\n", rdata->tid);

    rdata->server = zsock_new(ZMQ_REP);
    zsock_bind(rdata->server, "tcp://*:5555");

    struct CommandManager *command_manager = command_manager_create("message");
    command_manager_command_add(command_manager, "ping_pong");

    /*Pointers to the structures*/
        struct payload *payload;
        struct cmd_request_hdr *header;
        struct test_msg_rep *rep;

    /* Loop processing messages while CZMQ is not interrupted */
    while (!zsys_interrupted) {

      /*Memory allocation*/
        zframe_t *req_frame, *rep_frame;
        payload = (struct payload *)calloc(1, sizeof(struct payload));
        header = (struct cmd_request_hdr *)calloc(1, sizeof(struct cmd_request_hdr));
        //struct payload *payload;
        rep = (struct test_msg_rep *)calloc(1, sizeof(struct test_msg_rep));

        // Block waiting for a new message frame
        req_frame = zframe_recv(rdata->server);
        if (!req_frame) {
            fprintf(stderr, "req_frame is NULL\n");
            goto out;
        }
        /*Copy the memory address of the received data in header*/
        header = (struct cmd_request_hdr *)zframe_data(req_frame);
        /*Memory allocation*/
        payload->buff = (char*)malloc(header->payload_size*sizeof(char));
        /*Calculate the offset (request size - payload_size)*/
        payload->offset = zframe_size(req_frame) - header->payload_size;
        printf("%i\n", payload->offset);
        /*Copy memory from the bytestream + offset to the payload pointer*/
        memcpy(payload->buff, zframe_data(req_frame)+payload->offset, 94);
        if (!req_frame) {
            fprintf(stderr, "req_frame is NULL\n");
            goto out;
        }
        printf("Received request [command_name: %s, size: %u payload: %s]\n",
               header->cmd_name, header->payload_size, payload->buff);


        //rep->resp_buff = (char *)malloc(rep->resp_payload_size*sizeof(char));

        char **response;
        //response=(char**)malloc(rep->resp_payload_size*sizeof(char**));
        //printf("payload size %u \n", header->payload_size );
        //response = (char **)calloc(header->payload_size,sizeof(char));
        //printf("initial mem  alocation for response %li\n", header->payload_size*sizeof(char));
        //rep->resp_buff = (char *)malloc(strlen(payload->buff)*sizeof(char));
        //char ** response = &rep->resp_buff;
        response = (char **)calloc(header->payload_size, sizeof(char));
        command_manager_cmd_send(command_manager, header->cmd_name, payload->buff, response);
        /*Preparing the frame of the reply*/
        printf("response: %s \n", *response);
        rep->resp_buff = (char *)malloc(strlen(*response)*sizeof(char));
        memcpy(rep->resp_buff, *response, strlen(*response));
        printf("size of response after the method %li\n",strlen(rep->resp_buff));
        printf("allocating mem resp buff\n");
        //rep->resp_buff = (char *)malloc(strlen(rep->resp_buff)*sizeof(char));

        printf("size of resp buffer %li\n",strlen(rep->resp_buff)*sizeof(char));

        printf("after rep struct\n");
          // No longer need request frame
        zframe_destroy(&req_frame);
        printf("response size: %li\n", strlen(rep->resp_buff));
        //strcpy(rep->resp_buff, *response);
        //memcpy(rep->resp_buff, response, strlen(rep->resp_buff));
        printf("reply buffer %s\n", rep->resp_buff);

        //strcpy(rep->resp_buff, *response);
        printf("after memcpy\n");
        rep_frame = zframe_new(rep->resp_buff, strlen(rep->resp_buff));
        printf("after new frame\n");
        rep = (struct test_msg_rep *)zframe_data(rep_frame);
        // Write response data
        //strcpy(rep->resp_buff, header->buff);

        // Sending destroys the response frame
        ret = zframe_send(&rep_frame, rdata->server, 0);
        if (ret) {
            fprintf(stderr, "Failed to send msg with: %d\n", ret);
            goto out;
        }
        printf("free memory\n");
        free(payload);
        free(response);
        //free(rep->resp_buff);

        zframe_destroy(&rep_frame);

    }

out:
    zsock_destroy(&rdata->server);
    zsys_interrupted = 1;
    printf("Thread %ld finished\n", rdata->tid);
    command_manager_command_remove(command_manager, "message");
    command_manager_command_remove(command_manager, "ping_pong");
    command_manager_destroy(command_manager);
    return NULL;
}

int serverExec(void)
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
