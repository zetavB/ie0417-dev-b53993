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
    printf("test success\n");
}

int startupTest(){
    struct EieDevice *device;
    struct FunctionInfo *info;
    char *testJson;

    struct EieDeviceConfig *cfg =
        (struct EieDeviceConfig *)calloc(1, sizeof(struct EieDeviceConfig));
    

    cJSON *address = NULL;
    cJSON *id = NULL;
    cJSON *feature_id = NULL;
    cJSON *definition = NULL;
    cJSON *properties = NULL;
    cJSON *configuration = NULL;
    cJSON *status = NULL;
    cJSON *name = NULL;

    cJSON *config = cJSON_CreateObject();
    address = cJSON_CreateString("127.0.0.1");
    id = cJSON_CreateString("mosquitto");
    name = cJSON_CreateString("com.eie.proyecto:testDevice:1.0");
    feature_id = cJSON_CreateObject();
    definition = cJSON_CreateArray();
    properties = cJSON_CreateObject();
    configuration = cJSON_CreateObject();
    status = cJSON_CreateObject();

    cJSON_AddItemToObject(config, "testFeature", feature_id);
    cJSON_AddItemToObject(feature_id, "definition", definition);
    cJSON_AddItemToObject(feature_id, "properties", properties);
    cJSON_AddItemToObject(properties, "configuration", configuration);
    cJSON_AddItemToObject(properties, "status", status);
    cJSON_AddItemToArray(definition, name);

    cJSON_AddItemToObject(configuration, "address", address);
    cJSON_AddItemToObject(configuration, "id", id);
    testJson = cJSON_Print(config);
    cJSON_Delete(config);
    cfg->configJson = testJson;
    printf("Feature sent:\n %s\n", testJson);

    char *nameChar = (char *)malloc(12*sizeof(char));
    nameChar = "testFeature";
    cfg->name = nameChar;
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
    startupTest();
    return ret;
}
