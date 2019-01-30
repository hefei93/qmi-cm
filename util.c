#include "QMIThread.h"
#include <sys/time.h>
#include "util.h"

#if defined(__STDC__)
#include <stdarg.h>
#define __V(x)	x
#else
#include <varargs.h>
#define __V(x)	(va_alist) va_dcl
#define const
#define volatile
#endif

#ifdef ANDROID
#define LOG_TAG "NDIS"
#include "../ql-log.h"
#else
#include <syslog.h>
#endif
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

bool verbose = false;
#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif

#ifndef ANDROID //defined in atchannel.c
static void setTimespecRelative(struct timespec *p_ts, long long msec) {
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *) NULL);

    /* what's really funny about this is that I know
       pthread_cond_timedwait just turns around and makes this
       a relative time again */
    p_ts->tv_sec = tv.tv_sec + (msec / 1000);
    p_ts->tv_nsec = (tv.tv_usec + (msec % 1000) * 1000L ) * 1000L;
}

int pthread_cond_timeout_np(pthread_cond_t *cond,
                            pthread_mutex_t * mutex,
                            unsigned msecs) {
    if (msecs != 0) {
        struct timespec ts;
        setTimespecRelative(&ts, msecs);
        return pthread_cond_timedwait(cond, mutex, &ts);
    } else {
        return pthread_cond_wait(cond, mutex);
    }
}
#endif

const char * get_time(void) {
    static char time_buf[50];
    struct timeval  tv;
    time_t time;
    suseconds_t millitm;
    struct tm *ti;

    gettimeofday (&tv, NULL);

    time= tv.tv_sec;
    millitm = (tv.tv_usec + 500) / 1000;

    if (millitm == 1000) {
        ++time;
        millitm = 0;
    }

    ti = localtime(&time);
    sprintf(time_buf, "%02d-%02d_%02d:%02d:%02d:%03d", ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec, (int)millitm);
    return time_buf;
}

const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

ushort le16_to_cpu(ushort v16) {
    ushort tmp = v16;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v16);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[1];
        d[1] = s[0];
    }
    return tmp;
}

uint  le32_to_cpu (uint v32) {
    uint tmp = v32;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}

uint ql_swap32(uint v32) {
    uint tmp = v32;
    {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}

ushort cpu_to_le16(ushort v16) {
    ushort tmp = v16;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v16);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[1];
        d[1] = s[0];
    }
    return tmp;
}

uint cpu_to_le32 (uint v32) {
    uint tmp = v32;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}

// memory
void *malloc0(int size) {
    void * p = NULL;

    return_val_if_fail(size < 0, NULL);
    p = malloc(size);
    log_return_val_if_fail(p != NULL, NULL, "out of memory");
    bzero(p, size);
    return p;
}

void qfree(void *p) {
    if (p) free(p);
}

// string
bool str_has_prefix(const char *s1, const char *s2) {
    return_val_if_fail(s1 != NULL, false);
    return_val_if_fail(strncmp(s1, s2, strlen(s2)) == 0, false);
    return true;
}

bool str_has_suffix(const char *s1, const char *s2) {
    return_val_if_fail(s1 != NULL, false);
    return_val_if_fail(strncmp(s1 + strlen(s1) - strlen(s2), s2, strlen(s2)) == 0, false);
    return true;
}

bool str_is_digit(const char *s1, int d, int base) {
    char buf[64] = {0};

    return_val_if_fail(s1 != NULL, false);
    if (base == 16) {
        snprintf(buf, ARRAY_SIZE(buf), "%04x", d);
        if (str_has_prefix(s1, "0x")) {
            return_val_if_fail(strncasecmp(s1 + 2, buf, strlen(buf)) == 0, false);
        } else {
            return_val_if_fail(strncasecmp(s1, buf, strlen(buf)) == 0, false);
        }
        return true;
    }

    return false;
}

/* if split charater does not exist, copy util '\0' */
char *str_dup_until(const char *str, char split) {
    char *pos = NULL;
    return_val_if_fail(str != NULL, NULL);
    pos = index(str, split);
    return_val_if_fail(pos != NULL, strdup(str));
    return strndup(str, pos - str);
}

/* "apn=3gnet,user=carl,password=123" */
/* */
char *str_get_val_with_key(const char *str, const char *key) {
    char *substr = NULL;
    return_val_if_fail(str != NULL && key != NULL, NULL);
    substr = strstr(str, key);
    return_val_if_fail(substr != NULL, NULL);
    substr += strlen(key);
    while(isspace(*substr) || *substr == '=') {
        substr += 1;
    }
    return str_dup_until(substr, ',');
}

void str_rtrim(char **str) {
    int idx = 0;
    return_if_fail(*str != NULL);
    idx = strlen(*str) - 1;
    while (idx >= 0) {
        if (isalpha(*(*str + idx))) {
            *(*str + idx + 1) = '\0';
            return;
        }
        idx--;
    }
}

void str_ltrim(char **str) {
    int flag = false;
    int from = 0;
    int to = 0;
    return_if_fail(*str != NULL);
    while (from < strlen(*str)) {
        if (isspace(*(*str + from)) && flag == false) {
            from++;
        } else {
            if (from == to) break;
            flag = true;
            *(*str + to) = *(*str + from);
            to++;
            from++;
        }
    }
}

void str_trim(char **str) {
    str_rtrim(str);
    str_ltrim(str);
}

// directory
bool dir_has_child(const char *dir, const char *fname) {
    char buf[256] = {0};

    return_val_if_fail(dir != NULL, false);
    return_val_if_fail(fname != NULL, false);
    (void)snprintf(buf, sizeof(buf), "%s/%s", dir, fname);
    return_val_if_fail(access(buf, F_OK) == 0, false);
    return true;
}

bool dir_get_child(const char *dir, char **child) {
    struct dirent* ent = NULL;
    DIR *pdir = NULL;

    return_val_if_fail(dir != NULL, false);
    pdir = opendir(dir);
    if (pdir == NULL) {
        log_error("fail to open dir %s", dir);
        return false;
    }
    while((ent = readdir(pdir)) != NULL) {
        dir_skip(ent->d_name, ".");
        dir_skip(ent->d_name, "..");
        *child = strdup(ent->d_name);
        return true;
    }
    return false;
}

/* you have to free memory yourself */
bool file_get_line(const char *fname, int which_line, char **content) {
    FILE *stream = NULL;
    size_t len;
    int idx = 0;

    return_val_if_fail(fname != NULL, false);

    stream = fopen(fname, "r");
    if (stream == NULL) {
        log_error("fail to open %s (%d)%s\n", fname, errno, strerror(errno));
        return false;
    }

    while (getline(content, &len, stream) != -1) {
        idx++;
        if (idx == which_line) {
            fclose(stream);
            return true;
        }
    }
    fclose(stream);
    return false;
}

int dir_list(const char *dir, int (*match)(const char *dir, const char *file, void *argv[]), void *argv[]) {
    DIR *pDir;
    struct dirent* ent = NULL;
    int match_times = 0;

    pDir = opendir(dir);
    if (pDir == NULL)  {
        log_error("Cannot open directory: %s, errno: %d (%s)", dir, errno, strerror(errno));
        return 0;
    }

    while ((ent = readdir(pDir)) != NULL)  {
        match_times += match(dir, ent->d_name, argv);
    }
    closedir(pDir);

    return match_times;
}

int ql_system(const char *shell_cmd) {
    int ret = 0;
    log_info("%s", shell_cmd);
    ret = system(shell_cmd);
    if (ret == -1) {
        log_error("Fail to system(\"%s\") = %d, errno: %d (%s)", shell_cmd, ret, errno, strerror(errno));
    } else {
        if (WIFEXITED(ret)) {
            if (0 == WEXITSTATUS(ret))
                return 0;

            log_error("Fail to system(\"%s\") = %d, errno: %d (%s)", shell_cmd, WEXITSTATUS(ret), errno, strerror(errno));
            return -1;
        }
        log_error("ql_system: exit status = [%d]\n", WEXITSTATUS(ret));
        return -1;
    }
    return ret;
}

