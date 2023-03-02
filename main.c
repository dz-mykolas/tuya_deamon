#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>

#include "utils.h"
#include "cJSON.h"
#include "tuya_cacert.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"

const char *argp_program_version = "simple_daemon 1.0";


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

bool message_helper(char* s)
{
    if (strcmp(s, "func1") == 0) {
        printf("%s\n", s);
        return true;
    }
    if (strcmp(s, "disconnect") == 0) {
        isOnline = false;
        printf("Disconnected\n");
        return true;
    }
    return false;
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    cJSON *json = cJSON_Parse(msg->data_string);
    char *identifier = json->child->string;
    switch (msg->type) {
        case THING_TYPE_PROPERTY_SET:
            TY_LOGI("property set:%s", msg->data_string);
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

struct my_args {
  char *product_id;
  char *device_id;
  char *device_secret;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we know is a pointer to our arguments structure. */
    struct my_args *args = state->input;

    switch (key) {
        case 'z':
            args->product_id = arg;
            break;
        case 'x':
            args->device_id = arg;
            break;
        case 'c':
            args->device_secret = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* The options we understand. */
static struct argp_option options[] = {
    {"product", 'z', "KEY", 0, "Product id"},
    {"device", 'x', "KEY", 0, "Device id"},
    {"deviceS", 'c', "KEY", 0, "Device secret"},
    { 0 }
};

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, "Simple daemon"};

int main(int argc, char **argv)
{
    // ARGP leaks memory when 0 arguments with built in options
    // https://sourceware.org/bugzilla/show_bug.cgi?id=19039

    struct my_args args = { 0 };

    argp_parse(&argp, argc, argv, 0, 0, &args);

    printf("    Product ID = %s\n", args.product_id);
    printf("     Device ID = %s\n", args.device_id);
    printf(" Device Secret = %s\n", args.device_secret);

    int ret = OPRT_OK;
    tuya_mqtt_context_t *client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
                                     .host = "m1.tuyacn.com",
                                     .port = 8883,
                                     .cacert = tuya_cacert_pem,
                                     .cacert_len = sizeof(tuya_cacert_pem),
                                     .device_id = args.device_id,
                                     .device_secret = args.device_secret,
                                     .keepalive = 60,
                                     .timeout_ms = 2000,
                                     .on_connected = on_connected,
                                     .on_disconnect = on_disconnect,
                                     .on_messages = on_messages});

    assert(ret == OPRT_OK);
    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);
    int i = 0;
    while (isOnline)
    {
        log_connect();
        if (i % 10 == 0) {
            unsigned long ram = ram_get_free();
            char buffer[100];
            snprintf(buffer, 100, "{\"ram_free\":{\"value\":\"%ld\"}}", ram);
            tuyalink_thing_property_report(client, args.device_id, buffer);
        }
        tuya_mqtt_loop(client);
        i++;
    }
    ret = tuya_mqtt_disconnect(client);
    return ret;
}