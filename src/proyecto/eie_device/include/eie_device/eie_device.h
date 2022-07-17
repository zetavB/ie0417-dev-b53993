#ifndef EIE_DEVICE_H_
#define EIE_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif


/** Structure with MQTT configuration parameters in JSON format */
struct EieDeviceConfig {
    /** JSON with mqtt configuration */
    char * configJson;
};

/**
 * Creates a eie_device
 *
 * The device object allows users to abstract communications throrugh MQTT to broker.
 *
 * @param cfg  Device configuration structure
 *
 * @return Pointer to a eie_device structure
 */
struct EieDevice * eie_device_create(struct EieDeviceConfig *cfg);

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
 * Sends message to broker through MQTT.
 *
 * @param device  Pointer to device to stop.
 * @param msgJson Response data encoded in JSON format.
 *
 * @return Integer with exit code
 */
int eie_device_send_message(struct EieDevice *device, char *msgJson);

#ifdef __cplusplus
}
#endif

#endif // EIE_DEVICE_H_