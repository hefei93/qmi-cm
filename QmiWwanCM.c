#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
typedef unsigned short sa_family_t;
#include <linux/un.h>
#include "QMIThread.h"
#include "util.h"


#ifdef CONFIG_QMIWWAN
int cdc_wdm_fd = -1;
static uchar GetQCTLTransactionId(void) {
    static int TransactionId = 0;
    if (++TransactionId > 0xFF)
        TransactionId = 1;
    return TransactionId;
}

typedef ushort (*CUSTOMQCTL)(PQMICTL_MSG pCTLMsg, void *arg);

static PQCQMIMSG ComposeQCTLMsg(ushort QMICTLType, CUSTOMQCTL customQctlMsgFunction, void *arg) {
    uchar QMIBuf[WDM_DEFAULT_BUFSIZE];
    PQCQMIMSG pRequest = (PQCQMIMSG)QMIBuf;
    int Length;

    pRequest->QMIHdr.IFType   = USB_CTL_MSG_TYPE_QMI;
    pRequest->QMIHdr.CtlFlags = 0x00;
    pRequest->QMIHdr.QMIType  = QMUX_TYPE_CTL;
    pRequest->QMIHdr.ClientId= 0x00;

    pRequest->CTLMsg.QMICTLMsgHdr.CtlFlags = QMICTL_FLAG_REQUEST;
    pRequest->CTLMsg.QMICTLMsgHdr.TransactionId = GetQCTLTransactionId();
    pRequest->CTLMsg.QMICTLMsgHdr.QMICTLType = cpu_to_le16(QMICTLType);
    if (customQctlMsgFunction)
        pRequest->CTLMsg.QMICTLMsgHdr.Length = cpu_to_le16(customQctlMsgFunction(&pRequest->CTLMsg, arg) - sizeof(QCQMICTL_MSG_HDR));
    else
        pRequest->CTLMsg.QMICTLMsgHdr.Length = cpu_to_le16(0x0000);

    pRequest->QMIHdr.Length = cpu_to_le16(le16_to_cpu(pRequest->CTLMsg.QMICTLMsgHdr.Length) + sizeof(QCQMICTL_MSG_HDR) + sizeof(QCQMI_HDR) - 1);
    Length = le16_to_cpu(pRequest->QMIHdr.Length) + 1;

    pRequest = (PQCQMIMSG)malloc(Length);
    if (pRequest == NULL) {
        log_info("%s fail to malloc", __func__);
    } else {
        memcpy(pRequest, QMIBuf, Length);
    }

    return pRequest;
}

static ushort CtlGetVersionReq(PQMICTL_MSG QCTLMsg, void *arg) {
    (void)arg;
    QCTLMsg->GetVersionReq.TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
    QCTLMsg->GetVersionReq.TLVLength     = cpu_to_le16(0x0001);
    QCTLMsg->GetVersionReq.QMUXTypes     = QMUX_TYPE_ALL;
    return sizeof(QMICTL_GET_VERSION_REQ_MSG);
}

static ushort CtlGetClientIdReq(PQMICTL_MSG QCTLMsg, void *arg) {
    QCTLMsg->GetClientIdReq.TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
    QCTLMsg->GetClientIdReq.TLVLength     = cpu_to_le16(0x0001);
    QCTLMsg->GetClientIdReq.QMIType     = ((uchar *)arg)[0];
    return sizeof(QMICTL_GET_CLIENT_ID_REQ_MSG);
}

static ushort CtlReleaseClientIdReq(PQMICTL_MSG QCTLMsg, void *arg) {
    QCTLMsg->ReleaseClientIdReq.TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
    QCTLMsg->ReleaseClientIdReq.TLVLength     = cpu_to_le16(0x0002);
    QCTLMsg->ReleaseClientIdReq.QMIType     = ((uchar *)arg)[0];
    QCTLMsg->ReleaseClientIdReq.ClientId = ((uchar *)arg)[1] ;
    return sizeof(QMICTL_RELEASE_CLIENT_ID_REQ_MSG);
}

int QmiWwanSendQMI(PQCQMIMSG pRequest) {
    struct pollfd pollfds[]= {{cdc_wdm_fd, POLLOUT, 0}};
    int ret;

    if (cdc_wdm_fd == -1) {
        log_error("%s cdc_wdm_fd = -1", __func__);
        return -ENODEV;
    }

    // WHY?
    if (pRequest->QMIHdr.QMIType == QMUX_TYPE_WDS_IPV6)
        pRequest->QMIHdr.QMIType = QMUX_TYPE_WDS;

    do {
        ret = poll(pollfds, sizeof(pollfds)/sizeof(pollfds[0]), 5000);
    } while ((ret < 0) && (errno == EINTR));

    if (pollfds[0].revents & POLLOUT) {
        ssize_t nwrites = le16_to_cpu(pRequest->QMIHdr.Length) + 1;
        ret = write(cdc_wdm_fd, pRequest, nwrites);
        if (ret == nwrites) {
            ret = 0;
        } else {
            log_error("%s write=%d, errno: %d (%s)", __func__, ret, errno, strerror(errno));
        }
    } else {
        log_error("%s poll=%d, revents = 0x%x, errno: %d (%s)", __func__, ret, pollfds[0].revents, errno, strerror(errno));
    }

    return ret;
}

static int QmiWwanGetClientID(uchar QMIType) {
    PQCQMIMSG pResponse;

    QmiThreadSendQMI(ComposeQCTLMsg(QMICTL_GET_CLIENT_ID_REQ, CtlGetClientIdReq, &QMIType), &pResponse);

    if (pResponse) {
        ushort QMUXResult = cpu_to_le16(pResponse->CTLMsg.QMICTLMsgHdrRsp.QMUXResult);       // QMI_RESULT_SUCCESS
        ushort QMUXError = cpu_to_le16(pResponse->CTLMsg.QMICTLMsgHdrRsp.QMUXError);        // QMI_ERR_INVALID_ARG
        uchar ClientId = pResponse->CTLMsg.GetClientIdRsp.ClientId;

        if (!QMUXResult && !QMUXError && (QMIType == pResponse->CTLMsg.GetClientIdRsp.QMIType)) {
            switch (QMIType) {
            case QMUX_TYPE_WDS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_WDS), ClientId);
                break;
            case QMUX_TYPE_DMS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_DMS), ClientId);
                break;
            case QMUX_TYPE_NAS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_NAS), ClientId);
                break;
            case QMUX_TYPE_QOS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_QOS), ClientId);
                break;
            case QMUX_TYPE_WMS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_WMS), ClientId);
                break;
            case QMUX_TYPE_PDS:
                log_info("%s clientID %d", val_name(QMUX_TYPE_PDS), ClientId);
                break;
            case QMUX_TYPE_UIM:
                log_info("%s clientID %d", val_name(QMUX_TYPE_UIM), ClientId);
                break;
            case QMUX_TYPE_WDS_ADMIN:
                log_info("%s clientID %d", val_name(QMUX_TYPE_WDS_ADMIN), ClientId);
                break;
            case QMUX_TYPE_WDS_IPV6:
                log_info("%s clientID %d", val_name(QMUX_TYPE_WDS_IPV6), ClientId);
                break;
            default:
                break;
            }
            return ClientId;
        }
    }
    return 0;
}

static int QmiWwanReleaseClientID(QMI_SERVICE_TYPE QMIType, uchar ClientId) {
    uchar argv[] = {QMIType, ClientId};
    QmiThreadSendQMI(ComposeQCTLMsg(QMICTL_RELEASE_CLIENT_ID_REQ, CtlReleaseClientIdReq, argv), NULL);
    return 0;
}

int QmiWwanInit(PROFILE_T *profile) {
    unsigned i;
    int ret;
    PQCQMIMSG pResponse;

    if (profile->qmap_iface == NULL || profile->qmap_iface == profile->iface) {
		// get/dump message may be failed if fd has not been set
        for (i = 0; i < 10; i++) {
            ret = QmiThreadSendQMITimeout(ComposeQCTLMsg(QMICTL_SYNC_REQ, NULL, NULL), NULL, 1 * 1000);
            if (!ret)
                break;
            sleep(1);
        }
        if (ret)
            return ret;
    }

    QmiThreadSendQMI(ComposeQCTLMsg(QMICTL_GET_VERSION_REQ, CtlGetVersionReq, NULL), &pResponse);
    if (profile->qmap_iface != NULL && profile->qmap_iface == profile->iface) {
        if (pResponse) {
            if (pResponse->CTLMsg.QMICTLMsgHdrRsp.QMUXResult == 0 && pResponse->CTLMsg.QMICTLMsgHdrRsp.QMUXError == 0) {
                uint8_t  NumElements = 0;

                for (NumElements = 0; NumElements < pResponse->CTLMsg.GetVersionRsp.NumElements; NumElements++) {
                    log_info("QMUXType = %02x Version = %d.%d",
                             pResponse->CTLMsg.GetVersionRsp.TypeVersion[NumElements].QMUXType,
                             pResponse->CTLMsg.GetVersionRsp.TypeVersion[NumElements].MajorVersion,
                             pResponse->CTLMsg.GetVersionRsp.TypeVersion[NumElements].MinorVersion);
                    if (pResponse->CTLMsg.GetVersionRsp.TypeVersion[NumElements].QMUXType == QMUX_TYPE_WDS_ADMIN)
                        profile->qmap_version = (pResponse->CTLMsg.GetVersionRsp.TypeVersion[NumElements].MinorVersion > 16);
                }
            }
        }
    }
    qfree((void**)&pResponse);

    if (ipv4(profile->iPSupported))
        qmiclientId[QMUX_TYPE_WDS] = QmiWwanGetClientID(QMUX_TYPE_WDS);
    if (ipv6(profile->iPSupported))
        qmiclientId[QMUX_TYPE_WDS_IPV6] = QmiWwanGetClientID(QMUX_TYPE_WDS);
    qmiclientId[QMUX_TYPE_DMS] = QmiWwanGetClientID(QMUX_TYPE_DMS);
    qmiclientId[QMUX_TYPE_NAS] = QmiWwanGetClientID(QMUX_TYPE_NAS);
    qmiclientId[QMUX_TYPE_UIM] = QmiWwanGetClientID(QMUX_TYPE_UIM);
    if (profile->qmap_iface == NULL || profile->qmap_iface == profile->iface)
        qmiclientId[QMUX_TYPE_WDS_ADMIN] = QmiWwanGetClientID(QMUX_TYPE_WDS_ADMIN);

    return 0;
}

int QmiWwanDeInit(void) {
    unsigned int i;
    for (i = 0; i < sizeof(qmiclientId)/sizeof(qmiclientId[0]); i++) {
        if (qmiclientId[i] != 0) {
            QmiWwanReleaseClientID(i, qmiclientId[i]);
            qmiclientId[i] = 0;
        }
    }

    return 0;
}

int qmi_proxy_open(const char *name) {
    int sockfd = -1;
    int reuse_addr = 1;
    struct sockaddr_un sockaddr;
    socklen_t alen;

    /*Create server socket*/
    (sockfd = socket(AF_LOCAL, SOCK_STREAM, 0));
    if (sockfd < 0)
        return sockfd;

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sun_family = AF_LOCAL;
    sockaddr.sun_path[0] = 0;
    memcpy(sockaddr.sun_path + 1, name, strlen(name) );

    alen = strlen(name) + offsetof(struct sockaddr_un, sun_path) + 1;
    if(connect(sockfd, (struct sockaddr *)&sockaddr, alen) < 0) {
        close(sockfd);
        log_error("%s connect %s errno: %d (%s)\n", __func__, name, errno, strerror(errno));
        return -1;
    }
    (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr)));

    log_info("connect to %s sockfd = %d\n", name, sockfd);

    return sockfd;
}

static ssize_t qmi_proxy_read (int fd, void *buf, size_t size) {
    ssize_t nreads;
    PQCQMI_HDR pHdr = (PQCQMI_HDR)buf;

    (void)size;
    nreads = read(fd, pHdr, sizeof(QCQMI_HDR));
    if (nreads == sizeof(QCQMI_HDR)) {
        nreads += read(fd, pHdr+1, le16_to_cpu(pHdr->Length) + 1 - sizeof(QCQMI_HDR));
    }

    return nreads;
}

void * QmiWwanThread(void *pData) {
    PROFILE_T *profile = (PROFILE_T *)pData;

    log_info("%s thread with tid=%ld start", __func__, pthread_self());
    if (cdc_wdm_fd == -1) {
        log_error("%s Failed to open %s, errno: %d (%s)", __func__, profile->qmi_dev, errno, strerror(errno));
        goto __QmiWwanThread_quit;
    }

    fcntl(cdc_wdm_fd, F_SETFL, fcntl(cdc_wdm_fd, F_GETFL) | O_NONBLOCK);
    fcntl(cdc_wdm_fd, F_SETFD, FD_CLOEXEC);

    log_info("cdc_wdm_fd = %d", cdc_wdm_fd);

    while (1) {
        struct pollfd pollfds[] = {{SOCKPAIR_QMI, POLLIN, 0}, {cdc_wdm_fd, POLLIN, 0}};
        int ne, ret, nevents = sizeof(pollfds)/sizeof(pollfds[0]);

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
                log_warn("%s poll err/hup/inval", __func__);
                log_warn("poll fd = %d, events = 0x%04x", fd, revents);
                if (revents & (POLLHUP | POLLNVAL)) //EC20 bug, Can get POLLERR
                    goto __QmiWwanThread_quit;
            }

            if ((revents & POLLIN) == 0)
                continue;

            if (fd == SOCKPAIR_QMI) {
                int triger_event;
                if (read(fd, &triger_event, sizeof(triger_event)) == sizeof(triger_event)) {
                    switch (triger_event) {
                    case RIL_REQUEST_QUIT:
                        log_info("QMI receive event 0x%x", triger_event);
                        goto __QmiWwanThread_quit;
                        break;
                    default:
                        break;
                    }
                }
            }

            if (fd == cdc_wdm_fd) {
                ssize_t nreads;
                uchar QMIBuf[512];
                PQCQMIMSG pResponse = (PQCQMIMSG)QMIBuf;

                if (profile->qmap_iface == NULL || profile->qmap_iface == profile->iface)
                    nreads = read(fd, QMIBuf, sizeof(QMIBuf));
                else
                    nreads = qmi_proxy_read(fd, QMIBuf, sizeof(QMIBuf));
                if (nreads <= 0) {
                    log_error("%s read=%d errno: %d (%s)",  __func__, (int)nreads, errno, strerror(errno));
                    break;
                }

                if (nreads != (le16_to_cpu(pResponse->QMIHdr.Length) + 1)) {
                    log_info("%s nreads=%d,  pQCQMI->QMIHdr.Length = %d",  __func__, (int)nreads, le16_to_cpu(pResponse->QMIHdr.Length));
                    continue;
                }

                QmiThreadRecvQMI(pResponse);
            }
        }
    }

__QmiWwanThread_quit:
    if (cdc_wdm_fd != -1) {
        close(cdc_wdm_fd);
        cdc_wdm_fd = -1;
    }
    QmiThreadRecvQMI(NULL); //main thread may pending on QmiThreadSendQMI()
    log_info("%s thread with tid=%ld exit", __func__, pthread_self());
    pthread_exit(NULL);
    return NULL;
}

#else
int QmiWwanSendQMI(PQCQMIMSG pRequest) {
    return -1;
}
int QmiWwanInit(PROFILE_T *profile) {
    return -1;
}
int QmiWwanDeInit(void) {
    return -1;
}
void * QmiWwanThread(void *pData) {
    log_info("please set CONFIG_QMIWWAN");
    return NULL;
}
#endif
