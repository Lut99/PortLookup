/* PORT LOOKUP DAEMON.c
 *   by Lut99
 *
 * Created:
 *   10/01/2021, 12:25:45
 * Last edited:
 *   10/01/2021, 17:26:39
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The PortLookup program is designed to be run as a systemd daemon to
 *   serve as a centralized, network-accessible interface to determine
 *   which ports are used by a few specific applications. The server admin
 *   can register applications to use certain ports, which can then be
 *   queried for by clients.
 * 
 *   This is the server/daemon part of the software, which handles incoming
 *   network queries based on the configuration as given by the CLI-interface
 *   as well as by the configuration file.
**/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#include "Constants.h"


/***** GLOBALS *****/
/* The unix domain socket used to communicate with the cli. */
struct pollfd sockets[2];
/* The 'address' of the unix domain socket. */
struct sockaddr_un cli_address;
/* Determines whether or not the application still runs. */
int running;





/***** HELPER FUNCTIONS *****/
/* Initializes the daemon, resetting it if already started. */
void init() {
    syslog(LOG_INFO, "Initializing portlookup daemon");

    /* Setup the unix domain socket. */
    // Create a new domain socket with automatic protocol (0)
    sockets[CLI_FD].fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockets[CLI_FD].fd == -1) {
        syslog(LOG_ERR | LOG_CRIT | LOG_EMERG | LOG_ALERT, "Could not create CLI socket: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Make sure that there isn't already a socket at the given location
    if (remove(CLI_SOCKET_PATH "/" CLI_SOCKET_FILE) == -1 && errno != ENOENT) {
        syslog(LOG_ERR | LOG_CRIT | LOG_EMERG | LOG_ALERT, "Could not remove previous CLI socket file (" CLI_SOCKET_PATH "/" CLI_SOCKET_FILE "): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Prepare the "address", i.e., the file
    memset(&cli_address, 0, sizeof(struct sockaddr_un));
    cli_address.sun_family = AF_UNIX;
    strncpy(cli_address.sun_path, CLI_SOCKET_PATH "/" CLI_SOCKET_FILE, sizeof(cli_address.sun_path) - 1);

    // With the file out of the way and the address known, we'll bind the socket
    if (bind(sockets[CLI_FD].fd, (struct sockaddr*) &cli_address, sizeof(struct sockaddr_un)) == -1) {
        syslog(LOG_ERR | LOG_CRIT | LOG_EMERG | LOG_ALERT, "Failed to bind CLI socket to file (%s): %s", cli_address.sun_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Tell the socket to go into server mode
    if (listen(sockets[CLI_FD].fd, MAX_CLI_CONNECTIONS) == -1) {
        syslog(LOG_ERR | LOG_CRIT | LOG_EMERG | LOG_ALERT, "Failed to put CLI socket into listen mode: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Finally, tell it we want to write to this socket as well
    sockets[CLI_FD].events = POLLIN;



    /* Tell the system that we're running and we're done. */
    running = 1;
    syslog(LOG_INFO, "Initialization complete.");
}

/* Cleans up the program, by closing sockets & closing the system log. */
void cleanup() {
    syslog(LOG_INFO, "Stopping...");

    // Close the sockets
    if (close(sockets[CLI_FD].fd) == -1) {
        syslog(LOG_ERR, "Could not close CLI socket");
    }

    // Finally, close the systemlog
    syslog(LOG_INFO, "Stopped.");
    closelog();
}



/* Function that handles incoming messages from the CLI-side. */
void do_message(int client_fd, char* message, int message_length) {
    if (strncmp(message, "ping", 4) == 0) {
        // Simply send a ping back, done
        write(client_fd, "ping", 4);
    }
}





/***** ENTRY POINT *****/
int main() {
    // First, intialize the systemlog
    openlog("portlookup", LOG_PID | LOG_CONS, LOG_DAEMON);

    // Initialize ourselves to empty
    init();

    // Loop to accept any socket connections
    while (running) {
        // Wait for any socket to do something
        int poll_result = poll(sockets, 1/*2*/, -1);
        if (poll_result == 0) {
            // Nothing eventful happened
            continue;
        } else if (poll_result == -1) {
            // Error occurred
            syslog(LOG_ERR | LOG_CRIT | LOG_EMERG | LOG_ALERT, "Failed to poll sockets: %s", strerror(errno));
            return EXIT_FAILURE;
        }

        // Check if the client socket tells us anything
        if (sockets[CLI_FD].revents & POLLIN) {
            // Accept the new connection
            int client_fd = accept(sockets[CLI_FD].fd, NULL, NULL);

            // Read at most MAX_CLI_MESSAGE_LENGTH bytes
            char buffer[MAX_CLI_MESSAGE_LENGTH];
            int message_length = read(client_fd, buffer, MAX_CLI_MESSAGE_LENGTH);
            if (message_length == -1) {
                // Error occurred
                syslog(LOG_ERR, "Could not read from CLI socket: %s", strerror(errno));
                if (close(client_fd) == -1) {
                    syslog(LOG_ERR, "Could not close CLI client socket");
                }
                continue;
            } else if (message_length <= 0) {
                // Nothing received; ignore
                syslog(LOG_WARNING, "Empty message received from CLI socket");
                if (close(client_fd) == -1) {
                    syslog(LOG_ERR, "Could not close CLI client socket");
                }
                continue;
            }

            // Do actions based on that message
            do_message(client_fd, buffer, message_length);

            // Close the socket again
            if (close(client_fd) == -1) {
                syslog(LOG_ERR, "Could not close CLI client socket");
            }
            continue;
        } else if (sockets[NETWORK_FD].revents & POLLIN) {
            // Nothing as of yet
            continue;
        }
    }

    // Clean up
    cleanup();
}
