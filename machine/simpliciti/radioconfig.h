#ifndef RADIOCONFIG_H
#define RADIOCONFIG_H

/* ---------------- Konfiguration von SimpliciTI ----------------- */

/* MRFI auf CC430 einstellen */
#define MRFI_CC430

/* Ersetzt .dat-Datei mit Compilerschaltern */
/* max hop count */
#define MAX_HOPS 3

/* max hops away from and AP. Keeps hop count and therefore replay
 * storms down for sending to and from polling End Devices. Also used
 * when joining since the EDs can't be more than 1 hop away.
 */
#define MAX_HOPS_FROM_AP 1

/* Maximum size of Network application payload. Do not change unless
 * protocol changes are reflected in different maximum network
 * application payload size.
 */
#define MAX_NWK_PAYLOAD 9

/* Maximum size of application payload */
#define MAX_APP_PAYLOAD 10

/* default Link token */
#define DEFAULT_LINK_TOKEN 0x01020304

/* default Join token */
#define DEFAULT_JOIN_TOKEN 0x05060708

/* Remove 'x' corruption to define Frequency Agility as active for this build */
#define xFREQUENCY_AGILITY

/* Remove 'x' corruption to enable application autoacknowledge support. Requires extended API as well */
#define xAPP_AUTO_ACK

/* Remove 'x' corruption to enable Extended API */
#define xEXTENDED_API

/* Remove 'x' corruption to enable security. */
#define xSMPL_SECURE

/* Remove 'x' corruption to enable NV object support. */
#define xNVOBJECT_SUPPORT

/* Remove 'x' corruption to enable software timer. */
#define SW_TIMER


/* aus smpl_config.dat */

/* Number of connections supported. each connection supports bi-directional
 * communication.  Access Points and Range Extenders can set this to 0 if they
 * do not host End Device objects
 */
#define NUM_CONNECTIONS 2

/*  ***  Size of low level queues for sent and received frames. Affects RAM usage  ***  */

/* AP needs larger input frame queue if it is supporting store-and-forward
 * clients because the forwarded messages are held here. Two is probably enough
 * for an End Device
 */
#define SIZE_INFRAME_Q 2

/* The output frame queue can be small since Tx is done synchronously. Actually
 * 1 is probably enough. If an Access Point device is also hosting an End Device 
 * that sends to a sleeping peer the output queue should be larger -- the waiting 
 * frames in this case are held here. In that case the output frame queue should 
 * be bigger. 
 */
#define SIZE_OUTFRAME_Q 2

/* This device's address. The first byte is used as a filter on the CC1100/CC2500
 * radios so THE FIRST BYTE MUST NOT BE either 0x00 or 0xFF. Also, for these radios
 * on End Devices the first byte should be the least significant byte so the filtering
 * is maximally effective. Otherwise the frame has to be processed by the MCU before it
 * is recognized as not intended for the device. APs and REs run in promiscuous mode so
 * the filtering is not done. This macro intializes a static const array of unsigned
 * characters of length NET_ADDR_SIZE (found in nwk_types.h). the quotes (") are
 * necessary below unless the spaces are removed.
 */
#define THIS_DEVICE_ADDRESS {0x79, 0x56, 0x34, 0x12}

/* device type */
#define END_DEVICE

/* For polling End Devices we need to specify that they do so. Uncomment the 
 * macro definition below if this is a polling device. This field is used 
 * by the Access Point to know whether to reserve store-and-forward support 
 * for the polling End Device during the Join exchange.
 */
/* #define RX_POLLS */


#endif
