#include "daemon_utils.h"
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

void log_event(int type, char *log)
{   
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("tuya_daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    char buffer[100];
    snprintf(buffer, 100, "%s", log);
    syslog(type, "%s", buffer);
    closelog();
}
