//
// Created by zheng zhao on 2023/4/14.
//

#define PACKED __attribute__((packed))
#include <iostream>
#include <cstring>
typedef	int       Boolean;
typedef uint8_t   Enumeration8;
typedef uint16_t  Enumeration16;
typedef int8_t    Integer8;
typedef uint8_t   UInteger8;
typedef int16_t   Integer16;
typedef uint16_t  UInteger16;
typedef int32_t   Integer32;
typedef uint32_t  UInteger32;
typedef int64_t   Integer64;
typedef uint8_t   Octet;
typedef Integer64 TimeInterval; /* nanoseconds << 16 */

/** On the wire time stamp format. */
struct Timestamp {
    uint16_t   seconds_msb; /* 16 bits + */
    uint32_t   seconds_lsb; /* 32 bits = 48 bits*/
    UInteger32 nanoseconds;
} PACKED;

/** Internal binary time stamp format. */
struct timestamp {
    uint64_t   sec;
    UInteger32 nsec;
};

struct ClockIdentity {
    Octet id[8];
};

struct PortIdentity {
    struct ClockIdentity clockIdentity;
    UInteger16           portNumber;
} PACKED;

struct PortAddress {
    Enumeration16 networkProtocol;
    UInteger16    addressLength;
    Octet         address[0];
} PACKED;

struct PhysicalAddress {
    UInteger16 length;
    Octet      address[0];
} PACKED;

struct ClockQuality {
    UInteger8     clockClass;
    Enumeration8  clockAccuracy;
    UInteger16    offsetScaledLogVariance;
} PACKED;

struct TLV {
    Enumeration16 type;
    UInteger16    length; /* must be even */
    Octet         value[0];
} PACKED;

///PTP Message composition
struct ptp_header {
    uint8_t             tsmt; /* transportSpecific | messageType */
    uint8_t             ver;  /* reserved          | versionPTP  */
    UInteger16          messageLength;
    UInteger8           domainNumber;
    Octet               reserved1;
    Octet               flagField[2];
    Integer64           correction;
    UInteger32          reserved2;
    struct PortIdentity sourcePortIdentity;
    UInteger16          sequenceId;
    UInteger8           control;
    Integer8            logMessageInterval;
} PACKED;

struct announce_msg {
    struct ptp_header    hdr;
    struct Timestamp     originTimestamp;
    Integer16            currentUtcOffset;
    Octet                reserved;
    UInteger8            grandmasterPriority1;
    struct ClockQuality  grandmasterClockQuality;
    UInteger8            grandmasterPriority2;
    struct ClockIdentity grandmasterIdentity;
    UInteger16           stepsRemoved;
    Enumeration8         timeSource;
    uint8_t              suffix[0];
} PACKED;

struct sync_msg {
    struct ptp_header   hdr;
    struct Timestamp    originTimestamp;
} PACKED;
struct ptp_message {

    union {
        struct ptp_header header;
        struct announce_msg announce;
        struct sync_msg sync;
    } PACKED;

};
struct a {
    int a;
    int b;
};

struct b {
    struct a A;
    int c;
};

struct c {
    struct a A;
    int c;
    uint32_t d;
};
struct animal {
    union {
        struct a A;
        struct b B;
        struct c C;
        uint8_t c[100];
    } __attribute__((packed));
};
int main() {

    struct b B{};
    struct animal Animal {0};
    B.A.a = -1;
    B.A.b = -2;
    B.c = 3;

    memcpy(&Animal,&B,sizeof(struct b));

    printf("0x%x   0x%x   0x%x\n",&Animal.A,&Animal.B,&Animal.C);




}