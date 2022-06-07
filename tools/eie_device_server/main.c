#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include <eie_device/version.h>
#include <eie_device/command.h>
#include <eie_device/command_manager.h>

int main(int argc, char **argv){
    int opt;
    int ret = 0;
    const char *version = NULL;
    char cfg_filename[PATH_MAX + 1];

    // Parse cmdline options
    while((opt = getopt(argc, (char *const *)argv, "c:")) != -1) {
        switch(opt) {
            case 'c':
                strncpy(cfg_filename, optarg, PATH_MAX);
                break;
            default :
                (void)fprintf(stderr, "Unknown option -%c\n", opt);
                return -EINVAL;
        }
    }

    // Check library version
    ret = eie_device_version(&version);
    if (ret) return ret;
    printf("lib version: %s\n", version);

    struct CommandManager *command_manager = command_manager_create("message");

    command_manager_command_add(command_manager, "ping_pong");


    // Create test CJSON
    cJSON *name = NULL;
    cJSON *test = cJSON_CreateObject();
    name = cJSON_CreateString("testJson");
    cJSON_AddItemToObject(test, "arg0", name);
    
    char *payload = cJSON_Print(test);
    char **response;
    response=(char**)calloc(1,sizeof(char**));

    command_manager_cmd_send(command_manager, "message", payload, response);
    printf("%s\n", *response);
    command_manager_cmd_send(command_manager, "ping_pong", payload, response);
    printf("%s\n", *response);

    command_manager_command_remove(command_manager, "message");
    command_manager_command_remove(command_manager, "ping_pong");
    command_manager_destroy(command_manager);

    cJSON_Delete(test);
    
    return ret;
}