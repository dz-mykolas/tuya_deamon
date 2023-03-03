#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "daemon_utils.h"
#include "tuya_utils.h"

struct my_args {
  char *product_id;
  char *device_id;
  char *device_secret;
};

static struct argp_option options[] = {
    {"product", 'z', "KEY", 0, "Product id"},
    {"device", 'x', "KEY", 0, "Device id"},
    {"deviceS", 'c', "KEY", 0, "Device secret"},
    { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);