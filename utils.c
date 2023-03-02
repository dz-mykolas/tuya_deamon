#include "utils.h"
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <sys/sysinfo.h>

unsigned long int ram_get_free()
{
    struct sysinfo info;
    sysinfo(&info);
    return (info.freeram/1024/1024);
}

void log_info(char *s, int type)
{
    
}

void log_connect()
{
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("example", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    syslog(LOG_NOTICE, "Connected");
    closelog();
}

void log_disconnect()
{
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("example", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    syslog(LOG_WARNING, "Disconnected");
    closelog();
}