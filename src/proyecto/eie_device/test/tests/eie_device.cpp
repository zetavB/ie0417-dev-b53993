#include "gtest/gtest.h"
#include <cjson/cJSON.h>

#include <eie_device/eie_device.h>
#include <testutil/rand_gen.hpp>

/** Example fixture class for demo API tests */
class eie_device_fixture : public testing::Test
{
 protected:
    /* Fixture class members, accesible from test functions */
    int value;
    testutil::rand_gen rng;
    struct EieDevice *device;
    int ret;

    /* Fixture class constructor */
    /* NOTE: Using reproducible random value for seed, check
     * explanation in unittest_main.cpp for more details */
    eie_device_fixture()
        : value(2), rng(rand())
    {
        std::cout << "Test fixture constructor! "<< std::endl;
        std::cout << "  RNG seed " << rng.get_seed() << std::endl;
        
    }

    virtual void SetUp() {
        int q_size = int(rng.get_rnd_u64_range(1, 1000));
        char *testJson;
        struct EieDeviceConfig *cfg = (struct EieDeviceConfig *)calloc(1, sizeof(struct EieDeviceConfig));
        struct FunctionInfo *info;

        cJSON *address = NULL;
        cJSON *id = NULL;
        cJSON *feature_id = NULL;
        cJSON *definition = NULL;
        cJSON *properties = NULL;
        cJSON *configuration = NULL;
        cJSON *status = NULL;
        cJSON *name = NULL;
        
        cJSON *config = cJSON_CreateObject();
        address = cJSON_CreateString("tcp://mosquitto:1883");
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
        
        cfg->configJson = testJson;
        std::cout << "  Running " << cfg->configJson <<std::endl;
        //char *nameChar = (char *)malloc(12*sizeof(char));
        
        char *nameChar = (char*)"testFeature";
        cfg->name = nameChar;

        ret = 0;

        info = info_create("testCallback", testCallback);
        if(info == NULL){
            printf("Failed to catch null start condition for function info");
            ret++;
        }
        device = eie_device_create(cfg, info);
        if(device == NULL){
            printf("Failed to catch null start condition for device creation");
            ret++;
        }
        printf("Q is this size: %d\n",q_size);

        std::cout << "Test fixture SetUp! "<< std::endl;
        ASSERT_EQ(ret, 0);
        /* NOTE: Both the constructor and SetUp methods are called for each test.
         * Check Googletest FAQ for more details on when to use each one:
         * https://github.com/google/googletest/blob/main/docs/faq.md#should-i-use-the-constructordestructor-of-the-test-fixture-or-setupteardown-ctorvssetup */
    }

    virtual void TearDown() {
        eie_device_destroy(device);
        std::cout << "Test fixture TearDown! "<< std::endl;
        /* NOTE: Both the destructor and TearDown methods are called for each test.
         * Check Googletest FAQ for more details on when to use each one:
         * https://github.com/google/googletest/blob/main/docs/faq.md#should-i-use-the-constructordestructor-of-the-test-fixture-or-setupteardown-ctorvssetup */
    }

    static void testCallback(const char *name, char *features, char *resp_msg){
        printf("test success\n");
    }
};


TEST_F(eie_device_fixture, start_stop)
{
    ret = 0;
    int returner = 0;
    
    
    if(eie_device_start(NULL) != -1){
        fprintf(stderr, "Failed to catch null start condition with ret=%d\n", returner);
        ret++;
    }
    
    if(eie_device_stop(NULL) != -1){
        fprintf(stderr, "Failed to catch null stop condition with ret=%d\n", returner);
        ret++;
    }

    returner = eie_device_start(device);
    if (returner) {
        fprintf(stderr, "Failed to stop eie_device with ret=%d\n", returner);
        ret++;
    }

    returner = eie_device_stop(device);
    if (returner) {
        fprintf(stderr, "Failed to stop eie_device with ret=%d\n", returner);
        ret++;
    }
    
    ASSERT_EQ(ret, 0);
}

TEST_F(eie_device_fixture, send_message)
{
    ret = 0;
    int returner = 0;
    char *message = (char*)"testMessage";

    if(eie_device_send_message(NULL, message) != -1){
        fprintf(stderr, "Failed to catch null send condition with ret=%d\n", returner);
        ret++;
    }
    if(eie_device_send_message(device, NULL) != -1){
        fprintf(stderr, "Failed to catch null send condition with ret=%d\n", returner);
        ret++;
    }

    eie_device_start(device);
    returner = eie_device_send_message(device, message);
    if (returner) {
        fprintf(stderr, "Failed to send message with ret=%d\n", returner);
        ret++;
    }

    ASSERT_EQ(ret, 0);
}

