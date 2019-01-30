#include "util.h"

/* variables */
const char * quectel_lock = "/var/lock/quectel.pid";

dev_info_t quectel_device_list[] = {
    // vid == 0x1519
    {0x1519, 0x0020, "UG95/UG96", NULL},
    // vid == 0x05c6
    {0x05c6, 0x9090, "UC15", NULL},
    {0x05c6, 0x9003, "UC20", NULL},
    {0x05c6, 0x9215, "EC20", NULL},
    // vid == 0x2c7c
    {0x2c7c, 0x0125, "EC25", NULL},
    {0x2c7c, 0x0121, "EC21", NULL},
    {0x2c7c, 0x0191, "EG91", NULL},
    {0x2c7c, 0x0195, "EG95", NULL},
    {0x2c7c, 0x0306, "EG06/EP06/EM06", NULL},
    {0x2c7c, 0x0296, "BG96", NULL},
    {0x2c7c, 0x0435, "AG35", NULL},

    {0, 0, NULL, NULL},
};

