#ifndef COMMAND_H_
#define COMMAND_H_

#include <cjson/cJSON.h>

/** Type of the function that a command can execute */
typedef void (*command_fn)(const char *name, void *priv, char *req_msg, char **resp_msg);

struct command_info {
    /** Private data to pass to the execute function */
   const char *name;
   void *priv;
   /** Pointer to the command execute function */
   command_fn execute;
};

/**
 * Creates a generic command
 *
 * A command encapsulates the information required to
 * execute a command function.
 *
 * @param data    Private data to be passed to the command execute function
 * @param execute Pointer to the command execute function
 *
 * @return Pointer to a command structure.
 */
struct command_info *command_create(const char *name, void *priv, command_fn execute);

/**
 * Creates a message command
 *
 * A command encapsulates the information required to
 * execute a command function.
 *
 * @param priv    Private attributes
 *
 * @return Pointer to a command structure.
 */

struct command_info *message_command_create(void *priv);

/**
 * Creates a ping pong command
 *
 * A command encapsulates the information required to
 * execute a command function.
 *
 * @param priv    Private attributes
 *
 * @return Pointer to a command structure.
 */
struct command_info *ping_pong_command_create(void *priv);

/**
 * Executes the command function
 *
 * @param cmd command structure.
 * @param req_msg JSON formatted char string with input data
 * @param ret_msg Buffer for return
 */
void command_execute(struct command_info *cmd, char *req_msg, char **ret_msg);

/**
 * Destroys the command
 *
 * @param cmd command structure.
 */
void command_destroy(struct command_info *cmd);


#endif // COMMAND_H_
