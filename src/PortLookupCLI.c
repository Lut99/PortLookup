/* PORT LOOKUP CLI.c
 *   by Lut99
 *
 * Created:
 *   10/01/2021, 12:27:54
 * Last edited:
 *   10/01/2021, 14:20:06
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
 *   This is the CLI-part of the daemon, which can be used to set one-time
 *   configurations (i.e., non-persistent across reboots) or to edit the
 *   configuration file in an easy manner (for persistent changes).
**/

/***** ENTRY POINT *****/
int main() {
    
}
