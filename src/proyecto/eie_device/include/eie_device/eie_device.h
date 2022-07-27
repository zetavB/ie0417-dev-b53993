#ifndef EIE_DEVICE_H_
#define EIE_DEVICE_H_


/** Type of the function that a device can execute */
typedef void (*device_fn)(const char *name, char *features, char *resp_msg);

/** Structure holding callback function and function name */
struct FunctionInfo {
    const char *name;
    device_fn execute;
};

/** Structure with MQTT configuration parameters in JSON format */
struct EieDeviceConfig {
    /** JSON with mqtt configuration */
    char *configJson;
};

/**
 * Creates a struct containing function callback pointers
 * 
 * @param name Name of the function to create
 * 
 * @param execute Pointer to callback function
 * 
 * @return Pointer to FunctionInfo structure
 * 
 */
struct FunctionInfo *info_create(const char *name, device_fn execute);

/**
 * Creates a eie_device
 *
 * The device object allows users to abstract communications throrugh MQTT to broker.
 *
 * @param cfg  Device configuration structure
 * 
 * @param function Pointer to initial callback function
 *
 * @return Pointer to a eie_device structure
 */
struct EieDevice * eie_device_create(struct EieDeviceConfig *cfg, struct FunctionInfo *function);

/**
 * Destroys a eie_device.
 *
 * @param device  Pointer to device to destroy.
 *
 * @return Integer with exit code.
 */
int eie_device_destroy(struct EieDevice *device);

/**
 * Starts MQTT comms and begins monitoring for recieved messages.
 *
 * @param device  Pointer to device to start.
 *
 * @return Integer with exit code.
 */
int eie_device_start(struct EieDevice *device);

/**
 * Stopps MQTT comms and disconnects.
 *
 * @param device  Pointer to device to start.
 *
 * @return Integer with exit code.
 */
int eie_device_stop(struct EieDevice *device);

/**
 * Sends message to broker through MQTT.
 *
 * @param device  Pointer to device to stop.
 * @param msgJson Response data encoded in JSON format.
 *
 * @return Integer with exit code
 */
int eie_device_send_message(struct EieDevice *device, char *msgJson);

#endif // EIE_DEVICE_H_