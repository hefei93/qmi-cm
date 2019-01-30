/*===========================================================================

                            M P Q M U X. H
DESCRIPTION:

    This file provides support for QMUX.

INITIALIZATION AND SEQUENCING REQUIREMENTS:

Copyright (C) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

#ifndef MPQMUX_H
#define MPQMUX_H

#include "MPQMI.h"

#pragma pack(push, 1)

#define QMIWDS_SET_EVENT_REPORT_REQ           0x0001
#define QMIWDS_SET_EVENT_REPORT_RESP          0x0001
#define QMIWDS_EVENT_REPORT_IND               0x0001
#define QMIWDS_START_NETWORK_intERFACE_REQ    0x0020
#define QMIWDS_START_NETWORK_intERFACE_RESP   0x0020
#define QMIWDS_STOP_NETWORK_intERFACE_REQ     0x0021
#define QMIWDS_STOP_NETWORK_intERFACE_RESP    0x0021
#define QMIWDS_GET_PKT_SRVC_STATUS_REQ        0x0022
#define QMIWDS_GET_PKT_SRVC_STATUS_RESP       0x0022
#define QMIWDS_GET_PKT_SRVC_STATUS_IND        0x0022
#define QMIWDS_GET_CURRENT_CHANNEL_RATE_REQ   0x0023
#define QMIWDS_GET_CURRENT_CHANNEL_RATE_RESP  0x0023
#define QMIWDS_GET_PKT_STATISTICS_REQ         0x0024
#define QMIWDS_GET_PKT_STATISTICS_RESP        0x0024
#define QMIWDS_MODIFY_PROFILE_SETTINGS_REQ    0x0028
#define QMIWDS_MODIFY_PROFILE_SETTINGS_RESP   0x0028
#define QMIWDS_GET_PROFILE_SETTINGS_REQ         0x002B
#define QMIWDS_GET_PROFILE_SETTINGS_RESP        0x002B
#define QMIWDS_GET_DEFAULT_SETTINGS_REQ       0x002C
#define QMIWDS_GET_DEFAULT_SETTINGS_RESP      0x002C
#define QMIWDS_GET_RUNTIME_SETTINGS_REQ       0x002D
#define QMIWDS_GET_RUNTIME_SETTINGS_RESP      0x002D
#define QMIWDS_GET_MIP_MODE_REQ               0x002F
#define QMIWDS_GET_MIP_MODE_RESP              0x002F
#define QMIWDS_GET_DATA_BEARER_REQ            0x0037
#define QMIWDS_GET_DATA_BEARER_RESP           0x0037
#define QMIWDS_DUN_CALL_INFO_REQ              0x0038
#define QMIWDS_DUN_CALL_INFO_RESP             0x0038
#define QMIWDS_DUN_CALL_INFO_IND              0x0038
#define QMIWDS_SET_CLIENT_IP_FAMILY_PREF_REQ  0x004D
#define QMIWDS_SET_CLIENT_IP_FAMILY_PREF_RESP 0x004D
#define QMIWDS_SET_AUTO_CONNECT_REQ     0x0051
#define QMIWDS_SET_AUTO_CONNECT_RESP     0x0051
#define QMIWDS_BIND_MUX_DATA_PORT_REQ         0x00A2
#define QMIWDS_BIND_MUX_DATA_PORT_RESP        0x00A2


// Stats masks
#define QWDS_STAT_MASK_TX_PKT_OK 0x00000001
#define QWDS_STAT_MASK_RX_PKT_OK 0x00000002
#define QWDS_STAT_MASK_TX_PKT_ER 0x00000004
#define QWDS_STAT_MASK_RX_PKT_ER 0x00000008
#define QWDS_STAT_MASK_TX_PKT_OF 0x00000010
#define QWDS_STAT_MASK_RX_PKT_OF 0x00000020

// TLV Types for xfer statistics
#define TLV_WDS_TX_GOOD_PKTS      0x10
#define TLV_WDS_RX_GOOD_PKTS      0x11
#define TLV_WDS_TX_ERROR          0x12
#define TLV_WDS_RX_ERROR          0x13
#define TLV_WDS_TX_OVERFLOW       0x14
#define TLV_WDS_RX_OVERFLOW       0x15
#define TLV_WDS_CHANNEL_RATE      0x16
#define TLV_WDS_DATA_BEARER       0x17
#define TLV_WDS_DORMANCY_STATUS   0x18

#define QWDS_PKT_DATA_DISCONNECTED    0x01
#define QWDS_PKT_DATA_CONNECTED        0x02
#define QWDS_PKT_DATA_SUSPENDED        0x03
#define QWDS_PKT_DATA_AUTHENTICATING   0x04

#define QMIWDS_ADMIN_SET_DATA_FORMAT_REQ      0x0020
#define QMIWDS_ADMIN_SET_DATA_FORMAT_RESP     0x0020
#define QMIWDS_ADMIN_GET_DATA_FORMAT_REQ      0x0021
#define QMIWDS_ADMIN_GET_DATA_FORMAT_RESP     0x0021
#define QMIWDS_ADMIN_SET_QMAP_SETTINGS_REQ    0x002B
#define QMIWDS_ADMIN_SET_QMAP_SETTINGS_RESP   0x002B
#define QMIWDS_ADMIN_GET_QMAP_SETTINGS_REQ    0x002C
#define QMIWDS_ADMIN_GET_QMAP_SETTINGS_RESP   0x002C

#define NETWORK_DESC_ENCODING_OCTET       0x00
#define NETWORK_DESC_ENCODING_EXTPROTOCOL 0x01
#define NETWORK_DESC_ENCODING_7BITASCII   0x02
#define NETWORK_DESC_ENCODING_IA5         0x03
#define NETWORK_DESC_ENCODING_UNICODE     0x04
#define NETWORK_DESC_ENCODING_SHIFTJIS    0x05
#define NETWORK_DESC_ENCODING_KOREAN      0x06
#define NETWORK_DESC_ENCODING_LATINH      0x07
#define NETWORK_DESC_ENCODING_LATIN       0x08
#define NETWORK_DESC_ENCODING_GSM7BIT     0x09
#define NETWORK_DESC_ENCODING_GSMDATA     0x0A
#define NETWORK_DESC_ENCODING_UNKNOWN     0xFF

typedef struct _QMIWDS_ADMIN_SET_DATA_FORMAT {
    ushort Type;             // QMUX type 0x0000
    ushort Length;
} __attribute__ ((packed)) QMIWDS_ADMIN_SET_DATA_FORMAT, *PQMIWDS_ADMIN_SET_DATA_FORMAT;

typedef struct _QMIWDS_ADMIN_SET_DATA_FORMAT_TLV_QOS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  QOSSetting;
} __attribute__ ((packed)) QMIWDS_ADMIN_SET_DATA_FORMAT_TLV_QOS, *PQMIWDS_ADMIN_SET_DATA_FORMAT_TLV_QOS;

typedef struct _QMIWDS_ADMIN_SET_DATA_FORMAT_TLV {
    uchar  TLVType;
    ushort TLVLength;
    uint  Value;
} __attribute__ ((packed)) QMIWDS_ADMIN_SET_DATA_FORMAT_TLV, *PQMIWDS_ADMIN_SET_DATA_FORMAT_TLV;

typedef struct _QMIWDS_ENDPOint_TLV {
    uchar  TLVType;
    ushort TLVLength;
    uint  ep_type;
    uint  iface_id;
} __attribute__ ((packed)) QMIWDS_ENDPOint_TLV, *PQMIWDS_ENDPOint_TLV;


typedef struct _QMIWDS_ADMIN_SET_DATA_FORMAT_REQ_MSG {
    ushort Type;
    ushort Length;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV_QOS QosDataFormatTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV UnderlyingLinkLayerProtocolTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV UplinkDataAggregationProtocolTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV DownlinkDataAggregationProtocolTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV DownlinkDataAggregationMaxDatagramsTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV DownlinkDataAggregationMaxSizeTlv;
#if 0
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV UplinkDataAggregationMaxDatagramsTlv;
    QMIWDS_ADMIN_SET_DATA_FORMAT_TLV UplinkDataAggregationMaxSizeTlv;
#else
    QMIWDS_ENDPOint_TLV epTlv;
#endif
} __attribute__ ((packed)) QMIWDS_ADMIN_SET_DATA_FORMAT_REQ_MSG, *PQMIWDS_ADMIN_SET_DATA_FORMAT_REQ_MSG;

#if 0
typedef enum _QMI_RETURN_CODES {
    QMI_SUCCESS = 0,
    QMI_SUCCESS_NOT_COMPLETE,
    QMI_FAILURE
} QMI_RETURN_CODES;

typedef struct _QMIWDS_GET_PKT_SRVC_STATUS_REQ_MSG {
    ushort Type;    // 0x0022
    ushort Length;  // 0x0000
} QMIWDS_GET_PKT_SRVC_STATUS_REQ_MSG, *PQMIWDS_GET_PKT_SRVC_STATUS_REQ_MSG;

typedef struct _QMIWDS_GET_PKT_SRVC_STATUS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLVType2;
    ushort TLVLength2;
    uchar  ConnectionStatus; // 0x01: QWDS_PKT_DATAC_DISCONNECTED
    // 0x02: QWDS_PKT_DATA_CONNECTED
    // 0x03: QWDS_PKT_DATA_SUSPENDED
    // 0x04: QWDS_PKT_DATA_AUTHENTICATING
} QMIWDS_GET_PKT_SRVC_STATUS_RESP_MSG, *PQMIWDS_GET_PKT_SRVC_STATUS_RESP_MSG;

typedef struct _QMIWDS_GET_PKT_SRVC_STATUS_IND_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ConnectionStatus; // 0x01: QWDS_PKT_DATAC_DISCONNECTED
    // 0x02: QWDS_PKT_DATA_CONNECTED
    // 0x03: QWDS_PKT_DATA_SUSPENDED
    uchar  ReconfigRequired; // 0x00: No need to reconfigure
    // 0x01: Reconfiguration required
} QMIWDS_GET_PKT_SRVC_STATUS_IND_MSG, *PQMIWDS_GET_PKT_SRVC_STATUS_IND_MSG;

typedef struct _WDS_PKT_SRVC_IP_FAMILY_TLV {
    uchar  TLVType;     // 0x12
    ushort TLVLength;   // 1
    uchar  IpFamily;    // IPV4-0x04, IPV6-0x06
} WDS_PKT_SRVC_IP_FAMILY_TLV, *PWDS_PKT_SRVC_IP_FAMILY_TLV;

typedef struct _QMIWDS_DUN_CALL_INFO_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uint  Mask;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  ReportConnectionStatus;
} QMIWDS_DUN_CALL_INFO_REQ_MSG, *PQMIWDS_DUN_CALL_INFO_REQ_MSG;

typedef struct _QMIWDS_DUN_CALL_INFO_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWDS_DUN_CALL_INFO_RESP_MSG, *PQMIWDS_DUN_CALL_INFO_RESP_MSG;

typedef struct _QMIWDS_DUN_CALL_INFO_IND_MSG {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ConnectionStatus;
} QMIWDS_DUN_CALL_INFO_IND_MSG, *PQMIWDS_DUN_CALL_INFO_IND_MSG;

typedef struct _QMIWDS_GET_CURRENT_CHANNEL_RATE_REQ_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
} QMIWDS_GET_CURRENT_CHANNEL_RATE_REQ_MSG, *PQMIWDS_GET_CURRENT_CHANNEL_RATE_REQ_MSG;

typedef struct _QMIWDS_GET_CURRENT_CHANNEL_RATE_RESP_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT

    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 16
    //uint  CallHandle;       // Context corresponding to reported channel
    uint  CurrentTxRate;       // bps
    uint  CurrentRxRate;       // bps
    uint  ServingSystemTxRate; // bps
    uint  ServingSystemRxRate; // bps

} QMIWDS_GET_CURRENT_CHANNEL_RATE_RESP_MSG, *PQMIWDS_GET_CURRENT_CHANNEL_RATE_RESP;

#define QWDS_EVENT_REPORT_MASK_RATES 0x01
#define QWDS_EVENT_REPORT_MASK_STATS 0x02

#ifdef QCUSB_MUX_PROTOCOL
#error code not present
#endif // QCUSB_MUX_PROTOCOL

typedef struct _QMIWDS_SET_EVENT_REPORT_REQ_MSG {
    ushort Type;             // QMUX type 0x0042
    ushort Length;

    uchar  TLVType;          // 0x10 -- current channel rate indicator
    ushort TLVLength;        // 1
    uchar  Mode;             // 0-do not report; 1-report when rate changes

    uchar  TLV2Type;         // 0x11
    ushort TLV2Length;       // 5
    uchar  StatsPeriod;      // seconds between reports; 0-do not report
    uint  StatsMask;        //

    uchar  TLV3Type;          // 0x12 -- current data bearer indicator
    ushort TLV3Length;        // 1
    uchar  Mode3;             // 0-do not report; 1-report when changes

    uchar  TLV4Type;          // 0x13 -- dormancy status indicator
    ushort TLV4Length;        // 1
    uchar  DormancyStatus;    // 0-do not report; 1-report when changes
} QMIWDS_SET_EVENT_REPORT_REQ_MSG, *PQMIWDS_SET_EVENT_REPORT_REQ_MSG;

typedef struct _QMIWDS_SET_EVENT_REPORT_RESP_MSG {
    ushort Type;             // QMUX type 0x0042
    ushort Length;

    uchar  TLVType;          // 0x02 result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_NO_BATTERY
    // QMI_ERR_FAULT
} QMIWDS_SET_EVENT_REPORT_RESP_MSG, *PQMIWDS_SET_EVENT_REPORT_RESP_MSG;

typedef struct _QMIWDS_EVENT_REPORT_IND_MSG {
    ushort Type;             // QMUX type 0x0001
    ushort Length;
} QMIWDS_EVENT_REPORT_IND_MSG, *PQMIWDS_EVENT_REPORT_IND_MSG;

// PQCTLV_PKT_STATISTICS

typedef struct _QMIWDS_EVENT_REPORT_IND_CHAN_RATE_TLV {
    uchar  Type;
    ushort Length;  // 8
    uint  TxRate;
    uint  RxRate;
} QMIWDS_EVENT_REPORT_IND_CHAN_RATE_TLV, *PQMIWDS_EVENT_REPORT_IND_CHAN_RATE_TLV;

#ifdef QCUSB_MUX_PROTOCOL
#error code not present
#endif // QCUSB_MUX_PROTOCOL

typedef struct _QMIWDS_GET_PKT_STATISTICS_REQ_MSG {
    ushort Type;             // QMUX type 0x0041
    ushort Length;
    uchar  TLVType;          // 0x01
    ushort TLVLength;        // 4
    uint  StateMask;        // 0x00000001  tx success packets
    // 0x00000002  rx success packets
    // 0x00000004  rx packet errors (checksum)
    // 0x00000008  rx packets dropped (memory)

} QMIWDS_GET_PKT_STATISTICS_REQ_MSG, *PQMIWDS_GET_PKT_STATISTICS_REQ_MSG;

typedef struct _QMIWDS_GET_PKT_STATISTICS_RESP_MSG {
    ushort Type;             // QMUX type 0x0041
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMIWDS_GET_PKT_STATISTICS_RESP_MSG, *PQMIWDS_GET_PKT_STATISTICS_RESP_MSG;

// optional TLV for stats
typedef struct _QCTLV_PKT_STATISTICS {
    uchar  TLVType;          // see above definitions for TLV types
    ushort TLVLength;        // 4
    uint  Count;
} QCTLV_PKT_STATISTICS, *PQCTLV_PKT_STATISTICS;
#endif

//#ifdef QC_IP_MODE

#define QMIWDS_GET_RUNTIME_SETTINGS_MASK_IPV4DNS_ADDR 0x0010
#define QMIWDS_GET_RUNTIME_SETTINGS_MASK_IPV4_ADDR 0x0100
#define QMIWDS_GET_RUNTIME_SETTINGS_MASK_IPV4GATEWAY_ADDR 0x0200
#define QMIWDS_GET_RUNTIME_SETTINGS_MASK_MTU              0x2000

typedef struct _QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG {
    ushort Type;            // QMIWDS_GET_RUNTIME_SETTINGS_REQ
    ushort Length;
    uchar  TLVType;         // 0x10
    ushort TLVLength;       // 0x0004
    uint  Mask;            // mask, bit 8: IP addr -- 0x0100
} __attribute__ ((packed)) QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG, *PQMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG;

typedef struct _QMIWDS_BIND_MUX_DATA_PORT_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uint  ep_type;
    uint  iface_id;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  MuxId;
    uchar  TLV3Type;
    ushort TLV3Length;
    uint  client_type;
} __attribute__ ((packed)) QMIWDS_BIND_MUX_DATA_PORT_REQ_MSG, *PQMIWDS_BIND_MUX_DATA_PORT_REQ_MSG;

#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4PRIMARYDNS 0x15
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4SECONDARYDNS 0x16
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4 0x1E
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4GATEWAY 0x20
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4SUBNET 0x21

#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV6             0x25
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV6GATEWAY      0x26
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV6PRIMARYDNS   0x27
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV6SECONDARYDNS 0x28
#define QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_MTU              0x29

typedef struct _QMIWDS_GET_RUNTIME_SETTINGS_TLV_MTU {
    uchar  TLVType;         // QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_MTU
    ushort TLVLength;       // 4
    uint  Mtu;             // MTU
} __attribute__ ((packed)) QMIWDS_GET_RUNTIME_SETTINGS_TLV_MTU, *PQMIWDS_GET_RUNTIME_SETTINGS_TLV_MTU;

typedef struct _QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR {
    uchar  TLVType;         // QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4
    ushort TLVLength;       // 4
    uint  IPV4Address;     // address
} __attribute__ ((packed)) QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR, *PQMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR;

typedef struct _QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV6_ADDR {
    uchar  TLVType;         // QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV6
    ushort TLVLength;       // 16
    uchar  IPV6Address[16]; // address
    uchar  PrefixLength;    // prefix length
} __attribute__ ((packed)) QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV6_ADDR, *PQMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV6_ADDR;

typedef struct _QMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG {
    ushort Type;            // QMIWDS_GET_RUNTIME_SETTINGS_RESP
    ushort Length;
    uchar  TLVType;         // QCTLV_TYPE_RESULT_CODE
    ushort TLVLength;       // 0x0004
    ushort QMUXResult;      // result code
    ushort QMUXError;       // error code
} __attribute__ ((packed)) QMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG, *PQMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG;

//#endif // QC_IP_MODE

typedef struct _QMIWDS_IP_FAMILY_TLV {
    uchar  TLVType;          // 0x12
    ushort TLVLength;        // 1
    uchar  IpFamily;         // IPV4-0x04, IPV6-0x06
} __attribute__ ((packed)) QMIWDS_IP_FAMILY_TLV, *PQMIWDS_IP_FAMILY_TLV;

typedef struct _QMIWDS_PKT_SRVC_TLV {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ConnectionStatus;
    uchar  ReconfigReqd;
} __attribute__ ((packed)) QMIWDS_PKT_SRVC_TLV, *PQMIWDS_PKT_SRVC_TLV;

typedef struct _QMIWDS_CALL_END_REASON_TLV {
    uchar  TLVType;
    ushort TLVLength;
    ushort CallEndReason;
} __attribute__ ((packed)) QMIWDS_CALL_END_REASON_TLV, *PQMIWDS_CALL_END_REASON_TLV;

typedef struct _QMIWDS_CALL_END_REASON_V_TLV {
    uchar  TLVType;
    ushort TLVLength;
    ushort CallEndReasonType;
    ushort CallEndReason;
} __attribute__ ((packed)) QMIWDS_CALL_END_REASON_V_TLV, *PQMIWDS_CALL_END_REASON_V_TLV;

typedef struct _QMIWDS_SET_CLIENT_IP_FAMILY_PREF_REQ_MSG {
    ushort Type;             // QMUX type 0x004D
    ushort Length;
    uchar  TLVType;          // 0x01
    ushort TLVLength;        // 1
    uchar  IpPreference;     // IPV4-0x04, IPV6-0x06
} __attribute__ ((packed)) QMIWDS_SET_CLIENT_IP_FAMILY_PREF_REQ_MSG, *PQMIWDS_SET_CLIENT_IP_FAMILY_PREF_REQ_MSG;

typedef struct _QMIWDS_SET_CLIENT_IP_FAMILY_PREF_RESP_MSG {
    ushort Type;             // QMUX type 0x0037
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS, QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_intERNAL, QMI_ERR_MALFORMED_MSG, QMI_ERR_INVALID_ARG
} __attribute__ ((packed)) QMIWDS_SET_CLIENT_IP_FAMILY_PREF_RESP_MSG, *PQMIWDS_SET_CLIENT_IP_FAMILY_PREF_RESP_MSG;

typedef struct _QMIWDS_SET_AUTO_CONNECT_REQ_MSG {
    ushort Type;             // QMUX type 0x0051
    ushort Length;
    uchar  TLVType;          // 0x01
    ushort TLVLength;        // 1
    uchar  autoconnect_setting;     //  0x00 ?C Disabled, 0x01 ?C Enabled, 0x02 ?C Paused (resume on power cycle)
} __attribute__ ((packed)) QMIWDS_SET_AUTO_CONNECT_REQ_MSG, *PQMIWDS_SET_AUTO_CONNECT_REQ_MSG;

#if 0
typedef struct _QMIWDS_GET_MIP_MODE_REQ_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
} QMIWDS_GET_MIP_MODE_REQ_MSG, *PQMIWDS_GET_MIP_MODE_REQ_MSG;

typedef struct _QMIWDS_GET_MIP_MODE_RESP_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT

    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 20
    uchar  MipMode;          //
} QMIWDS_GET_MIP_MODE_RESP_MSG, *PQMIWDS_GET_MIP_MODE_RESP_MSG;
#endif

typedef struct _QMIWDS_TECHNOLOGY_PREFERECE {
    uchar  TLVType;
    ushort TLVLength;
    uchar  TechPreference;
} __attribute__ ((packed)) QMIWDS_TECHNOLOGY_PREFERECE, *PQMIWDS_TECHNOLOGY_PREFERECE;

typedef struct _QMIWDS_PROFILE_IDENTIFIER {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ProfileIndex;
} __attribute__ ((packed)) QMIWDS_PROFILE_IDENTIFIER, *PQMIWDS_PROFILE_IDENTIFIER;

#if 0
typedef struct _QMIWDS_IPADDRESS {
    uchar  TLVType;
    ushort TLVLength;
    uint  IPv4Address;
} QMIWDS_IPADDRESS, *PQMIWDS_IPADDRESS;

/*
typedef struct _QMIWDS_UMTS_QOS
{
   uchar  TLVType;
   ushort TLVLength;
   uchar  TrafficClass;
   uint  MaxUplinkBitRate;
   uint  MaxDownlinkBitRate;
   uint  GuarUplinkBitRate;
   uint  GuarDownlinkBitRate;
   uchar  QOSDevOrder;
   uint  MAXSDUSize;
   uchar  SDUErrorRatio;
   uchar  ResidualBerRatio;
   uchar  DeliveryErrorSDUs;
   uint  TransferDelay;
   uint  TrafficHndPri;
}QMIWDS_UMTS_QOS, *PQMIWDS_UMTS_QOS;

typedef struct _QMIWDS_GPRS_QOS
{
   uchar  TLVType;
   ushort TLVLength;
   uint  PrecedenceClass;
   uint  DelayClass;
   uint  ReliabilityClass;
   uint  PeekThroClass;
   uint  MeanThroClass;
}QMIWDS_GPRS_QOS, *PQMIWDS_GPRS_QOS;
*/
#endif

typedef struct _QMIWDS_PROFILENAME {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ProfileName;
} __attribute__ ((packed)) QMIWDS_PROFILENAME, *PQMIWDS_PROFILENAME;

typedef struct _QMIWDS_PDPTYPE {
    uchar  TLVType;
    ushort TLVLength;
// 0 ?C PDP-IP (IPv4)
// 1 ?C PDP-PPP
// 2 ?C PDP-IPv6
// 3 ?C PDP-IPv4v6
    uchar  PdpType;
} __attribute__ ((packed)) QMIWDS_PDPTYPE, *PQMIWDS_PDPTYPE;

typedef struct _QMIWDS_USERNAME {
    uchar  TLVType;
    ushort TLVLength;
    uchar  UserName;
} __attribute__ ((packed)) QMIWDS_USERNAME, *PQMIWDS_USERNAME;

typedef struct _QMIWDS_PASSWD {
    uchar  TLVType;
    ushort TLVLength;
    uchar  Passwd;
} __attribute__ ((packed)) QMIWDS_PASSWD, *PQMIWDS_PASSWD;

typedef struct _QMIWDS_AUTH_PREFERENCE {
    uchar  TLVType;
    ushort TLVLength;
    uchar  AuthPreference;
} __attribute__ ((packed)) QMIWDS_AUTH_PREFERENCE, *PQMIWDS_AUTH_PREFERENCE;

typedef struct _QMIWDS_APNNAME {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ApnName;
} __attribute__ ((packed)) QMIWDS_APNNAME, *PQMIWDS_APNNAME;

typedef struct _QMIWDS_AUTOCONNECT {
    uchar  TLVType;
    ushort TLVLength;
    uchar  AutoConnect;
} __attribute__ ((packed)) QMIWDS_AUTOCONNECT, *PQMIWDS_AUTOCONNECT;

typedef struct _QMIWDS_START_NETWORK_intERFACE_REQ_MSG {
    ushort Type;
    ushort Length;
} __attribute__ ((packed)) QMIWDS_START_NETWORK_intERFACE_REQ_MSG, *PQMIWDS_START_NETWORK_intERFACE_REQ_MSG;

typedef struct _QMIWDS_CALLENDREASON {
    uchar  TLVType;
    ushort TLVLength;
    ushort Reason;
} __attribute__ ((packed)) QMIWDS_CALLENDREASON, *PQMIWDS_CALLENDREASON;

typedef struct _QMIWDS_START_NETWORK_intERFACE_RESP_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT

    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 20
    uint  Handle;          //
} __attribute__ ((packed)) QMIWDS_START_NETWORK_intERFACE_RESP_MSG, *PQMIWDS_START_NETWORK_intERFACE_RESP_MSG;

typedef struct _QMIWDS_STOP_NETWORK_intERFACE_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uint  Handle;
} __attribute__ ((packed)) QMIWDS_STOP_NETWORK_intERFACE_REQ_MSG, *PQMIWDS_STOP_NETWORK_intERFACE_REQ_MSG;

typedef struct _QMIWDS_STOP_NETWORK_intERFACE_RESP_MSG {
    ushort Type;             // QMUX type 0x0040
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT

} __attribute__ ((packed)) QMIWDS_STOP_NETWORK_intERFACE_RESP_MSG, *PQMIWDS_STOP_NETWORK_intERFACE_RESP_MSG;

typedef struct _QMIWDS_GET_DEFAULT_SETTINGS_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ProfileType;
} __attribute__ ((packed)) QMIWDS_GET_DEFAULT_SETTINGS_REQ_MSG, *PQMIWDS_GET_DEFAULT_SETTINGS_REQ_MSG;

typedef struct _QMIWDS_GET_DEFAULT_SETTINGS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMIWDS_GET_DEFAULT_SETTINGS_RESP_MSG, *PQMIWDS_GET_DEFAULT_SETTINGS_RESP_MSG;

typedef struct _QMIWDS_MODIFY_PROFILE_SETTINGS_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ProfileType;
    uchar  ProfileIndex;
} __attribute__ ((packed)) QMIWDS_MODIFY_PROFILE_SETTINGS_REQ_MSG, *PQMIWDS_MODIFY_PROFILE_SETTINGS_REQ_MSG;

typedef struct _QMIWDS_MODIFY_PROFILE_SETTINGS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMIWDS_MODIFY_PROFILE_SETTINGS_RESP_MSG, *PQMIWDS_MODIFY_PROFILE_SETTINGS_RESP_MSG;

typedef struct _QMIWDS_GET_PROFILE_SETTINGS_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ProfileType;
    uchar  ProfileIndex;
} __attribute__ ((packed)) QMIWDS_GET_PROFILE_SETTINGS_REQ_MSG, *PQMIWDS_GET_PROFILE_SETTINGS_REQ_MSG;

#if 0
typedef struct _QMIWDS_EVENT_REPORT_IND_DATA_BEARER_TLV {
    uchar  Type;
    ushort Length;
    uchar  DataBearer;
} QMIWDS_EVENT_REPORT_IND_DATA_BEARER_TLV, *PQMIWDS_EVENT_REPORT_IND_DATA_BEARER_TLV;

typedef struct _QMIWDS_EVENT_REPORT_IND_DORMANCY_STATUS_TLV {
    uchar  Type;
    ushort Length;
    uchar  DormancyStatus;
} QMIWDS_EVENT_REPORT_IND_DORMANCY_STATUS_TLV, *PQMIWDS_EVENT_REPORT_IND_DORMANCY_STATUS_TLV;


typedef struct _QMIWDS_GET_DATA_BEARER_REQ_MSG {
    ushort Type;             // QMUX type 0x0037
    ushort Length;
} QMIWDS_GET_DATA_BEARER_REQ_MSG, *PQMIWDS_GET_DATA_BEARER_REQ_MSG;

typedef struct _QMIWDS_GET_DATA_BEARER_RESP_MSG {
    ushort Type;             // QMUX type 0x0037
    ushort Length;
    uchar  TLVType;          // 0x02
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_intERNAL
    // QMI_ERR_MALFORMED_MSG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_OUT_OF_CALL
    // QMI_ERR_INFO_UNAVAILABLE
    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       //
    uchar  Technology;       //
} QMIWDS_GET_DATA_BEARER_RESP_MSG, *PQMIWDS_GET_DATA_BEARER_RESP_MSG;
#endif

// ======================= DMS ==============================
#define QMIDMS_SET_EVENT_REPORT_REQ           0x0001
#define QMIDMS_SET_EVENT_REPORT_RESP          0x0001
#define QMIDMS_EVENT_REPORT_IND               0x0001
#define QMIDMS_GET_DEVICE_CAP_REQ             0x0020
#define QMIDMS_GET_DEVICE_CAP_RESP            0x0020
#define QMIDMS_GET_DEVICE_MFR_REQ             0x0021
#define QMIDMS_GET_DEVICE_MFR_RESP            0x0021
#define QMIDMS_GET_DEVICE_MODEL_ID_REQ        0x0022
#define QMIDMS_GET_DEVICE_MODEL_ID_RESP       0x0022
#define QMIDMS_GET_DEVICE_REV_ID_REQ          0x0023
#define QMIDMS_GET_DEVICE_REV_ID_RESP         0x0023
#define QMIDMS_GET_MSISDN_REQ                 0x0024
#define QMIDMS_GET_MSISDN_RESP                0x0024
#define QMIDMS_GET_DEVICE_SERIAL_NUMBERS_REQ  0x0025
#define QMIDMS_GET_DEVICE_SERIAL_NUMBERS_RESP 0x0025
#define QMIDMS_UIM_SET_PIN_PROTECTION_REQ     0x0027
#define QMIDMS_UIM_SET_PIN_PROTECTION_RESP    0x0027
#define QMIDMS_UIM_VERIFY_PIN_REQ             0x0028
#define QMIDMS_UIM_VERIFY_PIN_RESP            0x0028
#define QMIDMS_UIM_UNBLOCK_PIN_REQ            0x0029
#define QMIDMS_UIM_UNBLOCK_PIN_RESP           0x0029
#define QMIDMS_UIM_CHANGE_PIN_REQ             0x002A
#define QMIDMS_UIM_CHANGE_PIN_RESP            0x002A
#define QMIDMS_UIM_GET_PIN_STATUS_REQ         0x002B
#define QMIDMS_UIM_GET_PIN_STATUS_RESP        0x002B
#define QMIDMS_GET_DEVICE_HARDWARE_REV_REQ    0x002C
#define QMIDMS_GET_DEVICE_HARDWARE_REV_RESP   0x002C
#define QMIDMS_GET_OPERATING_MODE_REQ         0x002D
#define QMIDMS_GET_OPERATING_MODE_RESP        0x002D
#define QMIDMS_SET_OPERATING_MODE_REQ         0x002E
#define QMIDMS_SET_OPERATING_MODE_RESP        0x002E
#define QMIDMS_GET_ACTIVATED_STATUS_REQ       0x0031
#define QMIDMS_GET_ACTIVATED_STATUS_RESP      0x0031
#define QMIDMS_ACTIVATE_AUTOMATIC_REQ         0x0032
#define QMIDMS_ACTIVATE_AUTOMATIC_RESP        0x0032
#define QMIDMS_ACTIVATE_MANUAL_REQ            0x0033
#define QMIDMS_ACTIVATE_MANUAL_RESP           0x0033
#define QMIDMS_UIM_GET_ICCID_REQ              0x003C
#define QMIDMS_UIM_GET_ICCID_RESP             0x003C
#define QMIDMS_UIM_GET_CK_STATUS_REQ          0x0040
#define QMIDMS_UIM_GET_CK_STATUS_RESP         0x0040
#define QMIDMS_UIM_SET_CK_PROTECTION_REQ      0x0041
#define QMIDMS_UIM_SET_CK_PROTECTION_RESP     0x0041
#define QMIDMS_UIM_UNBLOCK_CK_REQ             0x0042
#define QMIDMS_UIM_UNBLOCK_CK_RESP            0x0042
#define QMIDMS_UIM_GET_IMSI_REQ               0x0043
#define QMIDMS_UIM_GET_IMSI_RESP              0x0043
#define QMIDMS_UIM_GET_STATE_REQ              0x0044
#define QMIDMS_UIM_GET_STATE_RESP             0x0044
#define QMIDMS_GET_BAND_CAP_REQ               0x0045
#define QMIDMS_GET_BAND_CAP_RESP              0x0045

#if 0
typedef struct _QMIDMS_GET_DEVICE_MFR_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMIDMS_GET_DEVICE_MFR_REQ_MSG, *PQMIDMS_GET_DEVICE_MFR_REQ_MSG;

typedef struct _QMIDMS_GET_DEVICE_MFR_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;         // 0x01 - required parameter
    ushort TLV2Length;       // length of the mfr string
    uchar  DeviceManufacturer; // first byte of string
} QMIDMS_GET_DEVICE_MFR_RESP_MSG, *PQMIDMS_GET_DEVICE_MFR_RESP_MSG;

typedef struct _QMIDMS_GET_DEVICE_MODEL_ID_REQ_MSG {
    ushort Type;             // QMUX type 0x0004
    ushort Length;
} QMIDMS_GET_DEVICE_MODEL_ID_REQ_MSG, *PQMIDMS_GET_DEVICE_MODEL_ID_REQ_MSG;

typedef struct _QMIDMS_GET_DEVICE_MODEL_ID_RESP_MSG {
    ushort Type;             // QMUX type 0x0004
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;         // 0x01 - required parameter
    ushort TLV2Length;       // length of the modem id string
    uchar  DeviceModelID;    // device model id
} QMIDMS_GET_DEVICE_MODEL_ID_RESP_MSG, *PQMIDMS_GET_DEVICE_MODEL_ID_RESP_MSG;
#endif

typedef struct _QMIDMS_GET_DEVICE_REV_ID_REQ_MSG {
    ushort Type;             // QMUX type 0x0005
    ushort Length;
} __attribute__ ((packed)) QMIDMS_GET_DEVICE_REV_ID_REQ_MSG, *PQMIDMS_GET_DEVICE_REV_ID_REQ_MSG;

typedef struct _DEVICE_REV_ID {
    uchar  TLVType;
    ushort TLVLength;
    uchar  RevisionID;
} __attribute__ ((packed)) DEVICE_REV_ID, *PDEVICE_REV_ID;

#if 0
typedef struct _QMIDMS_GET_DEVICE_REV_ID_RESP_MSG {
    ushort Type;             // QMUX type 0x0023
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMIDMS_GET_DEVICE_REV_ID_RESP_MSG, *PQMIDMS_GET_DEVICE_REV_ID_RESP_MSG;

typedef struct _QMIDMS_GET_MSISDN_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
} QMIDMS_GET_MSISDN_REQ_MSG, *PQMIDMS_GET_MSISDN_REQ_MSG;

typedef struct _QCTLV_DEVICE_VOICE_NUMBERS {
    uchar  TLVType;            // as defined above
    ushort TLVLength;          // 4/7/7
    uchar  VoideNumberString; // ESN, IMEI, or MEID

} QCTLV_DEVICE_VOICE_NUMBERS, *PQCTLV_DEVICE_VOICE_NUMBERS;


typedef struct _QMIDMS_GET_MSISDN_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
} QMIDMS_GET_MSISDN_RESP_MSG, *PQMIDMS_GET_MSISDN_RESP_MSG;
#endif

typedef struct _QMIDMS_UIM_GET_IMSI_REQ_MSG {
    ushort Type;
    ushort Length;
} __attribute__ ((packed)) QMIDMS_UIM_GET_IMSI_REQ_MSG, *PQMIDMS_UIM_GET_IMSI_REQ_MSG;

typedef struct _QMIDMS_UIM_GET_IMSI_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar IMSI;
} __attribute__ ((packed)) QMIDMS_UIM_GET_IMSI_RESP_MSG, *PQMIDMS_UIM_GET_IMSI_RESP_MSG;

#if 0
typedef struct _QMIDMS_GET_DEVICE_SERIAL_NUMBERS_REQ_MSG {
    ushort Type;             // QMUX type 0x0007
    ushort Length;
} QMIDMS_GET_DEVICE_SERIAL_NUMBERS_REQ_MSG, *PQMIDMS_GET_DEVICE_SERIAL_NUMBERS_REQ_MSG;

#define QCTLV_TYPE_SER_NUM_ESN  0x10
#define QCTLV_TYPE_SER_NUM_IMEI 0x11
#define QCTLV_TYPE_SER_NUM_MEID 0x12

typedef struct _QCTLV_DEVICE_SERIAL_NUMBER {
    uchar  TLVType;            // as defined above
    ushort TLVLength;          // 4/7/7
    uchar  SerialNumberString; // ESN, IMEI, or MEID

} QCTLV_DEVICE_SERIAL_NUMBER, *PQCTLV_DEVICE_SERIAL_NUMBER;

typedef struct _QMIDMS_GET_DEVICE_SERIAL_NUMBERS_RESP_MSG {
    ushort Type;             // QMUX type 0x0007
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    // followed by optional TLV
} QMIDMS_GET_DEVICE_SERIAL_NUMBERS_RESP_MSG, *PQMIDMS_GET_DEVICE_SERIAL_NUMBERS_RESP;

typedef struct _QMIDMS_GET_DMS_BAND_CAP {
    ushort  Type;
    ushort  Length;
} QMIDMS_GET_BAND_CAP_REQ_MSG, *PQMIDMS_GET_BAND_CAP_REQ_MSG;

typedef struct _QMIDMS_GET_BAND_CAP_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_NONE
    // QMI_ERR_intERNAL
    // QMI_ERR_MALFORMED_MSG
    // QMI_ERR_NO_MEMORY

    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 2
    uint64 BandCap;
} QMIDMS_GET_BAND_CAP_RESP_MSG, *PQMIDMS_GET_BAND_CAP_RESP;

typedef struct _QMIDMS_GET_DEVICE_CAP_REQ_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
} QMIDMS_GET_DEVICE_CAP_REQ_MSG, *PQMIDMS_GET_DEVICE_CAP_REQ_MSG;

typedef struct _QMIDMS_GET_DEVICE_CAP_RESP_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMUX_RESULT_SUCCESS
    // QMUX_RESULT_FAILURE
    ushort QMUXError;        // QMUX_ERR_INVALID_ARG
    // QMUX_ERR_NO_MEMORY
    // QMUX_ERR_intERNAL
    // QMUX_ERR_FAULT
    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 2

    uint  MaxTxChannelRate;
    uint  MaxRxChannelRate;
    uchar  VoiceCap;
    uchar  SimCap;

    uchar  RadioIfListCnt;   // #elements in radio interface list
    uchar  RadioIfList;      // N 1-byte elements
} QMIDMS_GET_DEVICE_CAP_RESP_MSG, *PQMIDMS_GET_DEVICE_CAP_RESP_MSG;

typedef struct _QMIDMS_GET_ACTIVATED_STATUS_REQ_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
} QMIDMS_GET_ACTIVATED_STATUS_REQ_MSG, *PQMIDMS_GET_ACTIVATES_STATUD_REQ_MSG;

typedef struct _QMIDMS_GET_ACTIVATED_STATUS_RESP_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMUX_RESULT_SUCCESS
    // QMUX_RESULT_FAILURE
    ushort QMUXError;        // QMUX_ERR_INVALID_ARG
    // QMUX_ERR_NO_MEMORY
    // QMUX_ERR_intERNAL
    // QMUX_ERR_FAULT
    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 2

    ushort ActivatedStatus;
} QMIDMS_GET_ACTIVATED_STATUS_RESP_MSG, *PQMIDMS_GET_ACTIVATED_STATUS_RESP_MSG;

typedef struct _QMIDMS_GET_OPERATING_MODE_REQ_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
} QMIDMS_GET_OPERATING_MODE_REQ_MSG, *PQMIDMS_GET_OPERATING_MODE_REQ_MSG;

typedef struct _OFFLINE_REASON {
    uchar  TLVType;
    ushort TLVLength;
    ushort OfflineReason;
} OFFLINE_REASON, *POFFLINE_REASON;

typedef struct _HARDWARE_RESTRICTED_MODE {
    uchar  TLVType;
    ushort TLVLength;
    uchar  HardwareControlledMode;
} HARDWARE_RESTRICTED_MODE, *PHARDWARE_RESTRICTED_MODE;

typedef struct _QMIDMS_GET_OPERATING_MODE_RESP_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMUX_RESULT_SUCCESS
    // QMUX_RESULT_FAILURE
    ushort QMUXError;        // QMUX_ERR_INVALID_ARG
    // QMUX_ERR_NO_MEMORY
    // QMUX_ERR_intERNAL
    // QMUX_ERR_FAULT
    uchar  TLV2Type;         // 0x01
    ushort TLV2Length;       // 2

    uchar  OperatingMode;
} QMIDMS_GET_OPERATING_MODE_RESP_MSG, *PQMIDMS_GET_OPERATING_MODE_RESP_MSG;

typedef struct _QMIDMS_UIM_GET_ICCID_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
} QMIDMS_UIM_GET_ICCID_REQ_MSG, *PQMIDMS_UIM_GET_ICCID_REQ_MSG;

typedef struct _QMIDMS_UIM_GET_ICCID_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;         // 0x01 - required parameter
    ushort TLV2Length;       // var
    uchar  ICCID;      // String of voice number
} QMIDMS_UIM_GET_ICCID_RESP_MSG, *PQMIDMS_UIM_GET_ICCID_RESP_MSG;
#endif

typedef struct _QMIDMS_SET_OPERATING_MODE_REQ_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  OperatingMode;
} __attribute__ ((packed)) QMIDMS_SET_OPERATING_MODE_REQ_MSG, *PQMIDMS_SET_OPERATING_MODE_REQ_MSG;

typedef struct _QMIDMS_SET_OPERATING_MODE_RESP_MSG {
    ushort Type;             // QMUX type 0x0002
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMUX_RESULT_SUCCESS
    // QMUX_RESULT_FAILURE
    ushort QMUXError;        // QMUX_ERR_INVALID_ARG
    // QMUX_ERR_NO_MEMORY
    // QMUX_ERR_intERNAL
    // QMUX_ERR_FAULT
} __attribute__ ((packed)) QMIDMS_SET_OPERATING_MODE_RESP_MSG, *PQMIDMS_SET_OPERATING_MODE_RESP_MSG;

#if 0
typedef struct _QMIDMS_ACTIVATE_AUTOMATIC_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        //
    uchar  ActivateCodelen;
    uchar  ActivateCode;
} QMIDMS_ACTIVATE_AUTOMATIC_REQ_MSG, *PQMIDMS_ACTIVATE_AUTOMATIC_REQ_MSG;

typedef struct _QMIDMS_ACTIVATE_AUTOMATIC_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMIDMS_ACTIVATE_AUTOMATIC_RESP_MSG, *PQMIDMS_ACTIVATE_AUTOMATIC_RESP_MSG;


typedef struct _SPC_MSG {
    uchar SPC[6];
    ushort SID;
} SPC_MSG, *PSPC_MSG;

typedef struct _MDN_MSG {
    uchar MDNLEN;
    uchar MDN;
} MDN_MSG, *PMDN_MSG;

typedef struct _MIN_MSG {
    uchar MINLEN;
    uchar MIN;
} MIN_MSG, *PMIN_MSG;

typedef struct _PRL_MSG {
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        //
    ushort PRLLEN;
    uchar PRL;
} PRL_MSG, *PPRL_MSG;

typedef struct _MN_HA_KEY_MSG {
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        //
    uchar MN_HA_KEY_LEN;
    uchar MN_HA_KEY;
} MN_HA_KEY_MSG, *PMN_HA_KEY_MSG;

typedef struct _MN_AAA_KEY_MSG {
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        //
    uchar MN_AAA_KEY_LEN;
    uchar MN_AAA_KEY;
} MN_AAA_KEY_MSG, *PMN_AAA_KEY_MSG;

typedef struct _QMIDMS_ACTIVATE_MANUAL_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        //
    uchar  Value;
} QMIDMS_ACTIVATE_MANUAL_REQ_MSG, *PQMIDMS_ACTIVATE_MANUAL_REQ_MSG;

typedef struct _QMIDMS_ACTIVATE_MANUAL_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMIDMS_ACTIVATE_MANUAL_RESP_MSG, *PQMIDMS_ACTIVATE_MANUAL_RESP_MSG;
#endif

typedef struct _QMIDMS_UIM_GET_STATE_REQ_MSG {
    ushort Type;
    ushort Length;
} __attribute__ ((packed)) QMIDMS_UIM_GET_STATE_REQ_MSG, *PQMIDMS_UIM_GET_STATE_REQ_MSG;

typedef struct _QMIDMS_UIM_GET_STATE_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  UIMState;
} __attribute__ ((packed)) QMIDMS_UIM_GET_STATE_RESP_MSG, *PQMIDMS_UIM_GET_STATE_RESP_MSG;

typedef struct _QMIDMS_UIM_GET_PIN_STATUS_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
} __attribute__ ((packed)) QMIDMS_UIM_GET_PIN_STATUS_REQ_MSG, *PQMIDMS_UIM_GET_PIN_STATUS_REQ_MSG;

typedef struct _QMIDMS_UIM_PIN_STATUS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  PINStatus;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} __attribute__ ((packed)) QMIDMS_UIM_PIN_STATUS, *PQMIDMS_UIM_PIN_STATUS;

#define QMI_PIN_STATUS_NOT_INIT      0
#define QMI_PIN_STATUS_NOT_VERIF     1
#define QMI_PIN_STATUS_VERIFIED      2
#define QMI_PIN_STATUS_DISABLED      3
#define QMI_PIN_STATUS_BLOCKED       4
#define QMI_PIN_STATUS_PERM_BLOCKED  5
#define QMI_PIN_STATUS_UNBLOCKED     6
#define QMI_PIN_STATUS_CHANGED       7


typedef struct _QMIDMS_UIM_GET_PIN_STATUS_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar PinStatus;
} __attribute__ ((packed)) QMIDMS_UIM_GET_PIN_STATUS_RESP_MSG, *PQMIDMS_UIM_GET_PIN_STATUS_RESP_MSG;

#if 0
typedef struct _QMIDMS_UIM_GET_CK_STATUS_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  Facility;
} QMIDMS_UIM_GET_CK_STATUS_REQ_MSG, *PQMIDMS_UIM_GET_CK_STATUS_REQ_MSG;


typedef struct _QMIDMS_UIM_CK_STATUS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  FacilityStatus;
    uchar  FacilityVerifyRetriesLeft;
    uchar  FacilityUnblockRetriesLeft;
} QMIDMS_UIM_CK_STATUS, *PQMIDMS_UIM_CK_STATUS;

typedef struct _QMIDMS_UIM_CK_OPERATION_STATUS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  OperationBlocking;
} QMIDMS_UIM_CK_OPERATION_STATUS, *PQMIDMS_UIM_CK_OPERATION_STATUS;

typedef struct _QMIDMS_UIM_GET_CK_STATUS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  CkStatus;
} QMIDMS_UIM_GET_CK_STATUS_RESP_MSG, *PQMIDMS_UIM_GET_CK_STATUS_RESP_MSG;
#endif

typedef struct _QMIDMS_UIM_VERIFY_PIN_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  PINID;
    uchar  PINLen;
    uchar  PINValue;
} __attribute__ ((packed)) QMIDMS_UIM_VERIFY_PIN_REQ_MSG, *PQMIDMS_UIM_VERIFY_PIN_REQ_MSG;

typedef struct _QMIDMS_UIM_VERIFY_PIN_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} __attribute__ ((packed)) QMIDMS_UIM_VERIFY_PIN_RESP_MSG, *PQMIDMS_UIM_VERIFY_PIN_RESP_MSG;

#if 0
typedef struct _QMIDMS_UIM_SET_PIN_PROTECTION_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  PINID;
    uchar  ProtectionSetting;
    uchar  PINLen;
    uchar  PINValue;
} QMIDMS_UIM_SET_PIN_PROTECTION_REQ_MSG, *PQMIDMS_UIM_SET_PIN_PROTECTION_REQ_MSG;

typedef struct _QMIDMS_UIM_SET_PIN_PROTECTION_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} QMIDMS_UIM_SET_PIN_PROTECTION_RESP_MSG, *PQMIDMS_UIM_SET_PIN_PROTECTION_RESP_MSG;

typedef struct _QMIDMS_UIM_SET_CK_PROTECTION_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  Facility;
    uchar  FacilityState;
    uchar  FacliltyLen;
    uchar  FacliltyValue;
} QMIDMS_UIM_SET_CK_PROTECTION_REQ_MSG, *PQMIDMS_UIM_SET_CK_PROTECTION_REQ_MSG;

typedef struct _QMIDMS_UIM_SET_CK_PROTECTION_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  FacilityRetriesLeft;
} QMIDMS_UIM_SET_CK_PROTECTION_RESP_MSG, *PQMIDMS_UIM_SET_CK_PROTECTION_RESP_MSG;


typedef struct _UIM_PIN {
    uchar  PinLength;
    uchar  PinValue;
} UIM_PIN, *PUIM_PIN;

typedef struct _QMIDMS_UIM_CHANGE_PIN_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  PINID;
    uchar  PinDetails;
} QMIDMS_UIM_CHANGE_PIN_REQ_MSG, *PQMIDMS_UIM_CHANGE_PIN_REQ_MSG;

typedef struct QMIDMS_UIM_CHANGE_PIN_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} QMIDMS_UIM_CHANGE_PIN_RESP_MSG, *PQMIDMS_UIM_CHANGE_PIN_RESP_MSG;

typedef struct _UIM_PUK {
    uchar  PukLength;
    uchar  PukValue;
} UIM_PUK, *PUIM_PUK;

typedef struct _QMIDMS_UIM_UNBLOCK_PIN_REQ_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  PINID;
    uchar  PinDetails;
} QMIDMS_UIM_UNBLOCK_PIN_REQ_MSG, *PQMIDMS_UIM_BLOCK_PIN_REQ_MSG;

typedef struct QMIDMS_UIM_UNBLOCK_PIN_RESP_MSG {
    ushort Type;             // QMUX type 0x0024
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} QMIDMS_UIM_UNBLOCK_PIN_RESP_MSG, *PQMIDMS_UIM_UNBLOCK_PIN_RESP_MSG;

typedef struct _QMIDMS_UIM_UNBLOCK_CK_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  Facility;
    uchar  FacliltyUnblockLen;
    uchar  FacliltyUnblockValue;
} QMIDMS_UIM_UNBLOCK_CK_REQ_MSG, *PQMIDMS_UIM_BLOCK_CK_REQ_MSG;

typedef struct QMIDMS_UIM_UNBLOCK_CK_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  FacilityUnblockRetriesLeft;
} QMIDMS_UIM_UNBLOCK_CK_RESP_MSG, *PQMIDMS_UIM_UNBLOCK_CK_RESP_MSG;

typedef struct _QMIDMS_SET_EVENT_REPORT_REQ_MSG {
    ushort Type;
    ushort Length;
} QMIDMS_SET_EVENT_REPORT_REQ_MSG, *PQMIDMS_SET_EVENT_REPORT_REQ_MSG;

typedef struct _QMIDMS_SET_EVENT_REPORT_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
} QMIDMS_SET_EVENT_REPORT_RESP_MSG, *PQMIDMS_SET_EVENT_REPORT_RESP_MSG;

typedef struct _PIN_STATUS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ReportPinState;
} PIN_STATUS, *PPIN_STATUS;

typedef struct _POWER_STATUS {
    uchar TLVType;
    ushort TLVLength;
    uchar PowerStatus;
    uchar BatteryLvl;
} POWER_STATUS, *PPOWER_STATUS;

typedef struct _ACTIVATION_STATE {
    uchar TLVType;
    ushort TLVLength;
    ushort ActivationState;
} ACTIVATION_STATE, *PACTIVATION_STATE;

typedef struct _ACTIVATION_STATE_REQ {
    uchar TLVType;
    ushort TLVLength;
    uchar ActivationState;
} ACTIVATION_STATE_REQ, *PACTIVATION_STATE_REQ;

typedef struct _OPERATING_MODE {
    uchar TLVType;
    ushort TLVLength;
    uchar OperatingMode;
} OPERATING_MODE, *POPERATING_MODE;

typedef struct _UIM_STATE {
    uchar TLVType;
    ushort TLVLength;
    uchar UIMState;
} UIM_STATE, *PUIM_STATE;

typedef struct _WIRELESS_DISABLE_STATE {
    uchar TLVType;
    ushort TLVLength;
    uchar WirelessDisableState;
} WIRELESS_DISABLE_STATE, *PWIRELESS_DISABLE_STATE;

typedef struct _QMIDMS_EVENT_REPORT_IND_MSG {
    ushort Type;
    ushort Length;
} QMIDMS_EVENT_REPORT_IND_MSG, *PQMIDMS_EVENT_REPORT_IND_MSG;
#endif

// ============================ END OF DMS ===============================

// ======================= QOS ==============================
typedef struct _MPIOC_DEV_INFO MPIOC_DEV_INFO, *PMPIOC_DEV_INFO;

#define QMI_QOS_SET_EVENT_REPORT_REQ  0x0001
#define QMI_QOS_SET_EVENT_REPORT_RESP 0x0001
#define QMI_QOS_EVENT_REPORT_IND      0x0001

#if 0
typedef struct _QMI_QOS_SET_EVENT_REPORT_REQ_MSG {
    ushort Type;             // QMUX type 0x0001
    ushort Length;
    // uchar  TLVType;          // 0x01 - physical link state
    // ushort TLVLength;        // 1
    // uchar  PhyLinkStatusRpt; // 0-enable; 1-disable
    uchar  TLVType2;         // 0x02 = global flow reporting
    ushort TLVLength2;       // 1
    uchar  GlobalFlowRpt;    // 1-enable; 0-disable
} QMI_QOS_SET_EVENT_REPORT_REQ_MSG, *PQMI_QOS_SET_EVENT_REPORT_REQ_MSG;

typedef struct _QMI_QOS_SET_EVENT_REPORT_RESP_MSG {
    ushort Type;             // QMUX type 0x0010
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMUX_RESULT_SUCCESS
    // QMUX_RESULT_FAILURE
    ushort QMUXError;        // QMUX_ERR_INVALID_ARG
    // QMUX_ERR_NO_MEMORY
    // QMUX_ERR_intERNAL
    // QMUX_ERR_FAULT
} QMI_QOS_SET_EVENT_REPORT_RESP_MSG, *PQMI_QOS_SET_EVENT_REPORT_RESP_MSG;

typedef struct _QMI_QOS_EVENT_REPORT_IND_MSG {
    ushort Type;             // QMUX type 0x0001
    ushort Length;
    uchar  TLVs;
} QMI_QOS_EVENT_REPORT_IND_MSG, *PQMI_QOS_EVENT_REPORT_IND_MSG;

#define QOS_EVENT_RPT_IND_FLOW_ACTIVATED 0x01
#define QOS_EVENT_RPT_IND_FLOW_MODIFIED  0x02
#define QOS_EVENT_RPT_IND_FLOW_DELETED   0x03
#define QOS_EVENT_RPT_IND_FLOW_SUSPENDED 0x04
#define QOS_EVENT_RPT_IND_FLOW_ENABLED   0x05
#define QOS_EVENT_RPT_IND_FLOW_DISABLED  0x06

#define QOS_EVENT_RPT_IND_TLV_PHY_LINK_STATE_TYPE 0x01
#define QOS_EVENT_RPT_IND_TLV_GLOBAL_FL_RPT_STATE 0x10
#define QOS_EVENT_RPT_IND_TLV_GLOBAL_FL_RPT_TYPE  0x10
#define QOS_EVENT_RPT_IND_TLV_TX_FLOW_TYPE        0x11
#define QOS_EVENT_RPT_IND_TLV_RX_FLOW_TYPE        0x12
#define QOS_EVENT_RPT_IND_TLV_TX_FILTER_TYPE      0x13
#define QOS_EVENT_RPT_IND_TLV_RX_FILTER_TYPE      0x14
#define QOS_EVENT_RPT_IND_TLV_FLOW_SPEC           0x10
#define QOS_EVENT_RPT_IND_TLV_FILTER_SPEC         0x10

typedef struct _QOS_EVENT_RPT_IND_TLV_PHY_LINK_STATE {
    uchar  TLVType;       // 0x01
    ushort TLVLength;     // 1
    uchar  PhyLinkState;  // 0-dormant, 1-active
} QOS_EVENT_RPT_IND_TLV_PHY_LINK_STATE, *PQOS_EVENT_RPT_IND_TLV_PHY_LINK_STATE;

typedef struct _QOS_EVENT_RPT_IND_TLV_GLOBAL_FL_RPT {
    uchar  TLVType;       // 0x10
    ushort TLVLength;     // 6
    uint  QosId;
    uchar  NewFlow;       // 1: newly added flow; 0: existing flow
    uchar  StateChange;   // 1: activated; 2: modified; 3: deleted;
    // 4: suspended(delete); 5: enabled; 6: disabled
} QOS_EVENT_RPT_IND_TLV_GLOBAL_FL_RPT, *PQOS_EVENT_RPT_IND_TLV_GLOBAL_FL_RPT;

// QOS Flow

typedef struct _QOS_EVENT_RPT_IND_TLV_FLOW {
    uchar  TLVType;       // 0x10-TX flow; 0x11-RX flow
    ushort TLVLength;     // var
    // embedded TLV's
} QOS_EVENT_RPT_IND_TLV_TX_FLOW, *PQOS_EVENT_RPT_IND_TLV_TX_FLOW;

#define QOS_FLOW_TLV_IP_FLOW_IDX_TYPE                    0x10
#define QOS_FLOW_TLV_IP_FLOW_TRAFFIC_CLASS_TYPE          0x11
#define QOS_FLOW_TLV_IP_FLOW_DATA_RATE_MIN_MAX_TYPE      0x12
#define QOS_FLOW_TLV_IP_FLOW_DATA_RATE_TOKEN_BUCKET_TYPE 0x13
#define QOS_FLOW_TLV_IP_FLOW_LATENCY_TYPE                0x14
#define QOS_FLOW_TLV_IP_FLOW_JITTER_TYPE                 0x15
#define QOS_FLOW_TLV_IP_FLOW_PKT_ERR_RATE_TYPE           0x16
#define QOS_FLOW_TLV_IP_FLOW_MIN_PKT_SIZE_TYPE           0x17
#define QOS_FLOW_TLV_IP_FLOW_MAX_PKT_SIZE_TYPE           0x18
#define QOS_FLOW_TLV_IP_FLOW_3GPP_BIT_ERR_RATE_TYPE      0x19
#define QOS_FLOW_TLV_IP_FLOW_3GPP_TRAF_PRIORITY_TYPE     0x1A
#define QOS_FLOW_TLV_IP_FLOW_3GPP2_PROFILE_ID_TYPE       0x1B

typedef struct _QOS_FLOW_TLV_IP_FLOW_IDX {
    uchar  TLVType;       // 0x10
    ushort TLVLength;     // 1
    uchar  IpFlowIndex;
}  QOS_FLOW_TLV_IP_FLOW_IDX, *PQOS_FLOW_TLV_IP_FLOW_IDX;

typedef struct _QOS_FLOW_TLV_IP_FLOW_TRAFFIC_CLASS {
    uchar  TLVType;       // 0x11
    ushort TLVLength;     // 1
    uchar  TrafficClass;
}  QOS_FLOW_TLV_IP_FLOW_TRAFFIC_CLASS, *PQOS_FLOW_TLV_IP_FLOW_TRAFFIC_CLASS;

typedef struct _QOS_FLOW_TLV_IP_FLOW_DATA_RATE_MIN_MAX {
    uchar  TLVType;       // 0x12
    ushort TLVLength;     // 8
    uint  DataRateMax;
    uint  GuaranteedRate;
}  QOS_FLOW_TLV_IP_FLOW_DATA_RATE_MIN_MAX, *PQOS_FLOW_TLV_IP_FLOW_DATA_RATE_MIN_MAX;

typedef struct _QOS_FLOW_TLV_IP_FLOW_DATA_RATE_TOKEN_BUCKET {
    uchar  TLVType;       // 0x13
    ushort TLVLength;     // 12
    uint  PeakRate;
    uint  TokenRate;
    uint  BucketSize;
}  QOS_FLOW_TLV_IP_FLOW_DATA_RATE_TOKEN_BUCKET, *PQOS_FLOW_TLV_IP_FLOW_DATA_RATE_TOKEN_BUCKET;

typedef struct _QOS_FLOW_TLV_IP_FLOW_LATENCY {
    uchar  TLVType;       // 0x14
    ushort TLVLength;     // 4
    uint  IpFlowLatency;
}  QOS_FLOW_TLV_IP_FLOW_LATENCY, *PQOS_FLOW_TLV_IP_FLOW_LATENCY;

typedef struct _QOS_FLOW_TLV_IP_FLOW_JITTER {
    uchar  TLVType;       // 0x15
    ushort TLVLength;     // 4
    uint  IpFlowJitter;
}  QOS_FLOW_TLV_IP_FLOW_JITTER, *PQOS_FLOW_TLV_IP_FLOW_JITTER;

typedef struct _QOS_FLOW_TLV_IP_FLOW_PKT_ERR_RATE {
    uchar  TLVType;       // 0x16
    ushort TLVLength;     // 4
    ushort ErrRateMultiplier;
    ushort ErrRateExponent;
}  QOS_FLOW_TLV_IP_FLOW_PKT_ERR_RATE, *PQOS_FLOW_TLV_IP_FLOW_PKT_ERR_RATE;

typedef struct _QOS_FLOW_TLV_IP_FLOW_MIN_PKT_SIZE {
    uchar  TLVType;       // 0x17
    ushort TLVLength;     // 4
    uint  MinPolicedPktSize;
}  QOS_FLOW_TLV_IP_FLOW_MIN_PKT_SIZE, *PQOS_FLOW_TLV_IP_FLOW_MIN_PKT_SIZE;

typedef struct _QOS_FLOW_TLV_IP_FLOW_MAX_PKT_SIZE {
    uchar  TLVType;       // 0x18
    ushort TLVLength;     // 4
    uint  MaxAllowedPktSize;
}  QOS_FLOW_TLV_IP_FLOW_MAX_PKT_SIZE, *PQOS_FLOW_TLV_IP_FLOW_MAX_PKT_SIZE;

typedef struct _QOS_FLOW_TLV_IP_FLOW_3GPP_BIT_ERR_RATE {
    uchar  TLVType;       // 0x19
    ushort TLVLength;     // 1
    uchar  ResidualBitErrorRate;
}  QOS_FLOW_TLV_IP_FLOW_3GPP_BIT_ERR_RATE, *PQOS_FLOW_TLV_IP_FLOW_3GPP_BIT_ERR_RATE;

typedef struct _QOS_FLOW_TLV_IP_FLOW_3GPP_TRAF_PRIORITY {
    uchar  TLVType;       // 0x1A
    ushort TLVLength;     // 1
    uchar  TrafficHandlingPriority;
}  QOS_FLOW_TLV_IP_FLOW_3GPP_TRAF_PRIORITY, *PQOS_FLOW_TLV_IP_FLOW_3GPP_TRAF_PRIORITY;

typedef struct _QOS_FLOW_TLV_IP_FLOW_3GPP2_PROFILE_ID {
    uchar  TLVType;       // 0x1B
    ushort TLVLength;     // 2
    ushort ProfileId;
}  QOS_FLOW_TLV_IP_FLOW_3GPP2_PROFILE_ID, *PQOS_FLOW_TLV_IP_FLOW_3GPP2_PROFILE_ID;

// QOS Filter

#define QOS_FILTER_TLV_IP_FILTER_IDX_TYPE          0x10
#define QOS_FILTER_TLV_IP_VERSION_TYPE             0x11
#define QOS_FILTER_TLV_IPV4_SRC_ADDR_TYPE          0x12
#define QOS_FILTER_TLV_IPV4_DEST_ADDR_TYPE         0x13
#define QOS_FILTER_TLV_NEXT_HDR_PROTOCOL_TYPE      0x14
#define QOS_FILTER_TLV_IPV4_TYPE_OF_SERVICE_TYPE   0x15
#define QOS_FILTER_TLV_TCP_UDP_PORT_SRC_TCP_TYPE   0x1B
#define QOS_FILTER_TLV_TCP_UDP_PORT_DEST_TCP_TYPE  0x1C
#define QOS_FILTER_TLV_TCP_UDP_PORT_SRC_UDP_TYPE   0x1D
#define QOS_FILTER_TLV_TCP_UDP_PORT_DEST_UDP_TYPE  0x1E
#define QOS_FILTER_TLV_ICMP_FILTER_MSG_TYPE_TYPE   0x1F
#define QOS_FILTER_TLV_ICMP_FILTER_MSG_CODE_TYPE   0x20
#define QOS_FILTER_TLV_TCP_UDP_PORT_SRC_TYPE       0x24
#define QOS_FILTER_TLV_TCP_UDP_PORT_DEST_TYPE      0x25

typedef struct _QOS_EVENT_RPT_IND_TLV_FILTER {
    uchar  TLVType;       // 0x12-TX filter; 0x13-RX filter
    ushort TLVLength;     // var
    // embedded TLV's
} QOS_EVENT_RPT_IND_TLV_RX_FILTER, *PQOS_EVENT_RPT_IND_TLV_RX_FILTER;

typedef struct _QOS_FILTER_TLV_IP_FILTER_IDX {
    uchar  TLVType;       // 0x10
    ushort TLVLength;     // 1
    uchar  IpFilterIndex;
}  QOS_FILTER_TLV_IP_FILTER_IDX, *PQOS_FILTER_TLV_IP_FILTER_IDX;

typedef struct _QOS_FILTER_TLV_IP_VERSION {
    uchar  TLVType;       // 0x11
    ushort TLVLength;     // 1
    uchar  IpVersion;
}  QOS_FILTER_TLV_IP_VERSION, *PQOS_FILTER_TLV_IP_VERSION;

typedef struct _QOS_FILTER_TLV_IPV4_SRC_ADDR {
    uchar  TLVType;       // 0x12
    ushort TLVLength;     // 8
    uint  IpSrcAddr;
    uint  IpSrcSubnetMask;
}  QOS_FILTER_TLV_IPV4_SRC_ADDR, *PQOS_FILTER_TLV_IPV4_SRC_ADDR;

typedef struct _QOS_FILTER_TLV_IPV4_DEST_ADDR {
    uchar  TLVType;       // 0x13
    ushort TLVLength;     // 8
    uint  IpDestAddr;
    uint  IpDestSubnetMask;
}  QOS_FILTER_TLV_IPV4_DEST_ADDR, *PQOS_FILTER_TLV_IPV4_DEST_ADDR;

typedef struct _QOS_FILTER_TLV_NEXT_HDR_PROTOCOL {
    uchar  TLVType;       // 0x14
    ushort TLVLength;     // 1
    uchar  NextHdrProtocol;
}  QOS_FILTER_TLV_NEXT_HDR_PROTOCOL, *PQOS_FILTER_TLV_NEXT_HDR_PROTOCOL;

typedef struct _QOS_FILTER_TLV_IPV4_TYPE_OF_SERVICE {
    uchar  TLVType;       // 0x15
    ushort TLVLength;     // 2
    uchar  Ipv4TypeOfService;
    uchar  Ipv4TypeOfServiceMask;
}  QOS_FILTER_TLV_IPV4_TYPE_OF_SERVICE, *PQOS_FILTER_TLV_IPV4_TYPE_OF_SERVICE;

typedef struct _QOS_FILTER_TLV_TCP_UDP_PORT {
    uchar  TLVType;       // source port: 0x1B-TCP; 0x1D-UDP
    // dest port:   0x1C-TCP; 0x1E-UDP
    ushort TLVLength;     // 4
    ushort FilterPort;
    ushort FilterPortRange;
}  QOS_FILTER_TLV_TCP_UDP_PORT, *PQOS_FILTER_TLV_TCP_UDP_PORT;

typedef struct _QOS_FILTER_TLV_ICMP_FILTER_MSG_TYPE {
    uchar  TLVType;       // 0x1F
    ushort TLVLength;     // 1
    uchar  IcmpFilterMsgType;
}  QOS_FILTER_TLV_ICMP_FILTER_MSG_TYPE, *PQOS_FILTER_TLV_ICMP_FILTER_MSG_TYPE;

typedef struct _QOS_FILTER_TLV_ICMP_FILTER_MSG_CODE {
    uchar  TLVType;       // 0x20
    ushort TLVLength;     // 1
    uchar  IcmpFilterMsgCode;
}  QOS_FILTER_TLV_ICMP_FILTER_MSG_CODE, *PQOS_FILTER_TLV_ICMP_FILTER_MSG_CODE;

#define QOS_FILTER_PRECEDENCE_INVALID  256
#define QOS_FILTER_TLV_PRECEDENCE_TYPE 0x22
#define QOS_FILTER_TLV_ID_TYPE         0x23

typedef struct _QOS_FILTER_TLV_PRECEDENCE {
    uchar  TLVType;    // 0x22
    ushort TLVLength;  // 2
    ushort Precedence; // precedence of the filter
}  QOS_FILTER_TLV_PRECEDENCE, *PQOS_FILTER_TLV_PRECEDENCE;

typedef struct _QOS_FILTER_TLV_ID {
    uchar  TLVType;    // 0x23
    ushort TLVLength;  // 2
    ushort FilterId;   // filter ID
}  QOS_FILTER_TLV_ID, *PQOS_FILTER_TLV_ID;

#ifdef QCQOS_IPV6

#define QOS_FILTER_TLV_IPV6_SRC_ADDR_TYPE          0x16
#define QOS_FILTER_TLV_IPV6_DEST_ADDR_TYPE         0x17
#define QOS_FILTER_TLV_IPV6_NEXT_HDR_PROTOCOL_TYPE 0x14  // same as IPV4
#define QOS_FILTER_TLV_IPV6_TRAFFIC_CLASS_TYPE     0x19
#define QOS_FILTER_TLV_IPV6_FLOW_LABEL_TYPE        0x1A

typedef struct _QOS_FILTER_TLV_IPV6_SRC_ADDR {
    uchar  TLVType;       // 0x16
    ushort TLVLength;     // 17
    uchar  IpSrcAddr[16];
    uchar  IpSrcAddrPrefixLen;  // [0..128]
}  QOS_FILTER_TLV_IPV6_SRC_ADDR, *PQOS_FILTER_TLV_IPV6_SRC_ADDR;

typedef struct _QOS_FILTER_TLV_IPV6_DEST_ADDR {
    uchar  TLVType;       // 0x17
    ushort TLVLength;     // 17
    uchar  IpDestAddr[16];
    uchar  IpDestAddrPrefixLen;  // [0..128]
}  QOS_FILTER_TLV_IPV6_DEST_ADDR, *PQOS_FILTER_TLV_IPV6_DEST_ADDR;

#define QOS_FILTER_IPV6_NEXT_HDR_PROTOCOL_TCP 0x06
#define QOS_FILTER_IPV6_NEXT_HDR_PROTOCOL_UDP 0x11

typedef struct _QOS_FILTER_TLV_IPV6_TRAFFIC_CLASS {
    uchar  TLVType;       // 0x19
    ushort TLVLength;     // 2
    uchar  TrafficClass;
    uchar  TrafficClassMask; // compare the first 6 bits only
}  QOS_FILTER_TLV_IPV6_TRAFFIC_CLASS, *PQOS_FILTER_TLV_IPV6_TRAFFIC_CLASS;

typedef struct _QOS_FILTER_TLV_IPV6_FLOW_LABEL {
    uchar  TLVType;       // 0x1A
    ushort TLVLength;     // 4
    uint  FlowLabel;
}  QOS_FILTER_TLV_IPV6_FLOW_LABEL, *PQOS_FILTER_TLV_IPV6_FLOW_LABEL;

#endif // QCQOS_IPV6
#endif

// ======================= WMS ==============================
#define QMIWMS_SET_EVENT_REPORT_REQ           0x0001
#define QMIWMS_SET_EVENT_REPORT_RESP          0x0001
#define QMIWMS_EVENT_REPORT_IND               0x0001
#define QMIWMS_RAW_SEND_REQ                   0x0020
#define QMIWMS_RAW_SEND_RESP                  0x0020
#define QMIWMS_RAW_WRITE_REQ                  0x0021
#define QMIWMS_RAW_WRITE_RESP                 0x0021
#define QMIWMS_RAW_READ_REQ                   0x0022
#define QMIWMS_RAW_READ_RESP                  0x0022
#define QMIWMS_MODIFY_TAG_REQ                 0x0023
#define QMIWMS_MODIFY_TAG_RESP                0x0023
#define QMIWMS_DELETE_REQ                     0x0024
#define QMIWMS_DELETE_RESP                    0x0024
#define QMIWMS_GET_MESSAGE_PROTOCOL_REQ       0x0030
#define QMIWMS_GET_MESSAGE_PROTOCOL_RESP      0x0030
#define QMIWMS_LIST_MESSAGES_REQ              0x0031
#define QMIWMS_LIST_MESSAGES_RESP             0x0031
#define QMIWMS_GET_SMSC_ADDRESS_REQ           0x0034
#define QMIWMS_GET_SMSC_ADDRESS_RESP          0x0034
#define QMIWMS_SET_SMSC_ADDRESS_REQ           0x0035
#define QMIWMS_SET_SMSC_ADDRESS_RESP          0x0035
#define QMIWMS_GET_STORE_MAX_SIZE_REQ         0x0036
#define QMIWMS_GET_STORE_MAX_SIZE_RESP        0x0036


#define WMS_MESSAGE_PROTOCOL_CDMA             0x00
#define WMS_MESSAGE_PROTOCOL_WCDMA            0x01

#if 0
typedef struct _QMIWMS_GET_MESSAGE_PROTOCOL_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMIWMS_GET_MESSAGE_PROTOCOL_REQ_MSG, *PQMIWMS_GET_MESSAGE_PROTOCOL_REQ_MSG;

typedef struct _QMIWMS_GET_MESSAGE_PROTOCOL_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  MessageProtocol;
} QMIWMS_GET_MESSAGE_PROTOCOL_RESP_MSG, *PQMIWMS_GET_MESSAGE_PROTOCOL_RESP_MSG;

typedef struct _QMIWMS_GET_STORE_MAX_SIZE_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
} QMIWMS_GET_STORE_MAX_SIZE_REQ_MSG, *PQMIWMS_GET_STORE_MAX_SIZE_REQ_MSG;

typedef struct _QMIWMS_GET_STORE_MAX_SIZE_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uint  MemStoreMaxSize;
} QMIWMS_GET_STORE_MAX_SIZE_RESP_MSG, *PQMIWMS_GET_STORE_MAX_SIZE_RESP_MSG;

typedef struct _REQUEST_TAG {
    uchar  TLVType;
    ushort TLVLength;
    uchar  TagType;
} REQUEST_TAG, *PREQUEST_TAG;

typedef struct _QMIWMS_LIST_MESSAGES_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
} QMIWMS_LIST_MESSAGES_REQ_MSG, *PQMIWMS_LIST_MESSAGES_REQ_MSG;

typedef struct _QMIWMS_MESSAGE {
    uint  MessageIndex;
    uchar  TagType;
} QMIWMS_MESSAGE, *PQMIWMS_MESSAGE;

typedef struct _QMIWMS_LIST_MESSAGES_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uint  NumMessages;
} QMIWMS_LIST_MESSAGES_RESP_MSG, *PQMIWMS_LIST_MESSAGES_RESP_MSG;

typedef struct _QMIWMS_RAW_READ_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
    uint  MemoryIndex;
} QMIWMS_RAW_READ_REQ_MSG, *PQMIWMS_RAW_READ_REQ_MSG;

typedef struct _QMIWMS_RAW_READ_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  TagType;
    uchar  Format;
    ushort MessageLength;
    uchar  Message;
} QMIWMS_RAW_READ_RESP_MSG, *PQMIWMS_RAW_READ_RESP_MSG;

typedef struct _QMIWMS_MODIFY_TAG_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
    uint  MemoryIndex;
    uchar  TagType;
} QMIWMS_MODIFY_TAG_REQ_MSG, *PQMIWMS_MODIFY_TAG_REQ_MSG;

typedef struct _QMIWMS_MODIFY_TAG_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWMS_MODIFY_TAG_RESP_MSG, *PQMIWMS_MODIFY_TAG_RESP_MSG;

typedef struct _QMIWMS_RAW_SEND_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  SmsFormat;
    ushort SmsLength;
    uchar  SmsMessage;
} QMIWMS_RAW_SEND_REQ_MSG, *PQMIWMS_RAW_SEND_REQ_MSG;

typedef struct _RAW_SEND_CAUSE_CODE {
    uchar  TLVType;
    ushort TLVLength;
    ushort CauseCode;
} RAW_SEND_CAUSE_CODE, *PRAW_SEND_CAUSE_CODE;


typedef struct _QMIWMS_RAW_SEND_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWMS_RAW_SEND_RESP_MSG, *PQMIWMS_RAW_SEND_RESP_MSG;


typedef struct _WMS_DELETE_MESSAGE_INDEX {
    uchar  TLVType;
    ushort TLVLength;
    uint  MemoryIndex;
} WMS_DELETE_MESSAGE_INDEX, *PWMS_DELETE_MESSAGE_INDEX;

typedef struct _WMS_DELETE_MESSAGE_TAG {
    uchar  TLVType;
    ushort TLVLength;
    uchar  MessageTag;
} WMS_DELETE_MESSAGE_TAG, *PWMS_DELETE_MESSAGE_TAG;

typedef struct _QMIWMS_DELETE_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
} QMIWMS_DELETE_REQ_MSG, *PQMIWMS_DELETE_REQ_MSG;

typedef struct _QMIWMS_DELETE_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWMS_DELETE_RESP_MSG, *PQMIWMS_DELETE_RESP_MSG;


typedef struct _QMIWMS_GET_SMSC_ADDRESS_REQ_MSG {
    ushort Type;
    ushort Length;
} QMIWMS_GET_SMSC_ADDRESS_REQ_MSG, *PQMIWMS_GET_SMSC_ADDRESS_REQ_MSG;

typedef struct _QMIWMS_SMSC_ADDRESS {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SMSCAddressType[3];
    uchar  SMSCAddressLength;
    uchar  SMSCAddressDigits;
} QMIWMS_SMSC_ADDRESS, *PQMIWMS_SMSC_ADDRESS;


typedef struct _QMIWMS_GET_SMSC_ADDRESS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  SMSCAddress;
} QMIWMS_GET_SMSC_ADDRESS_RESP_MSG, *PQMIWMS_GET_SMSC_ADDRESS_RESP_MSG;

typedef struct _QMIWMS_SET_SMSC_ADDRESS_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  SMSCAddress;
} QMIWMS_SET_SMSC_ADDRESS_REQ_MSG, *PQMIWMS_SET_SMSC_ADDRESS_REQ_MSG;

typedef struct _QMIWMS_SET_SMSC_ADDRESS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWMS_SET_SMSC_ADDRESS_RESP_MSG, *PQMIWMS_SET_SMSC_ADDRESS_RESP_MSG;

typedef struct _QMIWMS_SET_EVENT_REPORT_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ReportNewMessage;
} QMIWMS_SET_EVENT_REPORT_REQ_MSG, *PQMIWMS_SET_EVENT_REPORT_REQ_MSG;

typedef struct _QMIWMS_SET_EVENT_REPORT_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMIWMS_SET_EVENT_REPORT_RESP_MSG, *PQMIWMS_SET_EVENT_REPORT_RESP_MSG;

typedef struct _QMIWMS_EVENT_REPORT_IND_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  StorageType;
    uint  StorageIndex;
} QMIWMS_EVENT_REPORT_IND_MSG, *PQMIWMS_EVENT_REPORT_IND_MSG;
#endif

// ======================= End of WMS ==============================


// ======================= NAS ==============================
#define QMINAS_SET_EVENT_REPORT_REQ             0x0002
#define QMINAS_SET_EVENT_REPORT_RESP            0x0002
#define QMINAS_EVENT_REPORT_IND                 0x0002
#define QMINAS_GET_SIGNAL_STRENGTH_REQ          0x0020
#define QMINAS_GET_SIGNAL_STRENGTH_RESP         0x0020
#define QMINAS_PERFORM_NETWORK_SCAN_REQ         0x0021
#define QMINAS_PERFORM_NETWORK_SCAN_RESP        0x0021
#define QMINAS_INITIATE_NW_REGISTER_REQ         0x0022
#define QMINAS_INITIATE_NW_REGISTER_RESP        0x0022
#define QMINAS_INITIATE_ATTACH_REQ              0x0023
#define QMINAS_INITIATE_ATTACH_RESP             0x0023
#define QMINAS_GET_SERVING_SYSTEM_REQ           0x0024
#define QMINAS_GET_SERVING_SYSTEM_RESP          0x0024
#define QMINAS_SERVING_SYSTEM_IND               0x0024
#define QMINAS_GET_HOME_NETWORK_REQ             0x0025
#define QMINAS_GET_HOME_NETWORK_RESP            0x0025
#define QMINAS_GET_PREFERRED_NETWORK_REQ        0x0026
#define QMINAS_GET_PREFERRED_NETWORK_RESP       0x0026
#define QMINAS_SET_PREFERRED_NETWORK_REQ        0x0027
#define QMINAS_SET_PREFERRED_NETWORK_RESP       0x0027
#define QMINAS_GET_FORBIDDEN_NETWORK_REQ        0x0028
#define QMINAS_GET_FORBIDDEN_NETWORK_RESP       0x0028
#define QMINAS_SET_FORBIDDEN_NETWORK_REQ        0x0029
#define QMINAS_SET_FORBIDDEN_NETWORK_RESP       0x0029
#define QMINAS_SET_TECHNOLOGY_PREF_REQ          0x002A
#define QMINAS_SET_TECHNOLOGY_PREF_RESP         0x002A
#define QMINAS_GET_RF_BAND_INFO_REQ             0x0031
#define QMINAS_GET_RF_BAND_INFO_RESP            0x0031
#define QMINAS_GET_PLMN_NAME_REQ                0x0044
#define QMINAS_GET_PLMN_NAME_RESP               0x0044
#define QUECTEL_PACKET_TRANSFER_START_IND 0X100
#define QUECTEL_PACKET_TRANSFER_END_IND 0X101
#define QMINAS_GET_SYS_INFO_REQ                 0x004D
#define QMINAS_GET_SYS_INFO_RESP                0x004D
#define QMINAS_SYS_INFO_IND                     0x004D

typedef struct _QMINAS_GET_HOME_NETWORK_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} __attribute__ ((packed)) QMINAS_GET_HOME_NETWORK_REQ_MSG, *PQMINAS_GET_HOME_NETWORK_REQ_MSG;

typedef struct _HOME_NETWORK_SYSTEMID {
    uchar  TLVType;
    ushort TLVLength;
    ushort SystemID;
    ushort NetworkID;
} __attribute__ ((packed)) HOME_NETWORK_SYSTEMID, *PHOME_NETWORK_SYSTEMID;

typedef struct _HOME_NETWORK {
    uchar  TLVType;
    ushort TLVLength;
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    uchar  NetworkDesclen;
    uchar  NetworkDesc;
} __attribute__ ((packed)) HOME_NETWORK, *PHOME_NETWORK;

#if 0
typedef struct _HOME_NETWORK_EXT {
    uchar  TLVType;
    ushort TLVLength;
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    uchar  NetworkDescDisp;
    uchar  NetworkDescEncoding;
    uchar  NetworkDesclen;
    uchar  NetworkDesc;
} HOME_NETWORK_EXT, *PHOME_NETWORK_EXT;

typedef struct _QMINAS_GET_HOME_NETWORK_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} QMINAS_GET_HOME_NETWORK_RESP_MSG, *PQMINAS_GET_HOME_NETWORK_RESP_MSG;

typedef struct _QMINAS_GET_PREFERRED_NETWORK_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMINAS_GET_PREFERRED_NETWORK_REQ_MSG, *PQMINAS_GET_PREFERRED_NETWORK_REQ_MSG;


typedef struct _PREFERRED_NETWORK {
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    ushort RadioAccess;
} PREFERRED_NETWORK, *PPREFERRED_NETWORK;

typedef struct _QMINAS_GET_PREFERRED_NETWORK_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;         // 0x01 - required parameter
    ushort TLV2Length;       // length of the mfr string
    ushort NumPreferredNetwork;
} QMINAS_GET_PREFERRED_NETWORK_RESP_MSG, *PQMINAS_GET_PREFERRED_NETWORK_RESP_MSG;

typedef struct _QMINAS_GET_FORBIDDEN_NETWORK_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMINAS_GET_FORBIDDEN_NETWORK_REQ_MSG, *PQMINAS_GET_FORBIDDEN_NETWORK_REQ_MSG;

typedef struct _FORBIDDEN_NETWORK {
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
} FORBIDDEN_NETWORK, *PFORBIDDEN_NETWORK;

typedef struct _QMINAS_GET_FORBIDDEN_NETWORK_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;         // 0x01 - required parameter
    ushort TLV2Length;       // length of the mfr string
    ushort NumForbiddenNetwork;
} QMINAS_GET_FORBIDDEN_NETWORK_RESP_MSG, *PQMINAS_GET_FORBIDDEN_NETWORK_RESP_MSG;

typedef struct _QMINAS_GET_SERVING_SYSTEM_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMINAS_GET_SERVING_SYSTEM_REQ_MSG, *PQMINAS_GET_SERVING_SYSTEM_REQ_MSG;

typedef struct _QMINAS_ROAMING_INDICATOR_MSG {
    uchar  TLVType;         // 0x01 - required parameter
    ushort TLVLength;       // length of the mfr string
    uchar  RoamingIndicator;
} QMINAS_ROAMING_INDICATOR_MSG, *PQMINAS_ROAMING_INDICATOR_MSG;
#endif

typedef struct _QMINAS_DATA_CAP {
    uchar  TLVType;         // 0x01 - required parameter
    ushort TLVLength;       // length of the mfr string
    uchar  DataCapListLen;
    uchar  DataCap;
} __attribute__ ((packed)) QMINAS_DATA_CAP, *PQMINAS_DATA_CAP;

typedef struct _QMINAS_CURRENT_PLMN_MSG {
    uchar  TLVType;         // 0x01 - required parameter
    ushort TLVLength;       // length of the mfr string
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    uchar  NetworkDesclen;
    uchar  NetworkDesc;
} __attribute__ ((packed)) QMINAS_CURRENT_PLMN_MSG, *PQMINAS_CURRENT_PLMN_MSG;

typedef struct _QMINAS_GET_SERVING_SYSTEM_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMINAS_GET_SERVING_SYSTEM_RESP_MSG, *PQMINAS_GET_SERVING_SYSTEM_RESP_MSG;

typedef struct _SERVING_SYSTEM {
    uchar  TLVType;
    ushort TLVLength;
    uchar  RegistrationState;
    uchar  CSAttachedState;
    uchar  PSAttachedState;
    uchar  RegistredNetwork;
    uchar  InUseRadioIF;
    uchar  RadioIF;
} __attribute__ ((packed)) SERVING_SYSTEM, *PSERVING_SYSTEM;

typedef struct _QMINAS_GET_SYS_INFO_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMINAS_GET_SYS_INFO_RESP_MSG, *PQMINAS_GET_SYS_INFO_RESP_MSG;

typedef struct _QMINAS_SYS_INFO_IND_MSG {
    ushort Type;
    ushort Length;
} __attribute__ ((packed)) QMINAS_SYS_INFO_IND_MSG, *PQMINAS_SYS_INFO_IND_MSG;

typedef struct _SERVICE_STATUS_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvStatus;
    uchar  IsPrefDataPath;
} __attribute__ ((packed)) SERVICE_STATUS_INFO, *PSERVICE_STATUS_INFO;

typedef struct _CDMA_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  IsSysPrlMatchValid;
    uchar  IsSysPrlMatch;
    uchar  PRevInUseValid;
    uchar  PRevInUse;
    uchar  BSPRevValid;
    uchar  BSPRev;
    uchar  CCSSupportedValid;
    uchar  CCSSupported;
    uchar  CDMASysIdValid;
    ushort SID;
    ushort NID;
    uchar  BSInfoValid;
    ushort BaseID;
    uint  BaseLAT;
    uint  Baselong;
    uchar  PacketZoneValid;
    ushort PacketZone;
    uchar  NetworkIdValid;
    uchar  MCC[3];
    uchar  MNC[3];
} __attribute__ ((packed)) CDMA_SYSTEM_INFO, *PCDMA_SYSTEM_INFO;

typedef struct _HDR_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  IsSysPrlMatchValid;
    uchar  IsSysPrlMatch;
    uchar  HdrPersonalityValid;
    uchar  HdrPersonality;
    uchar  HdrActiveProtValid;
    uchar  HdrActiveProt;
    uchar  is856SysIdValid;
    uchar  is856SysId[16];
} __attribute__ ((packed)) HDR_SYSTEM_INFO, *PHDR_SYSTEM_INFO;

typedef struct _GSM_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  LacValid;
    ushort Lac;
    uchar  CellIdValid;
    uint  CellId;
    uchar  RegRejectInfoValid;
    uchar  RejectSrvDomain;
    uchar  RejCause;
    uchar  NetworkIdValid;
    uchar  MCC[3];
    uchar  MNC[3];
    uchar  EgprsSuppValid;
    uchar  EgprsSupp;
    uchar  DtmSuppValid;
    uchar  DtmSupp;
} __attribute__ ((packed)) GSM_SYSTEM_INFO, *PGSM_SYSTEM_INFO;

typedef struct _WCDMA_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  LacValid;
    ushort Lac;
    uchar  CellIdValid;
    uint  CellId;
    uchar  RegRejectInfoValid;
    uchar  RejectSrvDomain;
    uchar  RejCause;
    uchar  NetworkIdValid;
    uchar  MCC[3];
    uchar  MNC[3];
    uchar  HsCallStatusValid;
    uchar  HsCallStatus;
    uchar  HsIndValid;
    uchar  HsInd;
    uchar  PscValid;
    uchar  Psc;
} __attribute__ ((packed)) WCDMA_SYSTEM_INFO, *PWCDMA_SYSTEM_INFO;

typedef struct _LTE_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  LacValid;
    ushort Lac;
    uchar  CellIdValid;
    uint  CellId;
    uchar  RegRejectInfoValid;
    uchar  RejectSrvDomain;
    uchar  RejCause;
    uchar  NetworkIdValid;
    uchar  MCC[3];
    uchar  MNC[3];
    uchar  TacValid;
    ushort Tac;
} __attribute__ ((packed)) LTE_SYSTEM_INFO, *PLTE_SYSTEM_INFO;

typedef struct _TDSCDMA_SYSTEM_INFO {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SrvDomainValid;
    uchar  SrvDomain;
    uchar  SrvCapabilityValid;
    uchar  SrvCapability;
    uchar  RoamStatusValid;
    uchar  RoamStatus;
    uchar  IsSysForbiddenValid;
    uchar  IsSysForbidden;
    uchar  LacValid;
    ushort Lac;
    uchar  CellIdValid;
    uint  CellId;
    uchar  RegRejectInfoValid;
    uchar  RejectSrvDomain;
    uchar  RejCause;
    uchar  NetworkIdValid;
    uchar  MCC[3];
    uchar  MNC[3];
    uchar  HsCallStatusValid;
    uchar  HsCallStatus;
    uchar  HsIndValid;
    uchar  HsInd;
    uchar  CellParameterIdValid;
    ushort CellParameterId;
    uchar  CellBroadcastCapValid;
    uint  CellBroadcastCap;
    uchar  CsBarStatusValid;
    uint  CsBarStatus;
    uchar  PsBarStatusValid;
    uint  PsBarStatus;
    uchar  CipherDomainValid;
    uchar  CipherDomain;
} __attribute__ ((packed)) TDSCDMA_SYSTEM_INFO, *PTDSCDMA_SYSTEM_INFO;

#if 0
typedef struct _QMINAS_SERVING_SYSTEM_IND_MSG {
    ushort Type;
    ushort Length;
} QMINAS_SERVING_SYSTEM_IND_MSG, *PQMINAS_SERVING_SYSTEM_IND_MSG;

typedef struct _QMINAS_SET_PREFERRED_NETWORK_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort NumPreferredNetwork;
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    ushort RadioAccess;
} QMINAS_SET_PREFERRED_NETWORK_REQ_MSG, *PQMINAS_SET_PREFERRED_NETWORK_REQ_MSG;

typedef struct _QMINAS_SET_PREFERRED_NETWORK_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_SET_PREFERRED_NETWORK_RESP_MSG, *PQMINAS_SET_PREFERRED_NETWORK_RESP_MSG;

typedef struct _QMINAS_SET_FORBIDDEN_NETWORK_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort NumForbiddenNetwork;
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
} QMINAS_SET_FORBIDDEN_NETWORK_REQ_MSG, *PQMINAS_SET_FORBIDDEN_NETWORK_REQ_MSG;

typedef struct _QMINAS_SET_FORBIDDEN_NETWORK_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_SET_FORBIDDEN_NETWORK_RESP_MSG, *PQMINAS_SET_FORBIDDEN_NETWORK_RESP_MSG;

typedef struct _QMINAS_PERFORM_NETWORK_SCAN_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMINAS_PERFORM_NETWORK_SCAN_REQ_MSG, *PQMINAS_PERFORM_NETWORK_SCAN_REQ_MSG;

typedef struct _VISIBLE_NETWORK {
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    uchar  NetworkStatus;
    uchar  NetworkDesclen;
} VISIBLE_NETWORK, *PVISIBLE_NETWORK;

typedef struct _QMINAS_PERFORM_NETWORK_SCAN_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_PERFORM_NETWORK_SCAN_RESP_MSG, *PQMINAS_PERFORM_NETWORK_SCAN_RESP_MSG;

typedef struct _QMINAS_PERFORM_NETWORK_SCAN_NETWORK_INFO {
    uchar  TLVType;         // 0x010 - required parameter
    ushort TLVLength;       // length
    ushort NumNetworkInstances;
} QMINAS_PERFORM_NETWORK_SCAN_NETWORK_INFO, *PQMINAS_PERFORM_NETWORK_SCAN_NETWORK_INFO;

typedef struct _QMINAS_PERFORM_NETWORK_SCAN_RAT_INFO {
    uchar  TLVType;         // 0x011 - required parameter
    ushort TLVLength;       // length
    ushort NumInst;
} QMINAS_PERFORM_NETWORK_SCAN_RAT_INFO, *PQMINAS_PERFORM_NETWORK_SCAN_RAT_INFO;

typedef struct _QMINAS_PERFORM_NETWORK_SCAN_RAT {
    ushort MCC;
    ushort MNC;
    uchar  RAT;
} QMINAS_PERFORM_NETWORK_SCAN_RAT, *PQMINAS_PERFORM_NETWORK_SCAN_RAT;


typedef struct _QMINAS_MANUAL_NW_REGISTER {
    uchar  TLV2Type;          // 0x02 - result code
    ushort TLV2Length;        // 4
    ushort MobileCountryCode;
    ushort MobileNetworkCode;
    uchar  RadioAccess;
} QMINAS_MANUAL_NW_REGISTER, *PQMINAS_MANUAL_NW_REGISTER;

typedef struct _QMINAS_INITIATE_NW_REGISTER_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    uchar  RegisterAction;
} QMINAS_INITIATE_NW_REGISTER_REQ_MSG, *PQMINAS_INITIATE_NW_REGISTER_REQ_MSG;

typedef struct _QMINAS_INITIATE_NW_REGISTER_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_INITIATE_NW_REGISTER_RESP_MSG, *PQMINAS_INITIATE_NW_REGISTER_RESP_MSG;

typedef struct _QMINAS_SET_TECHNOLOGY_PREF_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort TechPref;
    uchar  Duration;
} QMINAS_SET_TECHNOLOGY_PREF_REQ_MSG, *PQMINAS_SET_TECHNOLOGY_PREF_REQ_MSG;

typedef struct _QMINAS_SET_TECHNOLOGY_PREF_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_SET_TECHNOLOGY_PREF_RESP_MSG, *PQMINAS_SET_TECHNOLOGY_PREF_RESP_MSG;

typedef struct _QMINAS_GET_SIGNAL_STRENGTH_REQ_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
} QMINAS_GET_SIGNAL_STRENGTH_REQ_MSG, *PQMINAS_GET_SIGNAL_STRENGTH_REQ_MSG;

typedef struct _QMINAS_SIGNAL_STRENGTH {
    char   SigStrength;
    uchar  RadioIf;
} QMINAS_SIGNAL_STRENGTH, *PQMINAS_SIGNAL_STRENGTH;

typedef struct _QMINAS_SIGNAL_STRENGTH_LIST {
    uchar  TLV3Type;
    ushort TLV3Length;
    ushort NumInstance;
} QMINAS_SIGNAL_STRENGTH_LIST, *PQMINAS_SIGNAL_STRENGTH_LIST;


typedef struct _QMINAS_GET_SIGNAL_STRENGTH_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    char   SignalStrength;
    uchar  RadioIf;
} QMINAS_GET_SIGNAL_STRENGTH_RESP_MSG, *PQMINAS_GET_SIGNAL_STRENGTH_RESP_MSG;


typedef struct _QMINAS_SET_EVENT_REPORT_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  ReportSigStrength;
    uchar  NumTresholds;
    char   TresholdList[2];
} QMINAS_SET_EVENT_REPORT_REQ_MSG, *PQMINAS_SET_EVENT_REPORT_REQ_MSG;

typedef struct _QMINAS_SET_EVENT_REPORT_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_SET_EVENT_REPORT_RESP_MSG, *PQMINAS_SET_EVENT_REPORT_RESP_MSG;

typedef struct _QMINAS_SIGNAL_STRENGTH_TLV {
    uchar  TLVType;
    ushort TLVLength;
    char   SigStrength;
    uchar  RadioIf;
} QMINAS_SIGNAL_STRENGTH_TLV, *PQMINAS_SIGNAL_STRENGTH_TLV;

typedef struct _QMINAS_REJECT_CAUSE_TLV {
    uchar  TLVType;
    ushort TLVLength;
    uchar  ServiceDomain;
    ushort RejectCause;
} QMINAS_REJECT_CAUSE_TLV, *PQMINAS_REJECT_CAUSE_TLV;

typedef struct _QMINAS_EVENT_REPORT_IND_MSG {
    ushort Type;
    ushort Length;
} QMINAS_EVENT_REPORT_IND_MSG, *PQMINAS_EVENT_REPORT_IND_MSG;

typedef struct _QMINAS_GET_RF_BAND_INFO_REQ_MSG {
    ushort Type;
    ushort Length;
} QMINAS_GET_RF_BAND_INFO_REQ_MSG, *PQMINAS_GET_RF_BAND_INFO_REQ_MSG;

typedef struct _QMINASRF_BAND_INFO {
    uchar  RadioIf;
    ushort ActiveBand;
    ushort ActiveChannel;
} QMINASRF_BAND_INFO, *PQMINASRF_BAND_INFO;

typedef struct _QMINAS_GET_RF_BAND_INFO_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  NumInstances;
} QMINAS_GET_RF_BAND_INFO_RESP_MSG, *PQMINAS_GET_RF_BAND_INFO_RESP_MSG;


typedef struct _QMINAS_GET_PLMN_NAME_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort MCC;
    ushort MNC;
} QMINAS_GET_PLMN_NAME_REQ_MSG, *PQMINAS_GET_PLMN_NAME_REQ_MSG;

typedef struct _QMINAS_GET_PLMN_NAME_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;       // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;        // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_GET_PLMN_NAME_RESP_MSG, *PQMINAS_GET_PLMN_NAME_RESP_MSG;

typedef struct _QMINAS_GET_PLMN_NAME_SPN {
    uchar  TLVType;
    ushort TLVLength;
    uchar  SPN_Enc;
    uchar  SPN_Len;
} QMINAS_GET_PLMN_NAME_SPN, *PQMINAS_GET_PLMN_NAME_SPN;

typedef struct _QMINAS_GET_PLMN_NAME_PLMN {
    uchar  PLMN_Enc;
    uchar  PLMN_Ci;
    uchar  PLMN_SpareBits;
    uchar  PLMN_Len;
} QMINAS_GET_PLMN_NAME_PLMN, *PQMINAS_GET_PLMN_NAME_PLMN;

typedef struct _QMINAS_INITIATE_ATTACH_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  PsAttachAction;
} QMINAS_INITIATE_ATTACH_REQ_MSG, *PQMINAS_INITIATE_ATTACH_REQ_MSG;

typedef struct _QMINAS_INITIATE_ATTACH_RESP_MSG {
    ushort Type;             // QMUX type 0x0003
    ushort Length;
    uchar  TLVType;          // 0x02 - result code
    ushort TLVLength;        // 4
    ushort QMUXResult;      // QMI_RESULT_SUCCESS
    // QMI_RESULT_FAILURE
    ushort QMUXError;       // QMI_ERR_INVALID_ARG
    // QMI_ERR_NO_MEMORY
    // QMI_ERR_intERNAL
    // QMI_ERR_FAULT
} QMINAS_INITIATE_ATTACH_RESP_MSG, *PQMINAS_INITIATE_ATTACH_RESP_MSG;
#endif
// ======================= End of NAS ==============================

// ======================= UIM ==============================
#define QMIUIM_READ_TRANSPARENT_REQ      0x0020
#define QMIUIM_READ_TRANSPARENT_RESP     0x0020
#define QMIUIM_READ_TRANSPARENT_IND      0x0020
#define QMIUIM_READ_RECORD_REQ           0x0021
#define QMIUIM_READ_RECORD_RESP          0x0021
#define QMIUIM_READ_RECORD_IND           0x0021
#define QMIUIM_WRITE_TRANSPARENT_REQ     0x0022
#define QMIUIM_WRITE_TRANSPARENT_RESP    0x0022
#define QMIUIM_WRITE_TRANSPARENT_IND     0x0022
#define QMIUIM_WRITE_RECORD_REQ          0x0023
#define QMIUIM_WRITE_RECORD_RESP         0x0023
#define QMIUIM_WRITE_RECORD_IND          0x0023
#define QMIUIM_SET_PIN_PROTECTION_REQ    0x0025
#define QMIUIM_SET_PIN_PROTECTION_RESP   0x0025
#define QMIUIM_SET_PIN_PROTECTION_IND    0x0025
#define QMIUIM_VERIFY_PIN_REQ            0x0026
#define QMIUIM_VERIFY_PIN_RESP           0x0026
#define QMIUIM_VERIFY_PIN_IND            0x0026
#define QMIUIM_UNBLOCK_PIN_REQ           0x0027
#define QMIUIM_UNBLOCK_PIN_RESP          0x0027
#define QMIUIM_UNBLOCK_PIN_IND           0x0027
#define QMIUIM_CHANGE_PIN_REQ            0x0028
#define QMIUIM_CHANGE_PIN_RESP           0x0028
#define QMIUIM_CHANGE_PIN_IND            0x0028
#define QMIUIM_DEPERSONALIZATION_REQ     0x0029
#define QMIUIM_DEPERSONALIZATION_RESP    0x0029
#define QMIUIM_EVENT_REG_REQ             0x002E
#define QMIUIM_EVENT_REG_RESP            0x002E
#define QMIUIM_GET_CARD_STATUS_REQ       0x002F
#define QMIUIM_GET_CARD_STATUS_RESP      0x002F
#define QMIUIM_STATUS_CHANGE_IND         0x0032


typedef struct _QMIUIM_GET_CARD_STATUS_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMIUIM_GET_CARD_STATUS_RESP_MSG, *PQMIUIM_GET_CARD_STATUS_RESP_MSG;

typedef struct _QMIUIM_CARD_STATUS {
    uchar  TLVType;
    ushort TLVLength;
    ushort IndexGWPri;
    ushort Index1XPri;
    ushort IndexGWSec;
    ushort Index1XSec;
    uchar  NumSlot;
    uchar  CardState;
    uchar  UPINState;
    uchar  UPINRetries;
    uchar  UPUKRetries;
    uchar  ErrorCode;
    uchar  NumApp;
    uchar  AppType;
    uchar  AppState;
    uchar  PersoState;
    uchar  PersoFeature;
    uchar  PersoRetries;
    uchar  PersoUnblockRetries;
    uchar  AIDLength;
} __attribute__ ((packed)) QMIUIM_CARD_STATUS, *PQMIUIM_CARD_STATUS;

typedef struct _QMIUIM_PIN_STATE {
    uchar  UnivPIN;
    uchar  PIN1State;
    uchar  PIN1Retries;
    uchar  PUK1Retries;
    uchar  PIN2State;
    uchar  PIN2Retries;
    uchar  PUK2Retries;
} __attribute__ ((packed)) QMIUIM_PIN_STATE, *PQMIUIM_PIN_STATE;

typedef struct _QMIUIM_VERIFY_PIN_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  Session_Type;
    uchar  Aid_Len;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINID;
    uchar  PINLen;
    uchar  PINValue;
} __attribute__ ((packed)) QMIUIM_VERIFY_PIN_REQ_MSG, *PQMIUIM_VERIFY_PIN_REQ_MSG;

typedef struct _QMIUIM_VERIFY_PIN_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
    uchar  TLV2Type;
    ushort TLV2Length;
    uchar  PINVerifyRetriesLeft;
    uchar  PINUnblockRetriesLeft;
} __attribute__ ((packed)) QMIUIM_VERIFY_PIN_RESP_MSG, *PQMIUIM_VERIFY_PIN_RESP_MSG;

typedef struct _QMIUIM_READ_TRANSPARENT_REQ_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    uchar  Session_Type;
    uchar  Aid_Len;
    uchar  TLV2Type;
    ushort TLV2Length;
    ushort file_id;
    uchar  path_len;
    uchar  path[];
} __attribute__ ((packed)) QMIUIM_READ_TRANSPARENT_REQ_MSG, *PQMIUIM_READ_TRANSPARENT_REQ_MSG;

typedef struct _READ_TRANSPARENT_TLV {
    uchar  TLVType;
    ushort TLVLength;
    ushort Offset;
    ushort Length;
} __attribute__ ((packed)) READ_TRANSPARENT_TLV, *PREAD_TRANSPARENT_TLV;

typedef struct _QMIUIM_CONTENT {
    uchar  TLVType;
    ushort TLVLength;
    ushort content_len;
    uchar  content[];
} __attribute__ ((packed)) QMIUIM_CONTENT, *PQMIUIM_CONTENT;

typedef struct _QMIUIM_READ_TRANSPARENT_RESP_MSG {
    ushort Type;
    ushort Length;
    uchar  TLVType;
    ushort TLVLength;
    ushort QMUXResult;
    ushort QMUXError;
} __attribute__ ((packed)) QMIUIM_READ_TRANSPARENT_RESP_MSG, *PQMIUIM_READ_TRANSPARENT_RESP_MSG;

typedef struct _QMUX_MSG {
    QCQMUX_HDR QMUXHdr;
    union {
        // Message Header
        QCQMUX_MSG_HDR                           QMUXMsgHdr;
        QCQMUX_MSG_HDR_RESP                      QMUXMsgHdrResp;

        // QMIWDS Message
#if 0
        QMIWDS_GET_PKT_SRVC_STATUS_REQ_MSG        PacketServiceStatusReq;
        QMIWDS_GET_PKT_SRVC_STATUS_RESP_MSG       PacketServiceStatusRsp;
        QMIWDS_GET_PKT_SRVC_STATUS_IND_MSG        PacketServiceStatusInd;
        QMIWDS_EVENT_REPORT_IND_MSG               EventReportInd;
        QMIWDS_GET_CURRENT_CHANNEL_RATE_REQ_MSG   GetCurrChannelRateReq;
        QMIWDS_GET_CURRENT_CHANNEL_RATE_RESP_MSG  GetCurrChannelRateRsp;
        QMIWDS_GET_PKT_STATISTICS_REQ_MSG         GetPktStatsReq;
        QMIWDS_GET_PKT_STATISTICS_RESP_MSG        GetPktStatsRsp;
        QMIWDS_SET_EVENT_REPORT_REQ_MSG           EventReportReq;
        QMIWDS_SET_EVENT_REPORT_RESP_MSG          EventReportRsp;
#endif
        //#ifdef QC_IP_MODE
        QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG       GetRuntimeSettingsReq;
        QMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG      GetRuntimeSettingsRsp;
        //#endif // QC_IP_MODE
        QMIWDS_SET_CLIENT_IP_FAMILY_PREF_REQ_MSG  SetClientIpFamilyPrefReq;
        QMIWDS_SET_CLIENT_IP_FAMILY_PREF_RESP_MSG SetClientIpFamilyPrefResp;
        QMIWDS_SET_AUTO_CONNECT_REQ_MSG SetAutoConnectReq;
#if 0
        QMIWDS_GET_MIP_MODE_REQ_MSG               GetMipModeReq;
        QMIWDS_GET_MIP_MODE_RESP_MSG              GetMipModeResp;
#endif
        QMIWDS_START_NETWORK_intERFACE_REQ_MSG    StartNwInterfaceReq;
        QMIWDS_START_NETWORK_intERFACE_RESP_MSG   StartNwInterfaceResp;
        QMIWDS_STOP_NETWORK_intERFACE_REQ_MSG     StopNwInterfaceReq;
        QMIWDS_STOP_NETWORK_intERFACE_RESP_MSG    StopNwInterfaceResp;
        QMIWDS_GET_DEFAULT_SETTINGS_REQ_MSG       GetDefaultSettingsReq;
        QMIWDS_GET_DEFAULT_SETTINGS_RESP_MSG      GetDefaultSettingsResp;
        QMIWDS_MODIFY_PROFILE_SETTINGS_REQ_MSG    ModifyProfileSettingsReq;
        QMIWDS_MODIFY_PROFILE_SETTINGS_RESP_MSG   ModifyProfileSettingsResp;
        QMIWDS_GET_PROFILE_SETTINGS_REQ_MSG    GetProfileSettingsReq;
#if 0
        QMIWDS_GET_DATA_BEARER_REQ_MSG            GetDataBearerReq;
        QMIWDS_GET_DATA_BEARER_RESP_MSG           GetDataBearerResp;
        QMIWDS_DUN_CALL_INFO_REQ_MSG              DunCallInfoReq;
        QMIWDS_DUN_CALL_INFO_RESP_MSG             DunCallInfoResp;
#endif
        QMIWDS_BIND_MUX_DATA_PORT_REQ_MSG         BindMuxDataPortReq;

        // QMIDMS Messages
#if 0
        QMIDMS_GET_DEVICE_MFR_REQ_MSG             GetDeviceMfrReq;
        QMIDMS_GET_DEVICE_MFR_RESP_MSG            GetDeviceMfrRsp;
        QMIDMS_GET_DEVICE_MODEL_ID_REQ_MSG        GetDeviceModeIdReq;
        QMIDMS_GET_DEVICE_MODEL_ID_RESP_MSG       GetDeviceModeIdRsp;
        QMIDMS_GET_DEVICE_REV_ID_REQ_MSG          GetDeviceRevIdReq;
        QMIDMS_GET_DEVICE_REV_ID_RESP_MSG         GetDeviceRevIdRsp;
        QMIDMS_GET_MSISDN_REQ_MSG                 GetMsisdnReq;
        QMIDMS_GET_MSISDN_RESP_MSG                GetMsisdnRsp;
        QMIDMS_GET_DEVICE_SERIAL_NUMBERS_REQ_MSG  GetDeviceSerialNumReq;
        QMIDMS_GET_DEVICE_SERIAL_NUMBERS_RESP_MSG GetDeviceSerialNumRsp;
        QMIDMS_GET_DEVICE_CAP_REQ_MSG             GetDeviceCapReq;
        QMIDMS_GET_DEVICE_CAP_RESP_MSG            GetDeviceCapResp;
        QMIDMS_GET_BAND_CAP_REQ_MSG               GetBandCapReq;
        QMIDMS_GET_BAND_CAP_RESP_MSG              GetBandCapRsp;
        QMIDMS_GET_ACTIVATED_STATUS_REQ_MSG       GetActivatedStatusReq;
        QMIDMS_GET_ACTIVATED_STATUS_RESP_MSG      GetActivatedStatusResp;
        QMIDMS_GET_OPERATING_MODE_REQ_MSG         GetOperatingModeReq;
        QMIDMS_GET_OPERATING_MODE_RESP_MSG        GetOperatingModeResp;
#endif
        QMIDMS_SET_OPERATING_MODE_REQ_MSG         SetOperatingModeReq;
        QMIDMS_SET_OPERATING_MODE_RESP_MSG        SetOperatingModeResp;
#if 0
        QMIDMS_UIM_GET_ICCID_REQ_MSG              GetICCIDReq;
        QMIDMS_UIM_GET_ICCID_RESP_MSG             GetICCIDResp;
        QMIDMS_ACTIVATE_AUTOMATIC_REQ_MSG         ActivateAutomaticReq;
        QMIDMS_ACTIVATE_AUTOMATIC_RESP_MSG        ActivateAutomaticResp;
        QMIDMS_ACTIVATE_MANUAL_REQ_MSG            ActivateManualReq;
        QMIDMS_ACTIVATE_MANUAL_RESP_MSG           ActivateManualResp;
#endif
        QMIDMS_UIM_GET_PIN_STATUS_REQ_MSG         UIMGetPinStatusReq;
        QMIDMS_UIM_GET_PIN_STATUS_RESP_MSG        UIMGetPinStatusResp;
        QMIDMS_UIM_VERIFY_PIN_REQ_MSG             UIMVerifyPinReq;
        QMIDMS_UIM_VERIFY_PIN_RESP_MSG            UIMVerifyPinResp;
#if 0
        QMIDMS_UIM_SET_PIN_PROTECTION_REQ_MSG     UIMSetPinProtectionReq;
        QMIDMS_UIM_SET_PIN_PROTECTION_RESP_MSG    UIMSetPinProtectionResp;
        QMIDMS_UIM_CHANGE_PIN_REQ_MSG             UIMChangePinReq;
        QMIDMS_UIM_CHANGE_PIN_RESP_MSG            UIMChangePinResp;
        QMIDMS_UIM_UNBLOCK_PIN_REQ_MSG            UIMUnblockPinReq;
        QMIDMS_UIM_UNBLOCK_PIN_RESP_MSG           UIMUnblockPinResp;
        QMIDMS_SET_EVENT_REPORT_REQ_MSG           DmsSetEventReportReq;
        QMIDMS_SET_EVENT_REPORT_RESP_MSG          DmsSetEventReportResp;
        QMIDMS_EVENT_REPORT_IND_MSG               DmsEventReportInd;
#endif
        QMIDMS_UIM_GET_STATE_REQ_MSG              UIMGetStateReq;
        QMIDMS_UIM_GET_STATE_RESP_MSG             UIMGetStateResp;
        QMIDMS_UIM_GET_IMSI_REQ_MSG               UIMGetIMSIReq;
        QMIDMS_UIM_GET_IMSI_RESP_MSG              UIMGetIMSIResp;
#if 0
        QMIDMS_UIM_GET_CK_STATUS_REQ_MSG          UIMGetCkStatusReq;
        QMIDMS_UIM_GET_CK_STATUS_RESP_MSG         UIMGetCkStatusResp;
        QMIDMS_UIM_SET_CK_PROTECTION_REQ_MSG      UIMSetCkProtectionReq;
        QMIDMS_UIM_SET_CK_PROTECTION_RESP_MSG     UIMSetCkProtectionResp;
        QMIDMS_UIM_UNBLOCK_CK_REQ_MSG             UIMUnblockCkReq;
        QMIDMS_UIM_UNBLOCK_CK_RESP_MSG            UIMUnblockCkResp;
#endif

        // QMIQOS Messages
#if 0
        QMI_QOS_SET_EVENT_REPORT_REQ_MSG          QosSetEventReportReq;
        QMI_QOS_SET_EVENT_REPORT_RESP_MSG         QosSetEventReportRsp;
        QMI_QOS_EVENT_REPORT_IND_MSG              QosEventReportInd;
#endif

        // QMIWMS Messages
#if 0
        QMIWMS_GET_MESSAGE_PROTOCOL_REQ_MSG       GetMessageProtocolReq;
        QMIWMS_GET_MESSAGE_PROTOCOL_RESP_MSG      GetMessageProtocolResp;
        QMIWMS_GET_SMSC_ADDRESS_REQ_MSG           GetSMSCAddressReq;
        QMIWMS_GET_SMSC_ADDRESS_RESP_MSG          GetSMSCAddressResp;
        QMIWMS_SET_SMSC_ADDRESS_REQ_MSG           SetSMSCAddressReq;
        QMIWMS_SET_SMSC_ADDRESS_RESP_MSG          SetSMSCAddressResp;
        QMIWMS_GET_STORE_MAX_SIZE_REQ_MSG         GetStoreMaxSizeReq;
        QMIWMS_GET_STORE_MAX_SIZE_RESP_MSG        GetStoreMaxSizeResp;
        QMIWMS_LIST_MESSAGES_REQ_MSG              ListMessagesReq;
        QMIWMS_LIST_MESSAGES_RESP_MSG             ListMessagesResp;
        QMIWMS_RAW_READ_REQ_MSG                   RawReadMessagesReq;
        QMIWMS_RAW_READ_RESP_MSG                  RawReadMessagesResp;
        QMIWMS_SET_EVENT_REPORT_REQ_MSG           WmsSetEventReportReq;
        QMIWMS_SET_EVENT_REPORT_RESP_MSG          WmsSetEventReportResp;
        QMIWMS_EVENT_REPORT_IND_MSG               WmsEventReportInd;
        QMIWMS_DELETE_REQ_MSG                     WmsDeleteReq;
        QMIWMS_DELETE_RESP_MSG                    WmsDeleteResp;
        QMIWMS_RAW_SEND_REQ_MSG                   RawSendMessagesReq;
        QMIWMS_RAW_SEND_RESP_MSG                  RawSendMessagesResp;
        QMIWMS_MODIFY_TAG_REQ_MSG                 WmsModifyTagReq;
        QMIWMS_MODIFY_TAG_RESP_MSG                WmsModifyTagResp;
#endif

        // QMINAS Messages
#if 0
        QMINAS_GET_HOME_NETWORK_REQ_MSG           GetHomeNetworkReq;
        QMINAS_GET_HOME_NETWORK_RESP_MSG          GetHomeNetworkResp;
        QMINAS_GET_PREFERRED_NETWORK_REQ_MSG      GetPreferredNetworkReq;
        QMINAS_GET_PREFERRED_NETWORK_RESP_MSG     GetPreferredNetworkResp;
        QMINAS_GET_FORBIDDEN_NETWORK_REQ_MSG      GetForbiddenNetworkReq;
        QMINAS_GET_FORBIDDEN_NETWORK_RESP_MSG     GetForbiddenNetworkResp;
        QMINAS_GET_SERVING_SYSTEM_REQ_MSG         GetServingSystemReq;
#endif
        QMINAS_GET_SERVING_SYSTEM_RESP_MSG        GetServingSystemResp;
        QMINAS_GET_SYS_INFO_RESP_MSG              GetSysInfoResp;
        QMINAS_SYS_INFO_IND_MSG                   NasSysInfoInd;
#if 0
        QMINAS_SERVING_SYSTEM_IND_MSG             NasServingSystemInd;
        QMINAS_SET_PREFERRED_NETWORK_REQ_MSG      SetPreferredNetworkReq;
        QMINAS_SET_PREFERRED_NETWORK_RESP_MSG     SetPreferredNetworkResp;
        QMINAS_SET_FORBIDDEN_NETWORK_REQ_MSG      SetForbiddenNetworkReq;
        QMINAS_SET_FORBIDDEN_NETWORK_RESP_MSG     SetForbiddenNetworkResp;
        QMINAS_PERFORM_NETWORK_SCAN_REQ_MSG       PerformNetworkScanReq;
        QMINAS_PERFORM_NETWORK_SCAN_RESP_MSG      PerformNetworkScanResp;
        QMINAS_INITIATE_NW_REGISTER_REQ_MSG       InitiateNwRegisterReq;
        QMINAS_INITIATE_NW_REGISTER_RESP_MSG      InitiateNwRegisterResp;
        QMINAS_SET_TECHNOLOGY_PREF_REQ_MSG        SetTechnologyPrefReq;
        QMINAS_SET_TECHNOLOGY_PREF_RESP_MSG       SetTechnologyPrefResp;
        QMINAS_GET_SIGNAL_STRENGTH_REQ_MSG        GetSignalStrengthReq;
        QMINAS_GET_SIGNAL_STRENGTH_RESP_MSG       GetSignalStrengthResp;
        QMINAS_SET_EVENT_REPORT_REQ_MSG           SetEventReportReq;
        QMINAS_SET_EVENT_REPORT_RESP_MSG          SetEventReportResp;
        QMINAS_EVENT_REPORT_IND_MSG               NasEventReportInd;
        QMINAS_GET_RF_BAND_INFO_REQ_MSG           GetRFBandInfoReq;
        QMINAS_GET_RF_BAND_INFO_RESP_MSG          GetRFBandInfoResp;
        QMINAS_INITIATE_ATTACH_REQ_MSG            InitiateAttachReq;
        QMINAS_INITIATE_ATTACH_RESP_MSG           InitiateAttachResp;
        QMINAS_GET_PLMN_NAME_REQ_MSG              GetPLMNNameReq;
        QMINAS_GET_PLMN_NAME_RESP_MSG             GetPLMNNameResp;
#endif

        // QMIUIM Messages
        QMIUIM_GET_CARD_STATUS_RESP_MSG           UIMGetCardStatus;
        QMIUIM_VERIFY_PIN_REQ_MSG                 UIMUIMVerifyPinReq;
        QMIUIM_VERIFY_PIN_RESP_MSG                UIMUIMVerifyPinResp;
#if 0
        QMIUIM_SET_PIN_PROTECTION_REQ_MSG         UIMUIMSetPinProtectionReq;
        QMIUIM_SET_PIN_PROTECTION_RESP_MSG        UIMUIMSetPinProtectionResp;
        QMIUIM_CHANGE_PIN_REQ_MSG                 UIMUIMChangePinReq;
        QMIUIM_CHANGE_PIN_RESP_MSG                UIMUIMChangePinResp;
        QMIUIM_UNBLOCK_PIN_REQ_MSG                UIMUIMUnblockPinReq;
        QMIUIM_UNBLOCK_PIN_RESP_MSG               UIMUIMUnblockPinResp;
#endif
        QMIUIM_READ_TRANSPARENT_REQ_MSG           UIMUIMReadTransparentReq;
        QMIUIM_READ_TRANSPARENT_RESP_MSG          UIMUIMReadTransparentResp;

        QMIWDS_ADMIN_SET_DATA_FORMAT_REQ_MSG      SetDataFormatReq;

    };
} __attribute__ ((packed)) QMUX_MSG, *PQMUX_MSG;

#pragma pack(pop)

#endif // MPQMUX_H
