#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <eie_device/command_manager.h>
#include <eie_device/command.h>
#include <eie_device/external/uthash.h>

/** Entry structure to support adding commands to an UT hash table */
struct CommandHashEntry {
    struct command_info *cmd;
    UT_hash_handle hh;
};

/** Command manager structure declaration (not exposed) */
struct CommandManager {
    struct CommandHashEntry *cmd_ht;
};

static int command_ht_add(struct CommandManager *cmdmgr, struct command_info *command){
    struct CommandHashEntry *entry =
        malloc(sizeof(struct CommandHashEntry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate command hash entry\n");
        return -ENOMEM;
    }
    entry->cmd = command;
    HASH_ADD_KEYPTR(hh, cmdmgr->cmd_ht, command->name, strlen(command->name), entry);
    return 0;
}

static int command_ht_remove(struct CommandManager *cmdmgr, struct command_info *command){
    struct CommandHashEntry *entry;
    HASH_FIND(hh, cmdmgr->cmd_ht, command->name, strlen(command->name), entry);
    if(entry == NULL){
        fprintf(stderr, "Failed to delete command from hash table\n");
        return -1;
    }
    HASH_DEL(cmdmgr->cmd_ht, entry);
    free(entry);
    return 0;
}

/** Destroys the manager's command hash table */
static void command_ht_destroy(struct CommandManager *cmdmgr)
{
    struct CommandHashEntry *entry, *tmp;
    HASH_ITER(hh, cmdmgr->cmd_ht, entry, tmp) {
        HASH_DEL(cmdmgr->cmd_ht, entry);
        free(entry);
    }
}

static int command_manager_ht_create(struct CommandManager *cmdmgr, char *name){
    int ret;
    struct command_info *command = NULL;

     // Init head entry for command hash table
    cmdmgr->cmd_ht = NULL;


    //Create sample command and add to HT
    if(strcmp(name, "message") == 0){
        char* msg = "Atribute msg";
        command = message_command_create(msg);
    }else{
        char* msg = "Atribute ping_pong";
        command = ping_pong_command_create(msg);
    }

    if(command == NULL){
        fprintf(stderr, "Failed to create command\n");
        return -1;
    }

    ret = command_ht_add(cmdmgr, command);
    if (ret) {
            fprintf(stderr, "Failed to add command to HT\n");
            return ret;
    }
    return 0;
}

struct CommandManager *command_manager_create(char *name){

    struct CommandManager *cmdmgr =
        (struct CommandManager *)calloc(1, sizeof(struct CommandManager));
    if(cmdmgr == NULL){
        fprintf(stderr, "Failed to allocate command manager\n");
        return NULL;
    }
    int ret = command_manager_ht_create(cmdmgr, name);
    if (ret) {
        fprintf(stderr, "Failed to create sensor hash table with ret=%d\n", ret);
        return NULL;
    }

    return cmdmgr;
}

void command_manager_command_add(struct CommandManager *cmdmgr,
                                         const char *name){
    char *attributes = "This is a attribute";
    int ret;
    struct command_info *command = NULL;

    struct CommandHashEntry *entry = NULL;

    HASH_FIND(hh, cmdmgr->cmd_ht, name, strlen(name), entry);
    if(entry != NULL){
        fprintf(stderr, "Command already exists\n");
    }else{
        if(strcmp(name, "message") == 0){
            command = message_command_create(attributes);
        }else{
            command = ping_pong_command_create(attributes);
        }
        if(command == NULL){
            fprintf(stderr, "Failed to create command\n");
        }
        ret = command_ht_add(cmdmgr, command);
        if (ret) {
            fprintf(stderr, "Failed to add command to HT\n");
        }
    }
}

void command_manager_command_remove(struct CommandManager *cmdmgr,
                                         const char *name){
    struct CommandHashEntry *entry = NULL;
    HASH_FIND(hh, cmdmgr->cmd_ht, name, strlen(name), entry);
    if(entry == NULL){
        fprintf(stderr, "Command doesn't exist\n");
    }else{
        command_ht_remove(cmdmgr, entry->cmd);
    }
}

void command_manager_cmd_send(struct CommandManager *cmdmgr,
                                         const char *name, char *json, char **return_buff){
    struct CommandHashEntry *entry = NULL;
    *return_buff = (char *)realloc(*return_buff,strlen(json)*sizeof(char));
    HASH_FIND(hh, cmdmgr->cmd_ht, name, strlen(name), entry);
    command_execute(entry->cmd, json, return_buff);
}

void command_manager_destroy(struct CommandManager *cmdmgr){
    command_ht_destroy(cmdmgr);
    free(cmdmgr);
}
