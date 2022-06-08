#ifndef TRANSPORT_SERVER_H_
#define TRANSPORT_SERVER_H_

/** Message header request structure */
struct cmd_request_hdr {
    char cmd_name [100];
    uint32_t payload_size;
} __attribute__((packed));
/** Message Payload structure definitio*/
struct payload{
  char * buff;
  int offset;
} __attribute__((packed));
/* Message response structure definition */
struct test_msg_rep {
    char resp_name [100];
    uint32_t resp_payload_size;
    char resp_buff [100];
    //char  resp_buff[100];
} __attribute__((packed));

/** Server thread data */
struct server_data {
    pthread_t tid;
    zsock_t *server;
};
/**
 * Creates a Transport Server
 *
 * Contains the necesary data to execute a transport server
 *
 * @param arg    Function arguments
 *
 * @return void pointer for error state
 */
void* msg_server_fn(void *arg);

/**
 * Executes a Transport Server
 *
 * @return Error state
 */
int serverExec(void);

#endif // TRANSPORT_SERVER_H_
