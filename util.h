/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int	uint;
typedef unsigned long long   uint64;
typedef char   			*pchar;
typedef unsigned char   *puchar;
typedef int   			*pint;
typedef char			int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;

struct listnode {
    struct listnode *next;
    struct listnode *prev;
};

typedef struct {
    int vid;
    int pid;
    char *name;
    char *desc;
} dev_info_t;

typedef enum drivet_t_ {
    DRI_INVALID = 0,
    DRI_WWAN,
    DRI_GOBI,
    DRI_MHI,
} driver_t;

typedef enum {
    STAT_UNREG,
    STAT_DISCONNECT,
    STAT_IPINVALID,
    STAT_OK,
} EN_NETSTAT;

typedef struct __IPV4 {
    uint32_t Address;
    uint32_t Gateway;
    uint32_t SubnetMask;
    uint32_t DnsPrimary;
    uint32_t DnsSecondary;
    uint32_t Mtu;
} IPV4_T;

typedef struct __IPV6 {
    uchar Address[16];
    uchar Gateway[16];
    uchar SubnetMask[16];
    uchar DnsPrimary[16];
    uchar DnsSecondary[16];
    uchar PrefixLengthIPAddr;
    uchar PrefixLengthGateway;
    uint Mtu;
} IPV6_T;

typedef struct __PROFILE {
    driver_t dri_type;
    dev_info_t *dev_info;
    FILE * logfp;
    char * qmi_dev;
    char * iface;
    char * qmap_iface;
    int qmap_mode;
    int qmap_version;
    char *apn;
    char *user;
    char *password;
    char *pincode;
    int auth;
    int pdp;
    int iPSupported;
    int curIpFamily;
    int rawIP;
    IPV4_T ipv4;
    IPV6_T ipv6;
} PROFILE_T;

#define node_to_item(node, container, member) \
    (container *) (((char*) (node)) - offsetof(container, member))

#define list_declare(name) \
    struct listnode name = { \
        .next = &name, \
        .prev = &name, \
    }

#define list_for_each(node, list) \
    for (node = (list)->next; node != (list); node = node->next)

#define list_for_each_reverse(node, list) \
    for (node = (list)->prev; node != (list); node = node->prev)

void list_init(struct listnode *list);
void list_add_tail(struct listnode *list, struct listnode *item);
void list_add_head(struct listnode *head, struct listnode *item);
void list_remove(struct listnode *item);

#define list_empty(list) ((list) == (list)->next)
#define list_head(list) ((list)->next)
#define list_tail(list) ((list)->prev)

int epoll_register(int  epoll_fd, int  fd, unsigned int events);
int epoll_deregister(int  epoll_fd, int  fd);

typedef int bool;
// memory
void *malloc0(int size);
void qfree(void *p);

// string
bool str_has_prefix(const char *s1, const char *s2);
bool str_has_suffix(const char *s1, const char *s2);
bool str_is_digit(const char *s1, int d, int base);
char *str_dup_until(const char *str, char split);
char *str_get_val_with_key(const char *str, const char *key);
void str_trim(char **str);
void str_ltrim(char **str);
void str_rtrim(char **str);

// directory
bool dir_has_child(const char *dir, const char *fname);
bool dir_get_child(const char *dir, char **child);

#define dir_skip(d, sd) {if (strncasecmp(d, sd, strlen(sd)) == 0) continue;}
/* you have to free memory yourself */
bool file_get_line(const char *fname, int which_line, char **content);
int dir_list(const char *dir, int (*match)(const char *dir, const char *file, void *argv[]), void *argv[]);

const char * get_time(void);
int ql_system(const char *shell_cmd);

/* global variable */
extern bool verbose;
extern bool debug;

/* stream related */
#if defined(DEBUG)
#define log_print(fmt, arg...) do{ dprintf(0, "[%s:%s:%04d:%s] "fmt"\r\n", get_time(), __FILE__, __LINE__, __FUNCTION__, ##arg); }while(0);
#else
#define log_print(fmt, arg...) do{ dprintf(0, "[%s] "fmt"\r\n", get_time(), ##arg); }while(0);
#endif
#define log_debug(fmt, arg...)      {if(debug) log_print("\033[47;30m""Warn: "fmt"\033[0m", ##arg)}
#define log_warn(fmt, arg...)       {log_print("\033[40;33m""Warn: "fmt"\033[0m", ##arg)}
#define log_error(fmt, arg...)      {log_print("\033[40;31m""Error: "fmt"\033[0m", ##arg)}
#define log_fatal(fmt, arg...)      {log_print("\033[41;37m""Fatal: "fmt"\033[0m", ##arg); exit(1);}
#define log_info(fmt, arg...)       {log_print("\033[40;37m"fmt"\033[0m", ##arg)}
#define log_verbose(fmt, arg...)    {if(verbose) log_print("\033[40;37m""Warn: "fmt"\033[0m", ##arg)}

#define copy_bit(u1, u2) {(u1) = (u2);}
#define test_bit(u32, n) {(u32) & 1<<n;}
#define set_bit(u32, n) {(u32) = (u32) | 1<<(n);}
#define clr_bit(u32, n) {(u32) = (u32) & ~(1<<(n));}
#define cln_bit(u32) {(u32) &= 0;}

#define ipv6(u32) (u32 & 1 << IpFamilyV6)
#define ipv4(u32) (u32 & 1 << IpFamilyV4)
#define ipv4v6(u32) (ipv4(u32) && ipv6(u32))

/* macros */
#define MAX_PATH 256
#define true 1
#define false 0
#define ARRAY_SIZE(u) (sizeof(u)/sizeof(u[0]))
#define return_if_fail(cond) {if (!(cond)) return;}
#define return_val_if_fail(cond, val) {if (!(cond)) return (val);}
#define log_return_val_if_fail(cond, val, msg) {if (!(cond)) {log_info(msg); return (val);}}

#endif /* _UTILS_H_ */