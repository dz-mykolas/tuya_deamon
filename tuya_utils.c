#include "tuya_utils.h"

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on connected");
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    cJSON *json = cJSON_Parse(msg->data_string);
    char *identifier = json->child->string;
    switch (msg->type) {
        case THING_TYPE_PROPERTY_SET:
            identifier = json->child->next->valuestring;
            break;
        case THING_TYPE_ACTION_EXECUTE:
            char *data = json->child->child->valuestring;
            char buffer[100];
            if (tuya_write_file(data) == 1) {
                snprintf(buffer, 100, "{\"file_write_error\":{\"value\":true}}");
                log_event(3, "Failed to write to file");
            }
            else {
                snprintf(buffer, 100, "{\"file_write_error\":{\"value\":false}}");
                log_event(5, "Wrote to file");
            }
            tuyalink_thing_property_report(context, msg->device_id, buffer);
            break;
        default:
            break;
    }
    printf("\r\n");
}

void ram_report_free(tuya_mqtt_context_t *context, char* device_id)
{
    unsigned long ram = ram_get_free();
    char buffer[100];
    snprintf(buffer, 100, "{\"ram_free\":{\"value\":\"%ld\"}}", ram);
    tuyalink_thing_property_report(context, device_id, buffer);
}

int tuya_write_file(char *data)
{
    FILE *fptr;
    fptr = fopen("example.txt","w");

    if(fptr == NULL)
    {
        printf("Error!");   
        return 1;           
    }

    fprintf(fptr, "%.*s", 300, data);
    fclose(fptr);
    return 0;
}