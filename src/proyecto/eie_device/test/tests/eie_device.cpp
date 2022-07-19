#include "gtest/gtest.h"

#include <eie_device/eie_device.h>
#include <testutil/rand_gen.hpp>

/** Example fixture class for demo API tests */
class command_runner_fixture : public testing::Test
{
 protected:
    /* Fixture class members, accesible from test functions */
    int value;
    testutil::rand_gen rng;
    struct CommandRunnerConfig cmd_runner_cfg;
    struct CommandRunner *cmd_runner;
    int ret;

    /* Fixture class constructor */
    /* NOTE: Using reproducible random value for seed, check
     * explanation in unittest_main.cpp for more details */
    command_runner_fixture()
        : value(2), rng(rand())
    {
        std::cout << "Test fixture constructor! "<< std::endl;
        std::cout << "  RNG seed " << rng.get_seed() << std::endl;
    }

    virtual void SetUp() {
        int q_size = int(rng.get_rnd_u64_range(1, 1000));
        cmd_runner_cfg = {
            .q_max_size = q_size,
        };
        printf("Q is this size: %d\n",q_size);
        cmd_runner = command_runner_create(&cmd_runner_cfg);
        command_runner_start(cmd_runner);

        std::cout << "Test fixture SetUp! "<< std::endl;
        /* NOTE: Both the constructor and SetUp methods are called for each test.
         * Check Googletest FAQ for more details on when to use each one:
         * https://github.com/google/googletest/blob/main/docs/faq.md#should-i-use-the-constructordestructor-of-the-test-fixture-or-setupteardown-ctorvssetup */
    }

    virtual void TearDown() {
        command_runner_destroy(cmd_runner);

        std::cout << "Test fixture TearDown! "<< std::endl;
        /* NOTE: Both the destructor and TearDown methods are called for each test.
         * Check Googletest FAQ for more details on when to use each one:
         * https://github.com/google/googletest/blob/main/docs/faq.md#should-i-use-the-constructordestructor-of-the-test-fixture-or-setupteardown-ctorvssetup */
    }
};

TEST_F(command_runner_fixture, create_destroy)
{
    ret = 0;
    struct CommandRunner *cmd_runner_test;

    cmd_runner_test = command_runner_create(NULL);
    if(cmd_runner_test != NULL){
        ret++;
    }

    if(command_runner_destroy(NULL) != -1){
        ret++;
    }
    ASSERT_EQ(ret, 0);
}

TEST_F(command_runner_fixture, start_stop)
{
    ret = 0;
    int returner;
    if(command_runner_start(NULL) != -1){
        ret++;
    }

    returner = command_runner_start(cmd_runner);
    if (returner) {
        fprintf(stderr, "Failed to start command runner with ret=%d\n", returner);
        ret++;
    }
    returner = 0;

    if(command_runner_stop(NULL) != -1){
        ret++;
    }
    returner = command_runner_stop(cmd_runner);
    if (returner) {
        fprintf(stderr, "Failed to stop command runner with ret=%d\n", returner);
        ret++;
    }
    
    ASSERT_EQ(ret, 0);
}

TEST_F(command_runner_fixture, command_send_single)
{
    struct Command *msg_cmd = msg_command_create("This is a command test message!\n");
    ret = 0;
    int returner;
    returner = command_runner_start(cmd_runner);
    if (returner) {
        fprintf(stderr, "Failed to start command runner with ret=%d\n", returner);
        ret++;
    }
    returner = 0;

    returner = command_runner_send(NULL, msg_cmd);
    if (returner != -1){
        fprintf(stderr, "Sent message to NULL command runner with ret=%d\n", returner);
        ret++;
    }
    returner = 0;

    returner = command_runner_send(cmd_runner, NULL);
    if (returner != -1){
        fprintf(stderr, "Sent NULL message to command runner with ret=%d\n", returner);
        ret++;
    }
    returner = 0;

    returner = command_runner_send(cmd_runner, msg_cmd);
    if (returner) {
        fprintf(stderr, "Failed to send command to command runner with ret=%d\n", ret);
        ret++;
    }
    returner = 0;

    command_destroy(msg_cmd);
    returner = command_runner_stop(cmd_runner);
    if (returner) {
        fprintf(stderr, "Failed to stop command runner with ret=%d\n", ret);
        ret++;
    }
    //ASSERT_EQ(ret, DEMO_API_OK);
    ASSERT_EQ(ret, 0);
}