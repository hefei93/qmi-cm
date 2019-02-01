#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <net/if.h>
#include <errno.h>
#include "QMIThread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>

#include "util.h"

static void ql_set_mtu(const char *ifname, int ifru_mtu) {
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (inet_sock > 0) {
        strcpy(ifr.ifr_name, ifname);

        if (!ioctl(inet_sock, SIOCGIFMTU, &ifr)) {
            if (ifr.ifr_ifru.ifru_mtu != ifru_mtu) {
                log_info("change mtu %d -> %d", ifr.ifr_ifru.ifru_mtu , ifru_mtu);
                ifr.ifr_ifru.ifru_mtu = ifru_mtu;
                ioctl(inet_sock, SIOCSIFMTU, &ifr);
            }
        }

        close(inet_sock);
    }
}

#ifdef ANDROID
static void android_property_set(char *ifname, char *type, uint32_t ipaddr) {
    char shell_cmd[128];
    unsigned char *r = (unsigned char *)&ipaddr;

    snprintf(shell_cmd, sizeof(shell_cmd), "/system/bin/setprop net.%s.%s %d.%d.%d.%d", ifname, type, r[3], r[2], r[1], r[0]);
    ql_system(shell_cmd);
}
#endif

static void* udhcpc_thread_function(void* arg) {
    FILE * udhcpc_fp;
    char *udhcpc_cmd = (char *)arg;

    if (udhcpc_cmd == NULL)
        return NULL;

    log_info("%s", udhcpc_cmd);
    udhcpc_fp = popen(udhcpc_cmd, "r");
    qfree((void*)&udhcpc_cmd);
    if (udhcpc_fp) {
        char buf[0xff];

        buf[sizeof(buf)-1] = '\0';
        while((fgets(buf, sizeof(buf)-1, udhcpc_fp)) != NULL) {
            if ((strlen(buf) > 1) && (buf[strlen(buf) - 1] == '\n'))
                buf[strlen(buf) - 1] = '\0';
            log_info("%s", buf);
        }

        pclose(udhcpc_fp);
    }

    return NULL;
}

//#define USE_DHCLIENT
#ifdef USE_DHCLIENT
static int dhclient_alive = 0;
#endif
static int dibbler_client_alive = 0;


//for bridge mode, only one public IP, so donot run udhcpc to obtain
static void qmi_bridge_set(PROFILE_T *profile) {
    char BRIDGE_MODE_FILE[128];
    char BRIDGE_IPV4_FILE[128];
    char shell_cmd[128];
    struct in_addr addr = {htobe32(profile->ipv4.Address)};
    int bridge_fd;
    char bridge_mode[2] = {0, 0};
    char *ifname = profile->iface;

    if (profile->qmap_iface) {
        ifname = profile->qmap_iface;
    }

    if (profile->dri_type == DRI_GOBI) {
        strcpy(BRIDGE_MODE_FILE , "/sys/module/qmi_wwan/parameters/bridge_mode");
        strcpy(BRIDGE_IPV4_FILE, "/sys/module/qmi_wwan/parameters/bridge_ipv4");
    } else {
        snprintf(BRIDGE_MODE_FILE, sizeof(BRIDGE_MODE_FILE), "/sys/class/net/%s/bridge_mode", ifname);
        snprintf(BRIDGE_IPV4_FILE, sizeof(BRIDGE_IPV4_FILE), "/sys/class/net/%s/bridge_ipv4", ifname);
    }

    if (profile->ipv4.Address && !access(BRIDGE_MODE_FILE, R_OK)) {
        bridge_fd = open(BRIDGE_MODE_FILE, O_RDONLY);
        if (bridge_fd <= 0) {
            log_error("fail to open %s", BRIDGE_MODE_FILE);
            return;
        }

        if (read(bridge_fd, &bridge_mode, sizeof(bridge_mode)) < 0) {
            log_error("Fail to read %s", BRIDGE_MODE_FILE);
            close(bridge_fd);
            return;
        }

        if(bridge_mode[0] != '0') {
            snprintf(shell_cmd, sizeof(shell_cmd), "echo 0x%08x > %s", profile->ipv4.Address, BRIDGE_IPV4_FILE);
            ql_system(shell_cmd);
            log_info("client IP address is %s (0x%x)\n", inet_ntoa(addr), profile->ipv4.Address);
            close(bridge_fd);
            return;
        }
    }
}

static void udhcpc_set_ipv4(PROFILE_T * profile) {
    char udhcpc_cmd[128];
    pthread_attr_t udhcpc_thread_attr;
    pthread_t udhcpc_thread_id;
    char *ifname = profile->iface;

    if (profile->qmap_iface) {
        ifname = profile->qmap_iface;
    }

    pthread_attr_init(&udhcpc_thread_attr);
    pthread_attr_setdetachstate(&udhcpc_thread_attr, PTHREAD_CREATE_DETACHED);

#ifdef USE_DHCLIENT
    snprintf(udhcpc_cmd, sizeof(udhcpc_cmd), "dhclient -4 -d --no-pid %s", ifname);
    dhclient_alive++;
#else
    if (access("/usr/share/udhcpc/default.script", F_OK|X_OK)) {
        log_warn("Fail to access /usr/share/udhcpc/default.script, errno: %d (%s)", errno, strerror(errno));
        log_warn("Warning: udhcpc on embedded devices may use /usr/share/udhcpc/default.script as default script");
    }

    //-f,--foreground	 Run in foreground
    //-b,--background	 Background if lease is not obtained
    //-n,--now		  Exit if lease is not obtained
    //-q,--quit 	   Exit after obtaining lease
    //-t,--retries N		Send up to N discover packets (default 3)
    snprintf(udhcpc_cmd, sizeof(udhcpc_cmd), "busybox udhcpc -f -n -q -t 5 -i %s", ifname);
#endif

#if 1 //for OpenWrt
    if (!access("/lib/netifd/dhcp.script", X_OK) && !access("/sbin/ifup", X_OK) && !access("/sbin/ifstatus", X_OK)) {
        log_info("you are use OpenWrt?");
        log_info("should not calling udhcpc manually?");
        log_info("should modify /etc/config/network as below?");
        log_info("config interface wan");
        log_info("\toption ifname	%s", ifname);
        log_info("\toption proto	dhcp");
        log_info("should use \"/sbin/ifstaus wan\" to check %s 's status?", ifname);
    }
#endif

    pthread_create(&udhcpc_thread_id, &udhcpc_thread_attr, udhcpc_thread_function, (void*)strdup(udhcpc_cmd));
    sleep(1);
}

static void udhcpc_set_ipv6(PROFILE_T *profile) {
    char udhcpc_cmd[128];
    pthread_attr_t udhcpc_thread_attr;
    pthread_t udhcpc_thread_id;
    char shell_cmd[128];
    char *ifname = profile->iface;

    if (profile->qmap_iface) {
        ifname = profile->qmap_iface;
    }

#ifdef USE_DHCLIENT
    snprintf(udhcpc_cmd, sizeof(udhcpc_cmd), "dhclient -6 -d --no-pid %s",	ifname);
    dhclient_alive++;
#else
    /*
    DHCPv6: Dibbler - a portable DHCPv6
    1. download from http://klub.com.pl/dhcpv6/
    2. cross-compile
    	2.1 ./configure --host=arm-linux-gnueabihf
    	2.2 copy dibbler-client to your board
    3. mkdir -p /var/log/dibbler/ /var/lib/ on your board
    4. create /etc/dibbler/client.conf on your board, the content is
    	log-mode short
    	log-level 7
    	iface wwan0 {
    		ia
    		option dns-server
    	}
     5. run "dibbler-client start" to get ipV6 address
     6. run "route -A inet6 add default dev wwan0" to add default route
    */
    snprintf(shell_cmd, sizeof(shell_cmd), "route -A inet6 add default %s", ifname);
    ql_system(shell_cmd);
    snprintf(udhcpc_cmd, sizeof(udhcpc_cmd), "dibbler-client run");
    dibbler_client_alive++;
#endif

    pthread_create(&udhcpc_thread_id, &udhcpc_thread_attr, udhcpc_thread_function, (void*)strdup(udhcpc_cmd));
}

void udhcpc_start(PROFILE_T *profile) {
    char shell_cmd[128];
    char *ifname = profile->iface;

    if (profile->qmap_iface) {
        ifname = profile->qmap_iface;
    }

    if (profile->rawIP && profile->ipv4.Address && profile->ipv4.Mtu) {
        ql_set_mtu(ifname, (profile->ipv4.Mtu));
    }

#ifdef ANDROID
    if(!access("/system/bin/netcfg", F_OK)) {
        snprintf(shell_cmd, sizeof(shell_cmd), "/system/bin/netcfg %s up", ifname);
        ql_system(shell_cmd);
        snprintf(shell_cmd, sizeof(shell_cmd), "/system/bin/netcfg %s dhcp", ifname);
        ql_system(shell_cmd);
    } else {
        // snprintf(shell_cmd, sizeof(shell_cmd), "/system/bin/dhcptool %s", ifname);
        unsigned char *r = (unsigned char *)&profile->ipv4.Address;
        unsigned char *m = (unsigned char *)&profile->ipv4.SubnetMask;

        snprintf(shell_cmd, sizeof(shell_cmd), "/system/bin/ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d",
                 ifname, r[3],r[2], r[1], r[0], m[3],m[2], m[1], m[0]);
        ql_system(shell_cmd);

        android_property_set(ifname, "dns1", profile->ipv4.DnsPrimary);
        android_property_set(ifname, "dns2", profile->ipv4.DnsSecondary);
    }

    android_property_set(ifname, "gw", profile->ipv4.Gateway);
    return;
#endif

    snprintf(shell_cmd, sizeof(shell_cmd) - 1, "ifconfig %s up", ifname);
    ql_system(shell_cmd);

    qmi_bridge_set(profile);

    if (profile->ipv4.Address) {
        udhcpc_set_ipv4(profile);
    }
    if (profile->ipv6.Address[0] && profile->ipv6.PrefixLengthIPAddr) {
        udhcpc_set_ipv6(profile);
    }
}

void udhcpc_stop(PROFILE_T *profile) {
    char *ifname = profile->iface;
    char shell_cmd[128];
    char reset_ip[128];
    int ret = 0;

    if (profile->qmap_iface) {
        ifname = profile->qmap_iface;
    }

#ifdef ANDROID
    if(!access("/system/bin/netcfg", F_OK)) {
        snprintf(shell_cmd, sizeof(shell_cmd) - 1, "/system/bin/netcfg %s down", ifname);
    } else {
        snprintf(shell_cmd, sizeof(shell_cmd) - 1, "ifconfig %s down", ifname); //for android 6.0 and above
    }
#else
#ifdef USE_DHCLIENT
    if (dhclient_alive) {
        ret = system("killall dhclient");
        if (!(ret != -1 && WIFEXITED(ret) && 0 == WEXITSTATUS(ret))) {
            log_error("Fail  to execute %s", "killall dhclient");
        }
        dhclient_alive = 0;
    }
#endif
    if (dibbler_client_alive) {
        ret = system("killall dibbler-client");
        if (!(ret != -1 && WIFEXITED(ret) && 0 == WEXITSTATUS(ret))) {
            log_error("Fail  to execute %s", "killall dibbler-client");
        }
        dibbler_client_alive = 0;
    }
    snprintf(shell_cmd, sizeof(shell_cmd) - 1, "ifconfig %s down", ifname);
#endif
    ql_system(shell_cmd);
    snprintf(reset_ip, sizeof(reset_ip) - 1, "ifconfig %s 0.0.0.0", ifname);
    ql_system(reset_ip);
}
