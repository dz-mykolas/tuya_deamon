#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <argp.h>

#include "daemon_utils.h"
#include "tuya_utils.h"

struct my_args {
  char *product_id;
  char *device_id;
  char *device_secret;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);
void sig_handler(int sig);