/**********************************************************************
*
* relay.h
*
* Definitions for PPPoE relay
*
* Copyright (C) 2001-2006 Roaring Penguin Software Inc.
*
* This program may be distributed according to the terms of the GNU
* General Public License, version 2 or (at your option) any later version.
*
* LIC: GPL
*
* $Id$
*
***********************************************************************/

#include "pppoe.h"
#include "event.h"

/* Description for each active Ethernet interface */
typedef struct InterfaceStruct {
    char name[IFNAMSIZ+1];	/* Interface name */
    int discoverySock;		/* Socket for discovery frames */
    int sessionSock;		/* Socket for session frames */
    int clientOK;		/* Client requests allowed (PADI, PADR) */
    int acOK;			/* AC replies allowed (PADO, PADS) */
    struct event *discoveryEvent;   /* Event for discovery packet to be read */
    struct event *sessionEvent;     /* Event for session packet to be read */
    unsigned char mac[ETH_ALEN]; /* MAC address */
} PPPoEInterface;

/* Session state for relay */
struct SessionPeerStruct;
typedef struct SessionStruct {
    struct SessionStruct *next;	/* Free list link */
    struct SessionStruct *prev;	/* Free list link */
    struct SessionPeerStruct *acPeer; /* Peer for AC MAC/Session */
    struct SessionPeerStruct *clientPeer; /* Peer for client MAC/Session */
    uint16_t sesNum;		/* Session number assigned by relay */
    unsigned int start;		/* Start time */
} PPPoESession;

/* Hash table entry to find sessions */
typedef struct SessionPeerStruct {
    struct SessionPeerStruct *next; /* Link in hash chain */
    struct SessionPeerStruct *prev; /* Link in hash chain */
    struct SessionPeerStruct *peer; /* Peer for this session */
    PPPoEInterface const *interface;	/* Interface */
    unsigned char peerMac[ETH_ALEN]; /* Peer's MAC address */
    uint16_t sesNum;		/* Session number */
    unsigned int epoch;		/* Epoch when last inbound activity was seen */
    PPPoESession *ses;		/* Session data */
} SessionPeer;

/* Function prototypes */

void relayGotSessionPacket(PPPoEInterface const *i);
void relayGotDiscoveryPacket(PPPoEInterface const *i);
PPPoEInterface *findInterface(int sock);
unsigned int hash(unsigned char const *mac, uint16_t sesNum);
SessionPeer *findSession(unsigned char const *mac, uint16_t sesNum);
SessionPeer *findDupSession(unsigned char const *mac, PPPoEInterface const *iface);
void deleteHash(SessionPeer *hash);
PPPoESession *createSession(PPPoEInterface const *ac,
			    PPPoEInterface const *cli,
			    unsigned char const *acMac,
			    unsigned char const *cliMac,
			    uint16_t acSes);
void freeSession(PPPoESession *ses, char const *msg);
void addInterface(char const *ifname, int clientOK, int acOK);
void usage(char const *progname);
void initRelay(int nsess);
void relayLoop(void);
void addHash(SessionPeer *sh);
void unhash(SessionPeer *sh);
void freeSessionPeer(SessionPeer *sh);
SessionPeer *allocSessionPeer();

void relayHandlePADT(PPPoEInterface const *iface, PPPoEPacket *packet, int size);
void relayHandlePADI(PPPoEInterface const *iface, PPPoEPacket *packet, int size);
void relayHandlePADO(PPPoEInterface const *iface, PPPoEPacket *packet, int size);
void relayHandlePADR(PPPoEInterface const *iface, PPPoEPacket *packet, int size);
void relayHandlePADS(PPPoEInterface const *iface, PPPoEPacket *packet, int size);

int addTag(PPPoEPacket *packet, PPPoETag const *tag);
int insertBytes(PPPoEPacket *packet, unsigned char *loc,
		void const *bytes, int length);
int removeBytes(PPPoEPacket *packet, unsigned char *loc,
		int length);
void relaySendError(unsigned char code,
		    uint16_t session,
		    PPPoEInterface const *iface,
		    unsigned char const *mac,
		    PPPoETag const *hostUniq,
		    char const *errMsg);

void alarmHandler(int sig);
void cleanSessions(void);
void PPPoE_cb_func(evutil_socket_t fd, short what, void *arg);

#define MAX_INTERFACES 8
#define DEFAULT_SESSIONS 8

/* Hash table size -- a prime number; gives load factor of around 6
   for 65534 sessions */
#define HASHTAB_SIZE 18917
