#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "MQTTClient.h"
#include <cjson/cJSON.h>
#include <eie_device/external/uthash.h>

#include <eie_device/eie_device.h>

/** Entry structure to support adding commands to an UT hash table */
struct DeviceHashEntry {
    /** Callback function container structure */
    struct FunctionInfo *function;
    /** Hash handle for hash table*/
    UT_hash_handle hh;
};

/** EieDevice structure */
struct EieDevice {
    /** MQTT Client */
    MQTTClient *client;
    /** Hash table */
    struct DeviceHashEntry *dHE_ht;
    /** JSON containing Ditto features */
    char *features;
};
void eie_device_parse_ditto_json(char *payload, char *features, char *topicName){
    cJSON *feature_id = NULL;
    cJSON *properties = NULL;

    cJSON *dittoPayload = cJSON_Parse(payload);
    feature_id = cJSON_GetObjectItemCaseSensitive(dittoPayload, topicName);
    properties = cJSON_GetObjectItemCaseSensitive(feature_id, "properties");

    strcpy(features, properties->valuestring);

    cJSON_Delete(feature_id);
    cJSON_Delete(properties);
    cJSON_Delete(dittoPayload);
}
int eie_device_msg_arrived_cb(void *context, char *topicName, int topicLen,
                              MQTTClient_message *message){
    printf("Message recieved.\n");
    struct EieDevice *tmp = (struct EieDevice*)context;
    struct DeviceHashEntry *entry = NULL;

    char *json = (char*)message->payload;
    char *features = (char *)malloc(254*sizeof(char));
    char *response = (char *)malloc(254*sizeof(char));

    eie_device_parse_ditto_json(json, features, topicName);

    HASH_FIND(hh, tmp->dHE_ht, topicName, strlen(topicName), entry);
    entry->function->execute(topicName, features, response);
    eie_device_send_message(tmp, response);
}

void eie_device_msg_delivered_cb(void *context, MQTTClient_deliveryToken dt){
    printf("Message delivered.\n");
}

void eie_device_conn_lost_cb(void *context, char *cause){
    printf("Connection lost.\n");
}

struct FunctionInfo *info_create(const char *name, device_fn execute){
    struct FunctionInfo *function =
        (struct FunctionInfo *)calloc(1, sizeof(struct FunctionInfo));

    if (function == NULL) {
        fprintf(stderr, "Failed to allocate function\n");
        return NULL;
    }
    if (execute == NULL) {
        fprintf(stderr, "Device execute function cannot be NULL\n");
        free(function);
        return NULL;
    }

    function->name = name;
    function->execute = execute;

    return function;
}

/** Adds element to device function hash table */
static int eie_device_ht_add(struct EieDevice *device, struct FunctionInfo *function){
    struct DeviceHashEntry *entry =
        malloc(sizeof(struct DeviceHashEntry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate device hash entry\n");
        return -ENOMEM;
    }
    entry->function = function;
    HASH_ADD_KEYPTR(hh, device->dHE_ht, function->name, strlen(function->name), entry);
    return 0;
}

/** Removes element from device function hash table */
static int eie_device_ht_remove(struct EieDevice *device, struct FunctionInfo *function){
    struct DeviceHashEntry *entry;
    HASH_FIND(hh, device->dHE_ht, function->name, strlen(function->name), entry);
    if(entry == NULL){
        fprintf(stderr, "Failed to delete function from hash table\n");
        return -1;
    }
    HASH_DEL(device->dHE_ht, entry);
    free(entry);
    return 0;
}

/** Destroys the device function hash table */
static void eie_device_ht_destroy(struct EieDevice *device)
{
    struct DeviceHashEntry *entry, *tmp;
    HASH_ITER(hh, device->dHE_ht, entry, tmp) {
        HASH_DEL(device->dHE_ht, entry);
        free(entry);
    }
}

static int eie_device_ht_create(struct EieDevice *device, struct FunctionInfo *function){
    int ret;

     // Init head entry for device hash table
    device->dHE_ht = NULL;

    ret = eie_device_ht_add(device, function);
    if (ret) {
            fprintf(stderr, "Failed to add function to HT\n");
            return ret;
    }
    return 0;
}

struct EieDevice * eie_device_create(struct EieDeviceConfig *cfg, struct FunctionInfo *function){
    int ret;
    struct EieDevice *device = (struct EieDevice *)calloc(1, sizeof(struct EieDevice));
    if(device == NULL){
        fprintf(stderr, "Failed to allocate eie Device\n");
        return NULL;
    }

    ret = eie_device_ht_create(device, function);
    if (ret) {
        fprintf(stderr, "Failed to create device hash table with ret=%d\n", ret);
        return NULL;
    }
    device->features = cfg->configJson;

    cJSON *data = NULL;
    cJSON *address = NULL;
    cJSON *id = NULL;
    char *tmp0 = (char *)malloc(60*sizeof(char));
    char *tmp1 = (char *)malloc(60*sizeof(char));

    MQTTClient *client = (MQTTClient *)malloc(sizeof(MQTTClient));
    
    data = cJSON_Parse(device->features);
    if (data == NULL) {
        fprintf(stderr, "Failed to parse json file\n");
        return NULL;
    }else{
        address = cJSON_GetObjectItemCaseSensitive(data, "address");
        id = cJSON_GetObjectItemCaseSensitive(data, "id");
        strcpy(tmp0, address->valuestring);
        strcpy(tmp1, id->valuestring);
    }
    cJSON_Delete(data);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create((client), tmp0, tmp1, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    ret = MQTTClient_setCallbacks(*client, device, eie_device_conn_lost_cb, eie_device_msg_arrived_cb, eie_device_msg_delivered_cb);
    device->client = client;
    
    return device;
}

int eie_device_destroy(struct EieDevice *device){
    int ret;

    MQTTClient_destroy((device->client));
    eie_device_ht_destroy(device);
    free(device);

    return ret;
}

int eie_device_start(struct EieDevice *device){
    int ret = 0;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    //ret = MQTTClient_connect(device->client, &conn_opts);

    const char topic[100] = "example/test";
    char ch;
    int qos = 0;

    printf("Subscribing to topic %s\nfor client using QoS %d\n\n", topic, qos);
    //ret = MQTTClient_subscribe(device->client, topic, qos);
    
    return ret;
}

int eie_device_stop(struct EieDevice *device){
    int ret;

    ret = MQTTClient_disconnect(device->client, 10000);
    if (ret) {
        fprintf(stderr, "Failed to disconnect from MQTT client with ret=%d\n", ret);
        return ret;
    }
    
    return ret;
}

int eie_device_send_message(struct EieDevice *device, char *msgJson){
    int ret;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = "Test";
    pubmsg.payloadlen = strlen("Test");
    pubmsg.qos = 0;
    pubmsg.retained = 0;
    int deliveredtoken = 0;

    ret = MQTTClient_publishMessage(device->client, "eie_device/tests", &pubmsg, &token);

    printf("Waiting for publication");
    while(deliveredtoken != token);
    return ret;
}
