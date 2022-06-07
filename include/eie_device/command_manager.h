#ifndef COMMAND_MANAGER_H_
#define COMMAND_MANAGER_H_

/**
 * Creates a command manager
 *
 * The command manager is in charge of dealing with the lifecycle
 * of the command types. It creates the initial command from a char
 * string and stores it in a hash table for name-based access.
 *
 * @param name Initial command name to register
 *
 * @return Pointer to a command manager structure.
 */
struct CommandManager *command_manager_create(char *name);

/**
 * Adds a command with its name
 *
 * @param cmdmgr Pointer to a command manager structure.
 * @param name Name of the command to add.
 *
 */
void command_manager_command_add(struct CommandManager *cmdmgr,
                                         const char *name);

/**
 * Removes a command with its name
 *
 * @param cmdmgr Pointer to a command manager structure.
 * @param name Name of the command to remove.
 *
 */
void command_manager_command_remove(struct CommandManager *cmdmgr,
                                         const char *name);

/**
 * Executes a command using its name.
 * It should be destroyed using the command_destroy API.
 *
 * @param cmdmgr Pointer to a command manager structure.
 * @param name Name of the command.
 *
 * @return Pointer to a command structure.
 */
void command_manager_cmd_send(struct CommandManager *cmdmgr,
    const char *name, char *json, char **return_buff);

/**
 * Destroys a command manager
 *
 * Clears memory occupied by command manager
 *
 * @param cmdmgr Pointer to a command manager structure.
 *
 */
void command_manager_destroy(struct CommandManager *cmdmgr);

#endif // COMMAND_MANAGER_H_