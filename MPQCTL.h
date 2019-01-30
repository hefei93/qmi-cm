/*===========================================================================

                            M P Q C T L. H
DESCRIPTION:

    This module contains QMI QCTL module.

INITIALIZATION AND SEQUENCING REQUIREMENTS:

Copyright (C) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

#ifndef MPQCTL_H
#define MPQCTL_H

#include "MPQMI.h"

#pragma pack(push, 1)

// ================= QMICTL ==================

// QMICTL Control Flags
#define QMICTL_CTL_FLAG_CMD     0x00
#define QMICTL_CTL_FLAG_RSP     0x01
#define QMICTL_CTL_FLAG_IND     0x02

#if 0
typedef struct _QMICTL_TRANSACTION_ITEM {
    LIST_ENTRY  List;
    uchar       TransactionId;  // QMICTL transaction id
    PVOID       Context;        // Adapter or IocDev
    PIRP        Irp;
} QMICTL_TRANSACTION_ITEM, *PQMICTL_TRANSACTION_ITEM;
#endif

typedef struct _QCQMICTL_MSG_HDR {
    uchar  CtlFlags;  // 00-cmd, 01-rsp, 10-ind
    uchar  TransactionId;
    ushort QMICTLType;
    ushort Length;
} __attribute__ ((packed)) QCQMICTL_MSG_HDR, *PQCQMICTL_MSG_HDR;

#define QCQMICTL_MSG_HDR_SIZE sizeof(QCQMICTL_MSG_HDR)

typedef struct _QCQMICTL_MSG_HDR_RESP {
    uchar  CtlFlags;  // 00-cmd, 01-rsp, 10-ind
    uchar  TransactionId;
    ushort QMICTLType;
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} __attribute__ ((packed)) QCQMICTL_MSG_HDR_RESP, *PQCQMICTL_MSG_HDR_RESP;

typedef struct _QCQMICTL_MSG {
    uchar  CtlFlags;  // 00-cmd, 01-rsp, 10-ind
    uchar  TransactionId;
    ushort QMICTLType;
    ushort Length;
    uchar  Payload;
} __attribute__ ((packed)) QCQMICTL_MSG, *PQCQMICTL_MSG;

// TLV Header
typedef struct _QCQMICTL_TLV_HDR {
    uchar  TLVType;
    ushort TLVLength;
} __attribute__ ((packed)) QCQMICTL_TLV_HDR, *PQCQMICTL_TLV_HDR;

#define QCQMICTL_TLV_HDR_SIZE sizeof(QCQMICTL_TLV_HDR)

// QMICTL Type
#define QMICTL_SET_INSTANCE_ID_REQ    0x0020
#define QMICTL_SET_INSTANCE_ID_RESP   0x0020
#define QMICTL_GET_VERSION_REQ        0x0021
#define QMICTL_GET_VERSION_RESP       0x0021
#define QMICTL_GET_CLIENT_ID_REQ      0x0022
#define QMICTL_GET_CLIENT_ID_RESP     0x0022
#define QMICTL_RELEASE_CLIENT_ID_REQ  0x0023
#define QMICTL_RELEASE_CLIENT_ID_RESP 0x0023
#define QMICTL_REVOKE_CLIENT_ID_IND   0x0024
#define QMICTL_INVALID_CLIENT_ID_IND  0x0025
#define QMICTL_SET_DATA_FORMAT_REQ    0x0026
#define QMICTL_SET_DATA_FORMAT_RESP   0x0026
#define QMICTL_SYNC_REQ               0x0027
#define QMICTL_SYNC_RESP              0x0027
#define QMICTL_SYNC_IND               0x0027

#define QMICTL_FLAG_REQUEST    0x00
#define QMICTL_FLAG_RESPONSE   0x01
#define QMICTL_FLAG_INDICATION 0x02

// QMICTL Message Definitions

typedef struct _QMICTL_SET_INSTANCE_ID_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_SET_INSTANCE_ID_REQ
    ushort Length;          // 4
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 1
    uchar  Value;           // Host-unique QMI instance for this device driver
} __attribute__ ((packed)) QMICTL_SET_INSTANCE_ID_REQ_MSG, *PQMICTL_SET_INSTANCE_ID_REQ_MSG;

typedef struct _QMICTL_SET_INSTANCE_ID_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_SET_INSTANCE_ID_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;
    ushort QMIError;
    uchar  TLV2Type;        // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLV2Length;      // 0x0002
    ushort QMI_ID;          // Upper byte is assigned by MSM,
    // lower assigned by host
} __attribute__ ((packed)) QMICTL_SET_INSTANCE_ID_RESP_MSG, *PQMICTL_SET_INSTANCE_ID_RESP_MSG;

typedef struct _QMICTL_GET_VERSION_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_GET_VERSION_REQ
    ushort Length;          // 0
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // var
    uchar  QMUXTypes;       // List of one byte QMUX_TYPE values
    // 0xFF returns a list of versions for all
    // QMUX_TYPEs implemented on the device
} __attribute__ ((packed)) QMICTL_GET_VERSION_REQ_MSG, *PQMICTL_GET_VERSION_REQ_MSG;

typedef struct _QMUX_TYPE_VERSION_STRUCT {
    uchar  QMUXType;
    ushort MajorVersion;
    ushort MinorVersion;
} __attribute__ ((packed)) QMUX_TYPE_VERSION_STRUCT, *PQMUX_TYPE_VERSION_STRUCT;

typedef struct _ADDENDUM_VERSION_PREAMBLE {
    uchar LabelLength;
    uchar Label;
} __attribute__ ((packed)) ADDENDUM_VERSION_PREAMBLE, *PADDENDUM_VERSION_PREAMBLE;

#define QMICTL_GET_VERSION_RSP_TLV_TYPE_VERSION     0x01
#define QMICTL_GET_VERSION_RSP_TLV_TYPE_ADD_VERSION 0x10

typedef struct _QMICTL_GET_VERSION_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_GET_VERSION_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;
    ushort QMIError;
    uchar  TLV2Type;        // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLV2Length;      // var
    uchar  NumElements;     // Num of QMUX_TYPE_VERSION_STRUCT
    QMUX_TYPE_VERSION_STRUCT TypeVersion[0];
} __attribute__ ((packed)) QMICTL_GET_VERSION_RESP_MSG, *PQMICTL_GET_VERSION_RESP_MSG;

typedef struct _QMICTL_GET_CLIENT_ID_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_GET_CLIENT_ID_REQ
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 1
    uchar  QMIType;         // QMUX type
} __attribute__ ((packed)) QMICTL_GET_CLIENT_ID_REQ_MSG, *PQMICTL_GET_CLIENT_ID_REQ_MSG;

typedef struct _QMICTL_GET_CLIENT_ID_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_GET_CLIENT_ID_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;       // result code
    ushort QMIError;        // error code
    uchar  TLV2Type;        // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLV2Length;      // 2
    uchar  QMIType;
    uchar  ClientId;
} __attribute__ ((packed)) QMICTL_GET_CLIENT_ID_RESP_MSG, *PQMICTL_GET_CLIENT_ID_RESP_MSG;

typedef struct _QMICTL_RELEASE_CLIENT_ID_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_RELEASE_CLIENT_ID_REQ
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 0x0002
    uchar  QMIType;
    uchar  ClientId;
} __attribute__ ((packed)) QMICTL_RELEASE_CLIENT_ID_REQ_MSG, *PQMICTL_RELEASE_CLIENT_ID_REQ_MSG;

typedef struct _QMICTL_RELEASE_CLIENT_ID_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_RELEASE_CLIENT_ID_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;       // result code
    ushort QMIError;        // error code
    uchar  TLV2Type;        // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLV2Length;      // 2
    uchar  QMIType;
    uchar  ClientId;
} __attribute__ ((packed)) QMICTL_RELEASE_CLIENT_ID_RESP_MSG, *PQMICTL_RELEASE_CLIENT_ID_RESP_MSG;

typedef struct _QMICTL_REVOKE_CLIENT_ID_IND_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_INDICATION
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_REVOKE_CLIENT_ID_IND
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 0x0002
    uchar  QMIType;
    uchar  ClientId;
} __attribute__ ((packed)) QMICTL_REVOKE_CLIENT_ID_IND_MSG, *PQMICTL_REVOKE_CLIENT_ID_IND_MSG;

typedef struct _QMICTL_INVALID_CLIENT_ID_IND_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_INDICATION
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_REVOKE_CLIENT_ID_IND
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 0x0002
    uchar  QMIType;
    uchar  ClientId;
} __attribute__ ((packed)) QMICTL_INVALID_CLIENT_ID_IND_MSG, *PQMICTL_INVALID_CLIENT_ID_IND_MSG;

typedef struct _QMICTL_SET_DATA_FORMAT_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_SET_DATA_FORMAT_REQ
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_REQUIRED_PARAMETER
    ushort TLVLength;       // 1
    uchar  DataFormat;      // 0-default; 1-QoS hdr present
} __attribute__ ((packed)) QMICTL_SET_DATA_FORMAT_REQ_MSG, *PQMICTL_SET_DATA_FORMAT_REQ_MSG;

#ifdef QC_IP_MODE
#define SET_DATA_FORMAT_TLV_TYPE_LINK_PROTO 0x10
#define SET_DATA_FORMAT_LINK_PROTO_ETH      0x0001
#define SET_DATA_FORMAT_LINK_PROTO_IP       0x0002
typedef struct _QMICTL_SET_DATA_FORMAT_TLV_LINK_PROT {
    uchar  TLVType;         // Link-Layer Protocol
    ushort TLVLength;       // 2
    ushort LinkProt;        // 0x1: ETH; 0x2: IP
} QMICTL_SET_DATA_FORMAT_TLV_LINK_PROT, *PQMICTL_SET_DATA_FORMAT_TLV_LINK_PROT;

#ifdef QCMP_UL_TLP
#define SET_DATA_FORMAT_TLV_TYPE_UL_TLP 0x11
typedef struct _QMICTL_SET_DATA_FORMAT_TLV_UL_TLP {
    uchar  TLVType;         // 0x11, Uplink TLP Setting
    ushort TLVLength;       // 1
    uchar  UlTlpSetting;    // 0x0: Disable; 0x01: Enable
} QMICTL_SET_DATA_FORMAT_TLV_UL_TLP, *PQMICTL_SET_DATA_FORMAT_TLV_UL_TLP;
#endif // QCMP_UL_TLP

#ifdef QCMP_DL_TLP
#define SET_DATA_FORMAT_TLV_TYPE_DL_TLP 0x13
typedef struct _QMICTL_SET_DATA_FORMAT_TLV_DL_TLP {
    uchar  TLVType;         // 0x11, Uplink TLP Setting
    ushort TLVLength;       // 1
    uchar  DlTlpSetting;    // 0x0: Disable; 0x01: Enable
} QMICTL_SET_DATA_FORMAT_TLV_DL_TLP, *PQMICTL_SET_DATA_FORMAT_TLV_DL_TLP;
#endif // QCMP_DL_TLP

#endif // QC_IP_MODE

#ifdef MP_QCQOS_ENABLED
#define SET_DATA_FORMAT_TLV_TYPE_QOS_SETTING 0x12
typedef struct _QMICTL_SET_DATA_FORMAT_TLV_QOS_SETTING {
    uchar  TLVType;       // 0x12, QoS setting
    ushort TLVLength;     // 1
    uchar  QosSetting;    // 0x0: Disable; 0x01: Enable
} QMICTL_SET_DATA_FORMAT_TLV_QOS_SETTING, *PQMICTL_SET_DATA_FORMAT_TLV_QOS_SETTING;
#endif // MP_QCQOS_ENABLED

typedef struct _QMICTL_SET_DATA_FORMAT_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_SET_DATA_FORMAT_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;       // result code
    ushort QMIError;        // error code
} __attribute__ ((packed)) QMICTL_SET_DATA_FORMAT_RESP_MSG, *PQMICTL_SET_DATA_FORMAT_RESP_MSG;

typedef struct _QMICTL_SYNC_REQ_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_REQUEST
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_CTL_SYNC_REQ
    ushort Length;          // 0
} __attribute__ ((packed)) QMICTL_SYNC_REQ_MSG, *PQMICTL_SYNC_REQ_MSG;

typedef struct _QMICTL_SYNC_RESP_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_RESPONSE
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_CTL_SYNC_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMIResult;
    ushort QMIError;
} __attribute__ ((packed)) QMICTL_SYNC_RESP_MSG, *PQMICTL_SYNC_RESP_MSG;

typedef struct _QMICTL_SYNC_IND_MSG {
    uchar  CtlFlags;        // QMICTL_FLAG_INDICATION
    uchar  TransactionId;
    ushort QMICTLType;      // QMICTL_REVOKE_CLIENT_ID_IND
    ushort Length;
} __attribute__ ((packed)) QMICTL_SYNC_IND_MSG, *PQMICTL_SYNC_IND_MSG;

typedef struct _QMICTL_MSG {
    union {
        // Message Header
        QCQMICTL_MSG_HDR                             QMICTLMsgHdr;
        QCQMICTL_MSG_HDR_RESP                             QMICTLMsgHdrRsp;

        // QMICTL Message
        QMICTL_SET_INSTANCE_ID_REQ_MSG               SetInstanceIdReq;
        QMICTL_SET_INSTANCE_ID_RESP_MSG              SetInstanceIdRsp;
        QMICTL_GET_VERSION_REQ_MSG                   GetVersionReq;
        QMICTL_GET_VERSION_RESP_MSG                  GetVersionRsp;
        QMICTL_GET_CLIENT_ID_REQ_MSG                 GetClientIdReq;
        QMICTL_GET_CLIENT_ID_RESP_MSG                GetClientIdRsp;
        QMICTL_RELEASE_CLIENT_ID_REQ_MSG             ReleaseClientIdReq;
        QMICTL_RELEASE_CLIENT_ID_RESP_MSG            ReleaseClientIdRsp;
        QMICTL_REVOKE_CLIENT_ID_IND_MSG              RevokeClientIdInd;
        QMICTL_INVALID_CLIENT_ID_IND_MSG             InvalidClientIdInd;
        QMICTL_SET_DATA_FORMAT_REQ_MSG               SetDataFormatReq;
        QMICTL_SET_DATA_FORMAT_RESP_MSG              SetDataFormatRsp;
        QMICTL_SYNC_REQ_MSG                          SyncReq;
        QMICTL_SYNC_RESP_MSG                         SyncRsp;
        QMICTL_SYNC_IND_MSG                          SyncInd;
    };
} __attribute__ ((packed)) QMICTL_MSG, *PQMICTL_MSG;

#pragma pack(pop)

#endif // MPQCTL_H
