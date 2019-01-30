#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include "QMIThread.h"
#include "util.h"

#ifdef CONFIG_GOBINET

// IOCTL to generate a client ID for this service type
#define IOCTL_QMI_GET_SERVICE_FILE 0x8BE0 + 1

// IOCTL to get the VIDPID of the device
#define IOCTL_QMI_GET_DEVICE_VIDPID 0x8BE0 + 2

// IOCTL to get the MEID of the device
#define IOCTL_QMI_GET_DEVICE_MEID 0x8BE0 + 3

int GobiNetSendQMI(PQCQMIMSG pRequest) {
    int ret, fd;
    ssize_t nwrites;

    fd = qmiclientId[pRequest->QMIHdr.QMIType];

    if (fd <= 0) {
        log_info("%s QMIType: %d has no clientID", __func__, pRequest->QMIHdr.QMIType);
        return -ENODEV;
    }

    // Always ready to write
    nwrites = le16_to_cpu(pRequest->QMIHdr.Length) + 1 - sizeof(QCQMI_HDR);
    ret = write(fd, &pRequest->MUXMsg, nwrites);
    if (ret == nwrites) {
        ret = 0;
    } else {
        log_error("%s write=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
    }

    return ret;
}

static int GobiNetGetClientID(const char *qcqmi, uchar QMIType) {
    int ClientId;
    ClientId = open(qcqmi, O_RDWR | O_NONBLOCK | O_NOCTTY);
    if (ClientId == -1) {
        log_error("failed to open %s, errno: %d (%s)", qcqmi, errno, strerror(errno));
        return -1;
    }
    fcntl(cdc_wdm_fd, F_SETFD, FD_CLOEXEC) ;

    if (ioctl(ClientId, IOCTL_QMI_GET_SERVICE_FILE, QMIType) != 0) {
        log_error("failed to get ClientID for 0x%02x errno: %d (%s)", QMIType, errno, strerror(errno));
        close(ClientId);
        ClientId = 0;
    }

    switch (QMIType) {
    case QMUX_TYPE_WDS:
        log_info("Get clientWDS = %d", ClientId);
        break;
    case QMUX_TYPE_DMS:
        log_info("Get clientDMS = %d", ClientId);
        break;
    case QMUX_TYPE_NAS:
        log_info("Get clientNAS = %d", ClientId);
        break;
    case QMUX_TYPE_QOS:
        log_info("Get clientQOS = %d", ClientId);
        break;
    case QMUX_TYPE_WMS:
        log_info("Get clientWMS = %d", ClientId);
        break;
    case QMUX_TYPE_PDS:
        log_info("Get clientPDS = %d", ClientId);
        break;
    case QMUX_TYPE_UIM:
        log_info("Get clientUIM = %d", ClientId);
        break;
    case QMUX_TYPE_WDS_ADMIN:
        log_info("Get clientWDA = %d", ClientId);
        break;
    default:
        break;
    }

    return ClientId;
}

int GobiNetDeInit(void) {
    unsigned int i;
    for (i = 0; i < sizeof(qmiclientId)/sizeof(qmiclientId[0]); i++) {
        if (qmiclientId[i] != 0) {
            close(qmiclientId[i]);
            qmiclientId[i] = 0;
        }
    }

    return 0;
}

int GobiInit(PROFILE_T *profile) {
    const char *qcqmi = (const char *)profile->qmi_dev;

    if (ipv4(profile->iPSupported))
        qmiclientId[QMUX_TYPE_WDS] = GobiNetGetClientID(qcqmi, QMUX_TYPE_WDS);
    if (ipv6(profile->iPSupported))
        qmiclientId[QMUX_TYPE_WDS_IPV6] = GobiNetGetClientID(qcqmi, QMUX_TYPE_WDS);
    qmiclientId[QMUX_TYPE_DMS] = GobiNetGetClientID(qcqmi, QMUX_TYPE_DMS);
    qmiclientId[QMUX_TYPE_NAS] = GobiNetGetClientID(qcqmi, QMUX_TYPE_NAS);
    qmiclientId[QMUX_TYPE_UIM] = GobiNetGetClientID(qcqmi, QMUX_TYPE_UIM);
    if (profile->qmap_iface == NULL) //when QMAP enabled, set data format in GobiNet Driver
        qmiclientId[QMUX_TYPE_WDS_ADMIN] = GobiNetGetClientID(qcqmi, QMUX_TYPE_WDS_ADMIN);

    //donot check clientWDA, there is only one client for WDA, if quectel-CM is killed by SIGKILL, i cannot get client ID for WDA again!
    if (qmiclientId[QMUX_TYPE_WDS] == 0) { /*|| (clientWDA == -1)*/
        log_error("%s Failed to open %s, errno: %d (%s)", __func__, qcqmi, errno, strerror(errno));
        return -1;
    }

    return 0;
}



void * GobiNetThread(void *pData) {
    (void)pData;

    log_info("%s thread with tid=%ld start", __func__, pthread_self());
    while (1) {
        struct pollfd pollfds[16] = {{SOCKPAIR_QMI, POLLIN, 0}};
        int ne, ret, nevents = 1;
        unsigned int i;

        for (i = 0; i < sizeof(qmiclientId)/sizeof(qmiclientId[0]); i++) {
            if (qmiclientId[i] != 0) {
                pollfds[nevents].fd = qmiclientId[i];
                pollfds[nevents].events = POLLIN;
                pollfds[nevents].revents = 0;
                nevents++;
            }
        }

        do {
            ret = poll(pollfds, nevents, -1);
        } while ((ret < 0) && (errno == EINTR));

        if (ret <= 0) {
            log_error("%s poll=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
            break;
        }

        for (ne = 0; ne < nevents; ne++) {
            int fd = pollfds[ne].fd;
            short revents = pollfds[ne].revents;

            if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
                log_error("%s poll err/hup/inval fd = %d, events = 0x%04x", __func__, fd, revents);
                if (fd == SOCKPAIR_QMI) {
                } else {
                }
                if (revents & (POLLERR | POLLHUP | POLLNVAL))
                    goto __GobiNetThread_quit;
            }

            if ((revents & POLLIN) == 0)
                continue;

            if (fd == SOCKPAIR_QMI) {
                int triger_event;
                if (read(fd, &triger_event, sizeof(triger_event)) == sizeof(triger_event)) {
                    switch (triger_event) {
                    case RIL_REQUEST_QUIT:
                        log_info("QMI receive event 0x%4x", triger_event);
                        goto __GobiNetThread_quit;
                        break;
                    default:
                        break;
                    }
                }
                continue;
            } else {
                ssize_t nreads;
                uchar QMIBuf[512];
                PQCQMIMSG pResponse = (PQCQMIMSG)QMIBuf;

                nreads = read(fd, &pResponse->MUXMsg, sizeof(QMIBuf) - sizeof(QCQMI_HDR));
                if (nreads <= 0) {
                    log_error("%s read=%d errno: %d (%s)",  __func__, (int)nreads, errno, strerror(errno));
                    break;
                }

                for (i = 0; i < sizeof(qmiclientId)/sizeof(qmiclientId[0]); i++) {
                    if (qmiclientId[i] == fd) {
                        pResponse->QMIHdr.QMIType = i;
                    }
                }

                pResponse->QMIHdr.IFType = USB_CTL_MSG_TYPE_QMI;
                pResponse->QMIHdr.Length = cpu_to_le16(nreads + sizeof(QCQMI_HDR)  - 1);
                pResponse->QMIHdr.CtlFlags = 0x00;
                pResponse->QMIHdr.ClientId = fd & 0xFF;

                QmiThreadRecvQMI(pResponse);
            }
        }
    }

__GobiNetThread_quit:
    GobiNetDeInit();
    QmiThreadRecvQMI(NULL); //main thread may pending on QmiThreadSendQMI()
    log_info("%s thread with tid=%ld exit", __func__, pthread_self());
    pthread_exit(NULL);
    return NULL;
}

#else
int GobiNetSendQMI(PQCQMIMSG pRequest) {
    return -1;
}
void * GobiNetThread(void *pData) {
    log_info("please set CONFIG_GOBINET");
    return NULL;
}
#endif
