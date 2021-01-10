# PortLookup
A small ubuntu daemon that can be used to discover which ports are used by which application on a single server. Particularly useful when a server runs multiple daemon-like services who's ports may be subject to change.

## Installation
### Server-side
The installation of this daemon assumes that you install it on a systemd-supported Linux distribution.  
To install, compile the code first with ```make server```, and then copy it to the relevant directories with ```sudo make install```.  
To de-install the daemon again, simply run the ```install_portlookup.sh deinstall``` as sudo. Copy this script somewhere else, as it's not guaranteed to be backwards compatible.

### Client-side
The installation of the client side install a small executable to get the port number manually, and a tiny C/C++ library that allows one to query dynamically from projects.  
To install, compile the code first with ```make client```, and then copy it to the relevant directories with ```sudo make install```.
To de-install the library and executable again, simply run the ```install_portlookup.sh deinstall``` as sudo. Copy this script somewhere else, as it's not guaranteed to be backwards compatible.

## Configuration
You can configure the port lookup daemon using its configuration file or using the CLI-interface ```portlookupctl```.

### Configuration file
TBD

### Command Line Interface
TBD

## Usage
To use the service of looking up ports as a daemon, you can either use the CLI-tool or the C/C++ library.

### CLI tool


### C/C++ Library

