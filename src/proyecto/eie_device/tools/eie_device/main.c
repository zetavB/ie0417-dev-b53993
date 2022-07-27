#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include <cjson/cJSON.h>

#include <eie_device/version.h>
#include <eie_device/eie_device.h>

void testCallback(const char *name, char *features, char *resp_msg){
    printf("test success");
}

int startupTest(){
    struct EieDevice *device;
    struct FunctionInfo *info;
    char *testJson;

    struct EieDeviceConfig *cfg =
        (struct EieDeviceConfig *)calloc(1, sizeof(struct EieDeviceConfig));
    

    cJSON *address = NULL;
    cJSON *id = NULL;

    cJSON *config = cJSON_CreateObject();
    address = cJSON_CreateString("127.0.0.1");
    id = cJSON_CreateString("mosquitto");

    cJSON_AddItemToObject(config, "address", address);
    cJSON_AddItemToObject(config, "id", id);
    testJson = cJSON_Print(config);
    cJSON_Delete(config);
    cfg->configJson = testJson;

    info = info_create("testCallback", testCallback);
    device = eie_device_create(cfg, info);
    eie_device_start(device);
    eie_device_destroy(device);

}

int main(int argc, char **argv) {
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
    printf("this might be working");
    startupTest();
    return ret;
}
