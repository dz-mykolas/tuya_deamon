#include "main_utils.h"
#include "tuya_cacert.h"

#define LOGS_ERROR 3
#define LOGS_WARNING 4
#define LOGS_NOTICE 5
#define MAX_DIR_LENGTH 4096

const char *argp_program_version = 
    "simple_daemon 1.0";
const char *argp_program_bug_address = 
    "<none>";
static char doc[] = 
    "Simple daemon that connects to Tuya Cloud using it's C SDK";
static char args_doc[] = 
    "product_id device_id device_secret";
static struct argp_option options[] = {
    { 0 }
};
static struct argp argp = { options, parse_opt, args_doc, "Simple daemon"};

tuya_mqtt_context_t client_instance;

int running = 1;
char cwd[MAX_DIR_LENGTH];
int main(int argc, char **argv)
{
    if (directory_get_current(cwd, MAX_DIR_LENGTH) != 0)
        exit(1);

    // Signal handling
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    
    // Parsing arguments
    struct arguments arguments = { 0 };
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    printf("    Product ID = %s\n", arguments.args[0]);
    printf("     Device ID = %s\n", arguments.args[1]);
    printf(" Device Secret = %s\n", arguments.args[2]);

    // Create daemon
    int ret;
    // ret = become_daemon(0);
    // if (ret != OPRT_OK) {
    //     log_event(LOGS_ERROR, "Error while starting daemon!");
    //     return EXIT_FAILURE;
    // }

    // Create client instance and connect to Tuya
    tuya_mqtt_context_t *client = &client_instance;
    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
                                     .host = "m1.tuyacn.com",
                                     .port = 8883,
                                     .cacert = tuya_cacert_pem,
                                     .cacert_len = sizeof(tuya_cacert_pem),
                                     .device_id = arguments.args[1],
                                     .device_secret = arguments.args[2],
                                     .keepalive = 60,
                                     .timeout_ms = 2000,
                                     .on_connected = on_connected,
                                     .on_disconnect = on_disconnect,
                                     .on_messages = on_messages});
    ret = tuya_mqtt_connect(client);
    if (ret != OPRT_OK) {
        log_event(LOGS_ERROR, "Error while connecting!");
        tuya_mqtt_deinit(client);
        return EXIT_FAILURE;
    }
    log_event(LOGS_NOTICE, "Device Connected");
    
    // Client keep-alive
    for(int i = 0; running != 0; i++)
    {
        if (i % 10 == 0) {
            ram_report_free(client, arguments.args[2]);
            i = 0;
        }
        tuya_mqtt_loop(client);
    }

    // Disconnect and deinitialize
    ret = tuya_mqtt_disconnect(client);
    if (ret != OPRT_OK) {
        log_event(LOGS_ERROR, "Error while disconnecting!");

    } else {
        log_event(LOGS_WARNING, "Device Disconnected");
    }
    ret = tuya_mqtt_deinit(client);
    return ret;
}