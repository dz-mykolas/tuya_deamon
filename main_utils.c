#include "main_utils.h"

error_t parse_opt(int key, char *arg, struct argp_state *state) 
{
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
        case ARGP_KEY_ARG:
            if (state->arg_num > 3) {
                argp_usage(state);
            }
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 3) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

extern int running;
void sig_handler(int sig)
{
    switch (sig) {
    case SIGINT:
        printf("\nCTRL+C. Exiting\n");
        running = 0;
        break;
    case SIGTERM:
        printf("\nKILL Signal. Exiting\n");
        running = 0;
        break;
    }
}