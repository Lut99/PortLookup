/* CONSTANTS.h
 *   by Lut99
 *
 * Created:
 *   10/01/2021, 13:08:35
 * Last edited:
 *   10/01/2021, 14:03:31
 * Auto updated?
 *   Yes
 *
 * Description:
 *   File that contains common constants used in both the CLI and the
 *   Daemon part of the portlookup daemon.
**/

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Index in the socket list of the CLI socket. */
#define CLI_FD 0
/* Defines the location of the UNIX-domain socket that the CLI uses to communicate with the daemon. */
#define CLI_SOCKET_PATH "/run/portlookup/portlookup.socket"
/* Defines the maximum number of connections for the CLI server side. */
#define MAX_CLI_CONNECTIONS 2
/* The maximum length a single CLI message may have. */
#define MAX_CLI_MESSAGE_LENGTH 512

/* Index in the socket list of the network socket. */
#define NETWORK_FD 1

#endif
