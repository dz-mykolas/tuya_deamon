#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <argp.h>

#include "daemon_utils.h"
#include "tuya_utils.h"

struct arguments {
  char *args[3];
};

error_t parse_opt(int key, char *arg, struct argp_state *state);
void sig_handler(int sig);
int directory_get_current(char *cwd, int size);