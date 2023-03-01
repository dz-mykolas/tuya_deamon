#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>

#include "ram_usage.h"
#include "cJSON.h"
#include "tuya_cacert.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"

struct my_args {
  char *productId;
  char *deviceId;
  char *deviceSecret;
};

tuya_mqtt_context_t client_instance;

bool isOnline = true;

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on connected");
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on disconnect");
}

void message_helper(char* s)
{
    if (strcmp(s, "func1") == 0) {
        printf("%s\n", s);
        return;
    }
    if (strcmp(s, "disconnect") == 0) {
        isOnline = false;
        printf("Disconnected\n");
        return;
    }
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    cJSON *json = cJSON_Parse(msg->data_string);
    char *identifier = json->child->string;
    switch (msg->type) {
        case THING_TYPE_PROPERTY_SET:
            TY_LOGI("property set:%s", msg->data_string);
            message_helper(identifier);
            break;
        case THING_TYPE_ACTION_EXECUTE:
            identifier = json->child->next->valuestring;
            message_helper(identifier);
            break;
        default:
            break;
    }
    printf("\r\n");
}

int main(int argc, char **argv)
{
    const char productId[] = "";
    const char deviceId[] = "";
    const char deviceSecret[] = "";
    int ret = OPRT_OK;
    tuya_mqtt_context_t *client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
                                     .host = "m1.tuyacn.com",
                                     .port = 8883,
                                     .cacert = tuya_cacert_pem,
                                     .cacert_len = sizeof(tuya_cacert_pem),
                                     .device_id = deviceId,
                                     .device_secret = deviceSecret,
                                     .keepalive = 60,
                                     .timeout_ms = 2000,
                                     .on_connected = on_connected,
                                     .on_disconnect = on_disconnect,
                                     .on_messages = on_messages});

    assert(ret == OPRT_OK);
    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);
    while (isOnline)
    {
        ram_print_free();
        tuya_mqtt_loop(client);
    }
    tuyalink_thing_property_report(client, deviceId, "Some data");
    ret = tuya_mqtt_disconnect(client);
    return ret;
}
