#include "QMIThread.h"
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"

//#define CONFIG_EXIT_WHEN_DIAL_FAILED
//#define CONFIG_BACKGROUND_WHEN_GET_IP
//#define CONFIG_PID_FILE_FORMAT "/var/run/quectel-CM-%s.pid" //for example /var/run/quectel-CM-wwan0.pid
#define MAJOR       1
#define MINOR       1
#define REVERSION   46
#define LINK_UP     1
#define LINK_DOWN   0
#define ATTACHED    1
#define DISATTACH   0

PROFILE_T *gstProfile = NULL;
int qmidevice_control_fd[2];
extern dev_info_t quectel_device_list[];

#ifdef CONFIG_BACKGROUND_WHEN_GET_IP
static int daemon_pipe_fd[2];

static void ql_prepare_daemon(void) {
    pid_t daemon_child_pid;

    if (pipe(daemon_pipe_fd) < 0) {
        log_error("%s Faild to create daemon_pipe_fd: %d (%s)", __func__, errno, strerror(errno));
        return;
    }

    daemon_child_pid = fork();
    if (daemon_child_pid > 0) {
        struct pollfd pollfds[] = {{daemon_pipe_fd[0], POLLIN, 0}, {0, POLLIN, 0}};
        int ne, ret, nevents = sizeof(pollfds)/sizeof(pollfds[0]);
        int signo;

        //log_info("father");

        close(daemon_pipe_fd[1]);

        if (socketpair( AF_LOCAL, SOCK_STREAM, 0, signal_control_fd) < 0 ) {
            log_error("%s Faild to create main_control_fd: %d (%s)", __func__, errno, strerror(errno));
            return;
        }

        pollfds[1].fd = signal_control_fd[1];

        while (1) {
            do {
                ret = poll(pollfds, nevents, -1);
            } while ((ret < 0) && (errno == EINTR));

            if (ret < 0) {
                log_error("%s poll=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
                goto __daemon_quit;
            }

            for (ne = 0; ne < nevents; ne++) {
                int fd = pollfds[ne].fd;
                short revents = pollfds[ne].revents;

                if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    //log_info("%s poll err/hup", __func__);
                    //log_info("poll fd = %d, events = 0x%04x", fd, revents);
                    if (revents & POLLHUP)
                        goto __daemon_quit;
                }

                if ((revents & POLLIN) &&  read(fd, &signo, sizeof(signo)) == sizeof(signo)) {
                    if (signal_control_fd[1] == fd) {
                        if (signo == SIGCHLD) {
                            int status;
                            int pid = waitpid(daemon_child_pid, &status, 0);
                            log_info("waitpid pid=%d, status=%x", pid, status);
                            goto __daemon_quit;
                        } else {
                            kill(daemon_child_pid, signo);
                        }
                    } else if (daemon_pipe_fd[0] == fd) {
                        //log_info("daemon_pipe_signo = %d", signo);
                        goto __daemon_quit;
                    }
                }
            }
        }
__daemon_quit:
        //log_info("father exit");
        _exit(0);
    } else if (daemon_child_pid == 0) {
        close(daemon_pipe_fd[0]);
        //log_info("child", getpid());
    } else {
        close(daemon_pipe_fd[0]);
        close(daemon_pipe_fd[1]);
        log_error("%s Faild to create daemon_child_pid: %d (%s)", __func__, errno, strerror(errno));
    }
}

static void ql_enter_daemon(int signo) {
    if (daemon_pipe_fd[1] > 0)
        if (signo) {
            write(daemon_pipe_fd[1], &signo, sizeof(signo));
            sleep(1);
        }
    close(daemon_pipe_fd[1]);
    daemon_pipe_fd[1] = -1;
    setsid();
}
#endif

//uint ifc_get_addr(const char *ifname);

static void qmi_link_change(PROFILE_T *profile, int link) {
    static int s_link = 0;

    if (s_link == link)
        return;

    s_link = link;

    if (link) {
        if (ipv4(profile->iPSupported)) {
            requestGetIPAddress(profile, IpFamilyV4);
        }
        if (ipv6(profile->iPSupported)) {
            requestGetIPAddress(profile, IpFamilyV6);
        }
        udhcpc_start(profile);
    } else {
        udhcpc_stop(profile);
    }

#ifdef LINUX_RIL_SHLIB
    if (link) {
        int timeout = 6;
        while (timeout-- /*&& ifc_get_addr(profile->iface) == 0*/) {
            sleep(1);
        }
    }

    if (link && requestGetIPAddress(profile, 0x04) == 0) {
        unsigned char *r;

        log_info("Using interface %s", profile->iface);
        r = (unsigned char *)&profile->ipv4.Address;
        log_info("local  IP address %d.%d.%d.%d", r[3], r[2], r[1], r[0]);
        r = (unsigned char *)&profile->ipv4.Gateway;
        log_info("remote IP address %d.%d.%d.%d", r[3], r[2], r[1], r[0]);
        r = (unsigned char *)&profile->ipv4.DnsPrimary;
        log_info("primary   DNS address %d.%d.%d.%d", r[3], r[2], r[1], r[0]);
        r = (unsigned char *)&profile->ipv4.DnsSecondary;
        log_info("secondary DNS address %d.%d.%d.%d", r[3], r[2], r[1], r[0]);
    }
#endif

#ifdef CONFIG_BACKGROUND_WHEN_GET_IP
    if (link && daemon_pipe_fd[1] > 0) {
        int timeout = 6;
        while (timeout-- /*&& ifc_get_addr(profile->iface) == 0*/) {
            sleep(1);
        }
        ql_enter_daemon(SIGUSR1);
    }
#endif
}

static int check_ipv4_address(PROFILE_T *profile) {
    PROFILE_T new_profile_v;
    PROFILE_T *new_profile = &new_profile_v;

    memcpy(new_profile, profile, sizeof(PROFILE_T));
    if (requestGetIPAddress(new_profile, 0x04) == 0) {
        if (new_profile->ipv4.Address != profile->ipv4.Address || verbose) {
            unsigned char *l = (unsigned char *)&profile->ipv4.Address;
            unsigned char *r = (unsigned char *)&new_profile->ipv4.Address;
            log_info("localIP: %d.%d.%d.%d VS remoteIP: %d.%d.%d.%d",
                     l[3], l[2], l[1], l[0], r[3], r[2], r[1], r[0]);
        }
        return (new_profile->ipv4.Address == profile->ipv4.Address);
    }
    return 0;
}

// send event from dial thread to qmi device
static void send_to_qmidevice(int triger_event) {
    if (write(SOCKPAIR_DIAL, &triger_event, sizeof(triger_event)) < 0) {
        log_error("%s fail to write", __FUNCTION__);
    }
}

// send event from qmi device to dial thread
void send_to_dial(int triger_event) {
    if (write(SOCKPAIR_QMI, &triger_event, sizeof(triger_event)) < 0) {
        log_error("%s fail to write", __FUNCTION__);
    }
}

pthread_t gQmiThreadID;
pthread_t gQmiProcessTdID;

static void sig_action(int signo) {
    if (SIGCHLD == signo)
        waitpid(-1, NULL, WNOHANG);
    else if (SIGUSR1 == signo) {
        verbose = ~verbose;
    } else {
        if (SIGTERM == signo || SIGHUP == signo || SIGINT == signo) {
        }
        log_info("Main get signal %d, send event 0x%x to dial thread", signo, RIL_REQUEST_QUIT);
        send_to_dial(RIL_REQUEST_QUIT); //main may be wating qmi response
    }
}

static int usage(const char *progname) {
    log_info("[Usage]: %s [options]", progname);
    log_info("");
    log_info("[setting options]");
    log_info("-s, --set=[\"key=val,...\"]                             Set apn/user/password/auth get from your network provider");
    log_info("-I, --id=[1|2|3|4]                                    Which PDN channel is going to be used, only for multiple PDN mode");
    log_info("-i, --interface=interface                             Network interface to be used (default wwan0)");
    log_info("-d, --device                                          Device listed in '/dev/' to be used");
    log_info("-4                                                    Use IPv4 only(default IPv4 only)");
    log_info("-6                                                    Use IPv6 only (you can use both -4 and -6 to use both protocol)");
    log_info("-p, --pin=pincode                                     Verify sim card pin if sim card is locked");
    log_info("[other options]");
    log_info("");
    log_info("-f, --file=logfile                                    Save log message of this program to file");
    log_info("-V, --version                                         Version info");
    log_info("-v, --verbose                                         Verbose");
    log_info("-h, --help                                            Show this message");
    log_info("");
    log_info("[examples]");
    log_info("Example 1: %s ", progname);
    log_info("Example 2: %s -s=\"apn=3gnet\"", progname);
    log_info("Example 3: %s -s=\"apn=3gnet,user=carl,password=1234,auth=0\" --pin 1234 -l quectel_log.txt", progname);
    log_info("");
    log_info("[signal]");
    log_info("killall -10 %-12s                                     Switch on/off debug mode (the same like -v option)", progname+2);
    return 0;
}

static void qmi_show(const PROFILE_T *profile)
{
    return_if_fail(profile != NULL);
    log_info("pdp %d", profile->pdp);
    log_info("apn %s", profile->apn);
    log_info("auth %d", profile->auth);
    log_info("pin %s", profile->pincode);
    log_info("user %s", profile->user);
    log_info("password %s", profile->password);
    log_info("IPv4 (%s), IPv6 (%s)", (profile->curIpFamily == IpFamilyV4)?"enable":"disable",
             (profile->curIpFamily == IpFamilyV6)?"enable":"disable");
}

static void qmi_parser_args(PROFILE_T *profile, int argc, char **argv) {
    int opt = 0;
    int long_idx = 0;

    static struct  option long_options[] = {
        {"set",		required_argument, NULL, 's'},
        {"id",		required_argument, NULL, 'I'},
        {"interface",	required_argument, NULL, 'i'},
        {"device",	required_argument, NULL, 'd'},
        {"4",		no_argument, NULL, '4'},
        {"6",		no_argument, NULL, '6'},
        {"pin", 	required_argument, NULL, 'p'},
        {"file",	required_argument, NULL, 'f'},
        {"version",	no_argument, NULL, 'V'},
        {"verbose",	no_argument, NULL, 'v'},
        {"help",	no_argument, NULL, 'h'},
        {0, 0, 0, 0},
    };

    if (str_has_suffix(argv[argc - 1], "&")) {
        argv[argc - 1][strlen(argv[argc - 1]) - 1] = '\0';
    }

    while ((opt = getopt_long(argc, argv, "s:I:i:d:p:f:46Vvh", long_options, &long_idx)) != -1) {
        switch (opt) {
            log_info("%c", opt);
        case '4':
            set_bit(profile->iPSupported, IpFamilyV4);
            set_bit(profile->curIpFamily, IpFamilyV4);
            break;
        case '6':
            set_bit(profile->iPSupported, IpFamilyV6);
            set_bit(profile->curIpFamily, IpFamilyV6);
            break;
        case 's':
            /* 0 ~ None, 1 ~ Pap, 2 ~ Chap (default), 3 ~ MsChapV2 */
            profile->apn = str_get_val_with_key(optarg, "apn");
            str_trim(&profile->apn);
            if (str_get_val_with_key(optarg, "auth"))
                profile->auth = atoi(str_get_val_with_key(optarg, "auth"));
            else
                profile->auth = 2;
            profile->user = str_get_val_with_key(optarg, "user");
            str_trim(&profile->user);
            profile->password = str_get_val_with_key(optarg, "password");
            str_trim(&profile->password);
            break;
        case 'I':
            profile->pdp = atoi(optarg);
            break;
        case 'i':
            profile->iface = optarg;
            break;
        case 'd':
            profile->qmi_dev = optarg;
            break;
        case 'p':
            profile->pincode = optarg;
            break;
        case 'f':
            profile->logfp = fopen(optarg, "a+");
            if (!profile->logfp) log_error("Fail to open %s, errno: %d(%s)", optarg, errno, strerror(errno));
            break;
        case 'v':
            verbose = true;
            break;
        case 'V':
            log_info("quectel connect manager version %d.%d.%d\n", MAJOR, MINOR, REVERSION);
            exit(0);
        case 'h':
        case '?':
            usage(argv[0]);
            exit(0);
        default:
            log_info("using '-h' to get help");
            exit(0);
        }
    }
    qmi_show(profile);
}

#if 0
static bool is_quectel_device_v1(const char *path) {
    char fname[256] = {0};
    char *vid = NULL;
    char *pid = NULL;
    int idx;

    return_val_if_fail(snprintf(fname, sizeof(fname), "%s/idProduct", path) > 0, false);
    return_val_if_fail(access(fname, R_OK) == 0, false);
    if (file_get_line(fname, 1, &pid) == false) {
        qfree(pid);
        return false;
    }

    return_val_if_fail(snprintf(fname, sizeof(fname), "%s/idVendor", path) > 0, false);
    return_val_if_fail(access(fname, R_OK) == 0, false);
    if (file_get_line(fname, 1, &vid) == false) {
        qfree(pid);
        qfree(vid);
        return false;
    }

    for (idx = 0;; idx++) {
        if (quectel_device_list[idx].vid == 0 && quectel_device_list[idx].pid == 0)
            break;
        if (str_is_digit(vid, quectel_device_list[idx].vid, 16) && str_is_digit(pid, quectel_device_list[idx].pid, 16)) {
            profile->dev_info = &quectel_device_list[idx];
            log_info("Find quectel device (%x:%x) in %s", quectel_device_list[idx].vid, quectel_device_list[idx].pid, path);
            qfree(vid);
            qfree(pid);
            return true;
        }
    }

    qfree(vid);
    qfree(pid);
    return false;
}
#endif

static bool is_quectel_device_v2(const char *path) {
    char fname[256] = {0};
    char *vid = NULL;
    char *pid = NULL;

    return_val_if_fail(snprintf(fname, sizeof(fname), "%s/idProduct", path) > 0, false);
    return_val_if_fail(access(fname, R_OK) == 0, false);
    if (file_get_line(fname, 1, &pid) == false) {
        qfree(pid);
        return false;
    }

    return_val_if_fail(snprintf(fname, sizeof(fname), "%s/idVendor", path) > 0, false);
    return_val_if_fail(access(fname, R_OK) == 0, false);
    if (file_get_line(fname, 1, &vid) == false) {
        qfree(pid);
        qfree(vid);
        return false;
    }

    /* Quectel Vendor ID 0x2c7c */
    if (str_is_digit(vid, 0x2c7c, 16)) {
        log_info("Find quectel device (%s:%s) in %s", vid, pid, path);
        qfree(vid);
        qfree(pid);
        return true;
    }

    qfree(vid);
    qfree(pid);
    return false;
}


static int qmi_qmap_detect(PROFILE_T *profile) {
    char MODE_FILE[128];
    char qmap_iface[128];
    char *qmap_mode = NULL;
    static char iface[32] = "\0";

    // DRIVER GOBINET
    if(profile->dri_type == DRI_GOBI) {
        snprintf(MODE_FILE, sizeof(MODE_FILE), "/sys/class/net/%s/qmap_mode", profile->iface);
        file_get_line(MODE_FILE, 1, &qmap_mode);
        profile->qmap_mode = qmap_mode[0] - '0';

        if (profile->qmap_mode) {
            sprintf(qmap_iface, "/sys/class/net/%s.%d", profile->iface, profile->pdp);
            if (!access(qmap_iface, R_OK)) {
                sprintf(iface, "%s.%d", profile->iface, profile->pdp);
                profile->qmap_iface = iface;
            } else {
                profile->qmap_iface = profile->iface;
            }
            log_info("Find qmap_iface %s", profile->qmap_iface);
        }
    }

    // DRIVER QMI_WWAN
    if(profile->dri_type == DRI_WWAN) {
        if (!access("/sys/module/qmi_wwan/parameters/rx_qmap", R_OK)) {
            sprintf(qmap_iface, "/sys/class/net/%s.%d", profile->iface, profile->pdp);
            if (!access(qmap_iface, R_OK)) {
                sprintf(iface, "%s.%d", profile->iface, profile->pdp);
                profile->qmap_iface = iface;
            } else {
                profile->qmap_iface = profile->iface;
            }
            log_info("Find qmap_iface = %s", profile->qmap_iface);
        }
    }

    // DRIVER QMI_MHI
    if (profile->dri_type == DRI_MHI) {
        profile->qmap_iface = profile->iface;
    }

    return 0;
}

static bool qmi_detect_device(PROFILE_T *profile) {
    const char *dir = NULL;
    char usbdir[255] = {0};
    char subdir[255] = {0};
    char buf[255] = {0};
    char tmp[255] = {0};
    struct dirent* ent = NULL;
    struct dirent* usbent = NULL;
    struct dirent* subent = NULL;
    DIR *pDir = NULL;
    DIR *pusbDir = NULL;
    DIR *psubDir = NULL;

    /* search driver info */
    dir = "/sys/bus/usb/devices";
    pDir = opendir(dir);
    log_return_val_if_fail(pDir != NULL, ENODEV,  "Fail to open directory /sys/bus/usb/devices");
    while ((ent = readdir(pDir)) != NULL) {
        if (ent->d_name[0] == '.' || str_has_prefix(ent->d_name, "usb") == false) {
            continue;
        }

        /* /sys/bus/usb/devices/usbx */
        (void)snprintf(usbdir, sizeof(usbdir), "%s/%s", dir, ent->d_name);
        pusbDir = opendir(usbdir);
        if (pusbDir == NULL) {
            log_error("Opendir %s failed", usbdir);
        }

        while ((usbent = readdir(pusbDir)) != NULL)  {
            /* /sys/bus/usb/devices/usbx/x-x */
            dir_skip(usbent->d_name, ".");
            dir_skip(usbent->d_name, "..");
            (void)snprintf(subdir, sizeof(subdir), "%s/%s", usbdir, usbent->d_name);
            if (is_quectel_device_v2(subdir)) {
                psubDir = opendir(subdir);
                while ((subent = readdir(psubDir)) != NULL) {
                    if (str_has_prefix(subent->d_name, usbent->d_name) == false) continue;
                    /* cdc-wdm0 */
                    (void)snprintf(buf, sizeof(buf), "%s/%s", subdir, subent->d_name);
                    if (dir_has_child(buf, "net")) {
                        snprintf(tmp, ARRAY_SIZE(tmp), "%s/net", buf);
                        dir_get_child(tmp, &profile->iface);
                        log_info("Find interface %s/%s", buf, profile->iface);
                    }

                    // qmi_wwan
                    if (dir_has_child(buf, "usbmisc")) {
                        snprintf(tmp, ARRAY_SIZE(tmp), "%s/usbmisc", buf);
                        dir_get_child(tmp, &profile->qmi_dev);
                        log_info("Find device %s/%s", buf, profile->qmi_dev);
                        profile->dri_type = DRI_WWAN;
                        log_info("Will use driver qmi_wwan");
                    }

                    // gobinet
                    if (dir_has_child(buf, "GobiQMI")) {
                        snprintf(tmp, ARRAY_SIZE(tmp), "%s/GobiQMI", buf);
                        dir_get_child(tmp, &profile->qmi_dev);
                        log_info("Find device %s/%s", buf, profile->qmi_dev);
                        profile->dri_type = DRI_GOBI;
                        log_info("Will use driver GobiNet");
                    }

                    // mhi
                    if (dir_has_child(buf, "usb")) {
                        snprintf(tmp, ARRAY_SIZE(tmp), "%s/usb", buf);
                        dir_get_child(tmp, &profile->qmi_dev);
                        log_info("Find device %s/%s", buf, profile->qmi_dev);
                        profile->dri_type = DRI_MHI;
                        log_info("Will use driver MHI");
                    }
                }
            }
        }
        closedir(psubDir);
    }
    closedir(pDir);

    // qmap
    qmi_qmap_detect(profile);

    if (!profile->qmi_dev || !profile->iface) {
        log_error("1. does your device support QMI?");
        log_error("2. does your device is configured correctly to use QMI driver?");
        log_error("3. what is the response of AT command 'at+qcfg=\"usbnet\"' ?");
        log_error("4. have you modify the qmi_wwan.c following the instruction?");
        log_error("5. does your device vendor ID is 0x2c7c(Quectel)?");
        log_fatal("no QMI device was found!");
    }
    return 0;
}

static int qmi_driver_gobi(PROFILE_T *profile) {
    int ret;

    // apply client ID
    ret = GobiInit(profile);
    if (ret) {
        log_error("%s error occured, will exit", __func__);
    }

    if (pthread_create( &gQmiThreadID, 0, GobiNetThread, (void *)profile) != 0) {
        log_error("%s Failed to create GobiNetThread: %d (%s)", __func__, errno, strerror(errno));
        return -1;
    }

    return 0;
}

static int qmi_driver_wwan(PROFILE_T *profile) {
    if (pthread_create( &gQmiThreadID, 0, QmiWwanThread, (void *)profile) != 0) {
        log_error("%s Failed to create QmiWwanThread: %d (%s)", __func__, errno, strerror(errno));
        return 0;
    }

    if (QmiWwanInit(profile)) {
        log_error("%s Failed to QmiWwanInit: %d (%s)", __func__, errno, strerror(errno));
        return 0;
    }

    return 0;
}

// apply qmi client id
// start qmi message receive thread.
static int qmi_thread_loop(PROFILE_T *profile) {
    int ret = 0;
    char buf[64] = {0};

    // make communication socket
    if (socketpair( AF_LOCAL, SOCK_STREAM, 0, qmidevice_control_fd ) < 0 ) {
        log_error("%s Failed to create qmidevice_control_fd: %d (%s)", __func__, errno, strerror(errno));
        return -EOPNOTSUPP;
    }

    switch (profile->dri_type) {
    case DRI_GOBI:
        ret = qmi_driver_gobi(profile);
        break;
    case DRI_WWAN:
        // cdc_wdm_fd should be set before the thread, otherwise QmiThreadSendQMITimeout may fail
        if (profile->qmap_iface == NULL || profile->qmap_iface == profile->iface) {
            snprintf(buf, ARRAY_SIZE(buf), "/dev/%s", profile->qmi_dev);
            cdc_wdm_fd = open(buf, O_RDWR | O_NONBLOCK | O_NOCTTY);
        } else
            cdc_wdm_fd = qmi_proxy_open(QUECTEL_QMI_PROXY);
        if (cdc_wdm_fd < 0) {
            log_error("open device failed %d(%s)", errno, strerror(errno));
            return -1;
        }
        ret = qmi_driver_wwan(profile);
        break;
    case DRI_MHI:
        ret = qmi_driver_wwan(profile);
        break;
    default:
        log_error("unknow driver type %d", profile->dri_type);
    }

    if (ret) {
        log_error("driver init failed type(%d)", profile->dri_type);
        return -1;
    }

    return 0;
}

// Print info about SIM card
// Set ip family
// Set profile
static void qmi_query_info(PROFILE_T *profile) {
    int ret;
    SIM_Status SIMStatus;

    ret = requestGetSIMStatus(&SIMStatus);
    while (ret == QMI_ERR_OP_DEVICE_UNSUPPORTED) {
        sleep(1);
        ret = requestGetSIMStatus(&SIMStatus);
    }
    if ((SIMStatus == SIM_PIN) && profile->pincode) {
        requestEnterSimPin(profile->pincode);
    }
#ifdef CONFIG_IMSI_ICCID
    if (SIMStatus == SIM_READY) {
        requestGetICCID();
        requestGetIMSI();
    }
#endif

    requestBaseBandVersion(NULL);
}

// Print info about SIM card
// Set ip family
// Set profile
static int qmi_set_profile(PROFILE_T *profile) {

    requestSetEthMode(profile);

#ifdef CONFIG_APN
    if (profile->apn || profile->user || profile->password) {
        requestSetProfile(profile);
    }
    requestGetProfile(profile);
#endif

    return 0;
}

// The function is essential for sometimes we get no response (rx_packets=0)
// but link status is ok.
// if fewer packets was received, print warning message
// It could be a error or probably you didn't send the anthing.
static int qmi_packet_count_check(PROFILE_T *profile) {
    int ret;
    char fname[256];
    int tmp_packets_rx = 0;
    int tmp_packets_tx = 0;
    static int fd_rx = -1;
    static int fd_tx = -1;
    static int rx_packets = 0;
    static int tx_packets = 0;
    static int nochange_times = 0;
    static int amount_rx_packets = 0;
    static int amount_tx_packets = 0;
    // least amount of change per check
    int threshold = 15;
    // amount of check that fewer packets was received
    int threshold_times = 5;

    assert(profile->iface);

    if(fd_rx < 0) {
        sprintf(fname, "/sys/class/net/%s/statistics/rx_packets", profile->iface);
        if((fd_rx= open(fname, O_RDONLY)) < 0) {
            log_error("%s open %s failed", __FUNCTION__, fname);
            return -1;
        }
    }
    if(fd_tx < 0) {
        sprintf(fname, "/sys/class/net/%s/statistics/tx_packets", profile->iface);
        if((fd_tx= open(fname, O_RDONLY)) < 0) {
            log_error("%s open %s failed", __FUNCTION__, fname);
            return -1;
        }
    }

    ret = lseek(fd_rx, 0, SEEK_SET);
    if (ret == -1) {
        log_error("fd_rx(%d) lseek set offset=0 failed errno=%d %s", fd_rx, errno, strerror(errno));
        return -1;
    }

    ret = lseek(fd_tx, 0, SEEK_SET);
    if (ret == -1) {
        log_error("fd_tx(%d) lseek set offset=0 failed errno=%d %s", fd_tx, errno, strerror(errno));
        return -1;
    }

    ret = read(fd_rx, &tmp_packets_rx, sizeof(tmp_packets_rx));
    if (ret == -1) {
        log_error("fd_rx(%d) read failed errno=%d %s", fd_rx, errno, strerror(errno));
        return -1;
    }

    ret = read(fd_tx, &tmp_packets_tx, sizeof(tmp_packets_tx));
    if (ret == -1) {
        log_error("fd_tx(%d) read failed errno=%d %s", fd_tx, errno, strerror(errno));
        return -1;
    }

    if (tmp_packets_tx - rx_packets < threshold) {
        nochange_times++;
        amount_rx_packets += tmp_packets_tx - rx_packets;
        amount_tx_packets += tmp_packets_tx - tx_packets;
        if (amount_tx_packets == amount_rx_packets) {
            amount_rx_packets = 0;
            amount_tx_packets = 0;
            nochange_times = 0;
            return 0;
        }
    } else {
        amount_rx_packets = 0;
        amount_tx_packets = 0;
        nochange_times = 0;
        return 0;
    }
    rx_packets = tmp_packets_rx;
    tx_packets = tmp_packets_tx;

    if (nochange_times >= threshold_times) {
        log_warn("%s received %d send %d packets in last %d check routine",
                 profile->iface, amount_rx_packets, amount_tx_packets, nochange_times);
    }
    return 0;
}

// qmi_check_network
// 1. check connect status
// 2. verify local ip address
// 3. check rx_packets of net interface
static int qmi_check_network(PROFILE_T *profile) {
    uchar  IPConnectionStatus = 0xff; //unknow state
    uchar  IPv6ConnectionStatus = 0xff; //unknow state
    static int oldLinkStatus = QWDS_PKT_DATA_DISCONNECTED;
    uchar PSAttachedState;

    requestRegistrationState(&PSAttachedState);
    if (PSAttachedState == DISATTACH) {
        log_error("Network does not registered");
        return STAT_UNREG;
    }

    if (ipv4(profile->iPSupported))
        requestQueryDataCall(&IPConnectionStatus, IpFamilyV4);
    if (ipv6(profile->iPSupported))
        requestQueryDataCall(&IPv6ConnectionStatus, IpFamilyV6);
    if (oldLinkStatus != IPConnectionStatus) {
        oldLinkStatus = IPConnectionStatus;
        if (IPConnectionStatus == QWDS_PKT_DATA_CONNECTED) {
            qmi_link_change(profile, LINK_UP);
        } else {
            qmi_link_change(profile, LINK_DOWN);
        }
    }
    if (QWDS_PKT_DATA_DISCONNECTED == IPConnectionStatus) {
        return STAT_DISCONNECT;
    }

    //local ip is different with remote ip
    if (QWDS_PKT_DATA_CONNECTED == IPConnectionStatus && check_ipv4_address(profile) == 0) {
        return STAT_IPINVALID;
    }

    // check interface statistic rx_packets
    // sometimes no packet was received
    qmi_packet_count_check(profile);

    if (QWDS_PKT_DATA_CONNECTED != IPConnectionStatus) {
#if defined(ANDROID) || defined(LINUX_RIL_SHLIB)
        kill(getpid(), SIGTERM); //android will setup data call again
#endif
    }

    return STAT_OK;
}

// Start net connect when SIM was registered
static int qmi_start_network(PROFILE_T *profile) {
    int ret;

    log_info("%s", __FUNCTION__);
    ret = requestSetupDataCall(profile, IpFamilyV4);
    if ((ret > 0) && profile->user && profile->user[0] && profile->password && profile->password[0]) {
        int old_auth =	profile->auth;

        //may be fail because wrong auth mode, try pap->chap, or chap->pap
        profile->auth = (profile->auth == 1) ? 2 : 1;
        ret = requestSetupDataCall(profile, IpFamilyV4);

        if (ret)
            profile->auth = old_auth; //still fail, restore old auth moe
    }

    //succssful setup data call
    if (!ret && ipv6(profile->iPSupported)) {
        requestSetupDataCall(profile, IpFamilyV6);
    }

    //try to setup data call 5 seconds later
    if (ret) {
        return -1;
    }

#ifdef CONFIG_EXIT_WHEN_DIAL_FAILED
    kill(getpid(), SIGTERM);
#endif

    return 0;
}

static int qmi_stop_network(PROFILE_T *profile) {

    if (qmi_check_network(profile) == STAT_OK) {
        log_info("Try to stop network");
        requestDeactivateDefaultPDP(profile, IpFamilyV4);
        if (ipv6(profile->iPSupported))
            requestDeactivateDefaultPDP(profile, IpFamilyV6);
    }

    qmi_link_change(profile, LINK_DOWN);
    if (profile->dri_type == DRI_MHI || profile->dri_type == DRI_WWAN)
        QmiWwanDeInit();

    return 0;
}

// check whether there is event for dial thread
// may be signal event from main
// may be qmi control event from qmi driver
static int WaitEvent(void) {
    int ret;
    int event;
    int timeout = 3*1000;
    struct pollfd pollfds[] = {{SOCKPAIR_DIAL, POLLIN, 0}};

__AGAIN:
    // to remove warning
    pollfds[0].fd = SOCKPAIR_DIAL;
    pollfds[0].events = POLLIN;

    ret = poll(pollfds, sizeof(pollfds)/sizeof(pollfds[0]),  timeout);
    if ((ret < 0) && (errno == EINTR)) {
        goto __AGAIN;
    }

    if (ret == 0) {
        timeout = 5*1000;
        return RIL_EVENT_TIMEOUT;
    }

    if (ret < 0) {
        log_error("%s poll=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
        return RIL_REQUEST_QUIT;
    }

    // poll is a LT trigger function.
    // may be more than one event was get so set timeout=1s to read the next event quickly
    timeout = 1000;
    if (pollfds[0].revents & POLLIN) {
        ret = (int)read(pollfds[0].fd, &event, sizeof(event));
        if (ret < 0) {
            log_error("%s read failed ret=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
            return RIL_REQUEST_QUIT;
        }

        return event;
    }

    goto __AGAIN;
}

// a loop thread
// check event from socket pair
// query link status
// set link status
static void * qmi_event_proc(void *pData) {
    int ret;
    int event = 0;
    PROFILE_T *profile = (PROFILE_T*)pData; 
#ifdef CONFIG_RESET_RADIO
    int preRegStat = true;
    struct timeval resetRadioTime = {0};
    struct timeval	nowTime;
#endif

    log_info("%s thread with tid=%ld start", __func__, pthread_self());
    do {
        event = WaitEvent();
        switch (event) {
        // quit event from qmi driver thread
        case RIL_REQUEST_QUIT:
            log_info("%s get event 0x%4x and set disconnect", __FUNCTION__, event);
            goto __quit;
            break;

        case RIL_UNSOL_DATA_CALL_LIST_CHANGED:
        // link status changed
        case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
        // routine to check link status
        case RIL_EVENT_TIMEOUT:
            ret = qmi_check_network(profile);
            if (ret == STAT_IPINVALID) {
                requestDeactivateDefaultPDP(profile, IpFamilyV4);
            }

            if (ret == STAT_UNREG) {
#ifdef CONFIG_RESET_RADIO
                if (preRegStat == true) {
                    gettimeofday(&resetRadioTime, (struct timezone *) NULL);
                }
                preRegStat = false;
                gettimeofday(&nowTime, (struct timezone *) NULL);
                if (abs(nowTime.tv_sec - resetRadioTime.tv_sec) > CONFIG_RESET_RADIO) {
                    resetRadioTime = nowTime;
                    //requestSetOperatingMode(0x06); //same as AT+CFUN=0
                    requestSetOperatingMode(0x01); //same as AT+CFUN=4
                    requestSetOperatingMode(0x00); //same as AT+CFUN=1
                }
#endif
            } else {
                preRegStat = true;
            }

            if (ret != STAT_OK) {

                qmi_start_network(profile);
            }
            break;

        default:
            log_warn("Unrecognized event 0x%4x", event);
            break;
        }
    } while(1);

__quit:
    ret = qmi_stop_network(profile);
    if (ret) {
        log_error("qmi_stop_network failed ret=%d", ret);
    }
    send_to_qmidevice(RIL_REQUEST_QUIT);
    log_info("%s send event 0x%x to qmidevice", __FUNCTION__, RIL_REQUEST_QUIT);
    log_info("%s thread with tid=%ld exit", __func__, pthread_self());
    return 0;
}

static void qmi_cleanup(PROFILE_T *profile) {
    if (pthread_join(gQmiThreadID, NULL)) {
        log_error("%s Error joining to listener thread (%s)", __func__, strerror(errno));
    }

    if (pthread_join(gQmiProcessTdID, NULL)) {
        log_error("%s Error joining to listener thread (%s)", __func__, strerror(errno));
    }

    close(SOCKPAIR_DIAL);
    close(SOCKPAIR_QMI);
    if (profile->logfp) fclose(profile->logfp);

#ifdef CONFIG_PID_FILE_FORMAT
    {
        char cmd[255];
        sprintf(cmd, "rm  " CONFIG_PID_FILE_FORMAT, profile.iface);
        system(cmd);
    }
#endif

    log_info("Done.");
}

static int qmi_run(PROFILE_T *profile) {
    int ret = 0;
    int retryTimes = 0;
    int maxTimes = 10;

#ifdef CONFIG_BACKGROUND_WHEN_GET_IP
    ql_prepare_daemon();
#endif

    if (!profile->iface && !profile->qmi_dev) {
        ret = qmi_detect_device(profile);
        if(ret) {
            log_error("qmi_detect_device failed ret=%d", ret);
            return -1;
        }
    }

    ret = qmi_thread_loop(profile);
    if(ret) {
        log_error("qmi_thread_loop failed ret=%d", ret);
        return -1;
    }

    ret = qmi_set_profile(profile);
    if(ret) {
        log_error("qmi_set_profile failed ret=%d", ret);
        return -1;
    }

#ifdef CONFIG_PID_FILE_FORMAT
    {
        char cmd[255];
        sprintf(cmd, "echo %d > " CONFIG_PID_FILE_FORMAT, getpid(), profile.iface);
        system(cmd);
    }
#endif

    qmi_query_info(profile);

    // try to connect network
    do {
        if (qmi_check_network(profile) != STAT_OK) {
            ret = qmi_start_network(profile);
            if (!ret) {
                break;
            }
        }
        retryTimes++;
        usleep(3*1000);
    } while(retryTimes < maxTimes);

    // do not show check details unless an error occurs
    LOOP_MSG = false;

    // qmi event process thread
    if (pthread_create( &gQmiProcessTdID, 0, qmi_event_proc, (void*)profile) != 0) {
        log_error("%s Failed to create qmi_event_proc: %d (%s)", __func__, errno, strerror(errno));
        return 0;
    }

    // do some cleanup
    qmi_cleanup(profile);

    return 0;
}

static void qmi_banner(void) {
    log_info("WCDMA&LTE_QConnectManager_Linux&Android compiled at %s %s", __DATE__, __TIME__);
    log_warn("WCDMA&LTE_QConnectManager_Linux&Android_V1.1.45");
    log_warn("Make sure you have installed udhcpc and removed ModemManager");
}

#if defined(ANDROID) || defined(LINUX_RIL_SHLIB)
int quectel_CM(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    gstProfile = (PROFILE_T*)malloc(sizeof(PROFILE_T));
    memset(gstProfile, 0x00, sizeof(PROFILE_T));
    gstProfile->pdp = CONFIG_DEFAULT_PDP;

    qmi_parser_args(gstProfile, argc, argv);
    qmi_banner();
    
    signal(SIGINT,  sig_action);
    signal(SIGTERM, sig_action);
    signal(SIGHUP,  sig_action);
    // to switch on/off debug mode
    signal(SIGUSR1, sig_action);
    signal(SIGCHLD, sig_action);

    (void)qmi_run(gstProfile);

    return 0;
}
