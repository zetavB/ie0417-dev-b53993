#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

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

    cmd->name = name;
    cmd->priv = priv;
    cmd->execute = execute;
    return cmd;
}

void command_destroy(struct command_info *cmd){
    free(cmd);
}

void command_execute(struct command_info *cmd, char *req_msg, char **ret_msg){
    cmd->execute(cmd->name, cmd->priv, req_msg, ret_msg);
}

static void exec_func_msg(const char *name, void *priv, char *req_msg, char **resp_msg){
    cJSON *message = NULL;
    cJSON *storage = cJSON_CreateObject();

    message = cJSON_CreateString("Esto fue un mensaje");
    cJSON_AddItemToObject(storage, "message", message);
    *resp_msg = cJSON_Print(storage);
    cJSON_Delete(storage);
}

static void exec_func_pipo(const char *name, void *priv, char *req_msg, char **resp_msg){
    // Parse JSON from file data
    cJSON *data = cJSON_Parse(req_msg);
    if (data == NULL) {
        fprintf(stderr, "Failed to parse json file\n");
    }else{
        *resp_msg = cJSON_Print(data);
    }
    cJSON_Delete(data);
}

struct command_info *message_command_create(void *priv){
    char *message = "message";

    return command_create(message, priv, exec_func_msg);
}

struct command_info *ping_pong_command_create(void *priv){
    char *ping_pong = "ping_pong";

    return command_create(ping_pong, priv, exec_func_pipo);
}