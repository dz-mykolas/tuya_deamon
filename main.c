#include "main_utils.h"
#include "tuya_cacert.h"

int running = 1;
void sig_handler(int signum)
{
    printf("\nKILL Signal. Exiting\n");
    running = 0;
}

const char *argp_program_version = "simple_daemon 1.0";
static struct argp argp = { options, parse_opt, 0, "Simple daemon"};

tuya_mqtt_context_t client_instance;
int main(int argc, char **argv)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sigaction(SIGKILL, &sa, NULL);

    // ARGP leaks memory when running 0 arguments with built in options
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
    log_event(5, "Device Connected");
    for(int i = 0; running != 0; i++)
    {
        if (i % 10 == 0) {
            ram_report_free(client, args.device_id);
            i = 0;
        }
        tuya_mqtt_loop(client);
    }
    ret = tuya_mqtt_disconnect(client);
    return ret;
}