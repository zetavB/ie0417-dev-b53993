#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <eie_device/command.h>

#define MSG_CMD_MAX_SIZE 256

struct command_info *command_create(const char *name, void *priv, command_fn execute){
    struct command_info *cmd =
        (struct command_info *)calloc(1, sizeof(struct command_info));

    if (cmd == NULL) {
        fprintf(stderr, "Failed to allocate command\n");
        return NULL;
    }
    if (execute == NULL) {
        fprintf(stderr, "Command execute function cannot be NULL\n");
        free(cmd);
        return NULL;
    }
    strncpy(cmd->name, name, MSG_CMD_MAX_SIZE);
    cmd->priv = priv;
    cmd->execute = execute;
    return cmd;
}

void command_destroy(struct command_info *cmd){
    free(cmd);
}

void command_execute(struct command_info *cmd, char *req_msg, char *ret_msg){
    cmd->execute(cmd->name, cmd->priv, req_msg, ret_msg);
}

static void exec_func_msg(const char *name, void *priv, const char *req_msg, char *resp_msg){
    resp_msg = cJSON_CreateString("Esto fue un mensaje");
}

static void exec_func_pipo(const char *name, void *priv, const char *req_msg, char *resp_msg){
    cJSON *value = NULL;
    int num_args = sizeof(priv);
    char *tempArr[sizeof(priv)] = priv;

    cJSON *object = cJSON_CreateObject();
    for(int i=0; i < num_args; i++){
        value = cJSON_CreateString(tempArr[i]);
        char *arg_name = "arg"+i;
        cJSON_AddItemToObject(object, arg_name, value);  
    }
    resp_msg = (char *)object;
}

struct command_info *message_command_create(void *json){
    char *message = "message";

    cJSON *cjson = NULL;
    cJSON *args = NULL;
    int num_args = 0;

    // Parse JSON from file data
    cjson = cJSON_Parse(json);
    if (cjson == NULL) {
        fprintf(stderr, "Failed to parse json file\n");
        return NULL;
    }

    args = cJSON_GetObjectItem(cjson, "Arguments");
    if (args == NULL) {
        fprintf(stderr, "Failed to read argument array: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    num_args = cJSON_GetArraySize(args);
    char* results[num_args];
    for(int i = 0; i < num_args; i++){
        cJSON *arg, *obj;
        char *value;
        arg = cJSON_GetArrayItem(args, i);

        // Read arg value from JSON
        obj = cJSON_GetObjectItem(arg, "value");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read argument value: %s\n", cJSON_GetErrorPtr());
            return NULL;
        }
        value = cJSON_GetStringValue(obj);
        results[i] = value;
    }
    cJSON_Delete(cjson);
    cJSON_Delete(args);
    cJSON_Delete(arg);
    cJSON_Delete(obj);
    void *ptr = results;
    return command_create(message, ptr, exec_func_msg);
}

struct command_info *ping_pong_command_create(void *json){
    char *ping_pong = "ping_pong";

    cJSON *cjson = NULL;
    cJSON *args = NULL;
    int num_args = 0;
    // Parse JSON from file data
    cjson = cJSON_Parse(json);
    if (cjson == NULL) {
        fprintf(stderr, "Failed to parse json file\n");
        return NULL;
    }

    args = cJSON_GetObjectItem(cjson, "Arguments");
    if (args == NULL) {
        fprintf(stderr, "Failed to read argument array: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    num_args = cJSON_GetArraySize(args);
    char* results[num_args];
    for(int i = 0; i < num_args; i++){
        cJSON *arg, *obj;
        char *value;
        arg = cJSON_GetArrayItem(args, i);

        // Read arg value from JSON
        obj = cJSON_GetObjectItem(arg, "value");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read argument value: %s\n", cJSON_GetErrorPtr());
            return NULL;
        }
        value = cJSON_GetStringValue(obj);
        results[i] = value;
    }

    void *ptr;
    ptr = results;
    return command_create(ping_pong, ptr, exec_func_pipo);
}