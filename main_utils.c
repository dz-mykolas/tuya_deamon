#include "main_utils.h"

static error_t parse_opt(int key, char *arg, struct argp_state *state) 
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
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}