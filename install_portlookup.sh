# INSTALL PORTLOOKUP.sh
#   by Lut99
#
# A file which installs the portlookup files. Is smart enough to know when it
#   has been compiled or not, and whether it's talking about the server or
#   client content or both.



# Define the source directories
client_dir="install/client"
server_dir="install/server"

# Define the target directories
service_dir="/lib/systemd/system"
config_dir="/etc/portlookup"
daemon_dir="/usr/sbin"
binary_dir="/usr/bin"
header_dir="/usr/include"
dynlib_dir="/usr/lib"

# Define the files to install for the client & the server
client_sources=( "$client_dir/portlookup" "$client_dir/portlookup.h" "$client_dir/portlookup.hpp" "$client_dir/libportlookup.so" )
client_targets=( "$binary_dir/portlookup" "$header_dir/portlookup.h" "$header_dir/portlookup" "$dynlib_dir/libportlookup.so" )
server_sources=( "$server_dir/portlookup.conf" "$server_dir/portlookup" "$server_dir/portlookupctl" "$server_dir/portlookup.service" )
server_targets=( "$config_dir/portlookup.conf" "$daemon_dir/portlookup" "$binary_dir/portlookupctl" "$service_dir/portlookup.service" )

# First, make sure we're in the correct directory
# Credits: https://stackoverflow.com/a/24112741
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

# Determine if we're install or deinstalling
mode="installing"
if [ "$#" -eq 1 ] && [ "$1" == "deinstall" ]; then
    mode="deinstalling"
elif [ "$#" -gt 0 ]; then
    echo "Usage: $0 [deinstall]"
    exit 0
fi

# Switch on the install mode
if [ "$mode" == "installing" ]; then

    # Install the client first if any files have been compiled
    echo "Installing client..."
    if [ "$(ls -A $client_dir)" ]; then
        echo "   Copying files..."
        for i in ${!server_sources[@]}; do
            echo "      Copying '${server_sources[$i]}' to '${server_targets[$i]}'..."
            cp "${server_sources[$i]}" "${server_targets[$i]}"
        done

        echo "   Rebuilding dynamic linker cache..."
        ldconfig

        echo "   Done"
    else
        echo "   Client not compiled."
    fi

    # Then, install the server if compiled
        echo "Installing server..."
    if [ "$(ls -A $server_dir)" ]; then
        echo "   Creating configuration directory '$config_dir'..."
        mkdir -p "$config_dir"

        echo "   Copying files..."
        for i in ${!server_sources[@]}; do
            echo "      Copying '${server_sources[$i]}' to '${server_targets[$i]}'..."
            cp "${server_sources[$i]}" "${server_targets[$i]}"
        done

        echo "   Registring service..."
        systemctl daemon-reload
        systemctl enable portlookup

        echo "   Done"

    else
        echo "   Server not compiled."
    fi

elif [ "$mode" == "deinstalling" ]; then

    # Remove all client files
    echo "Installing client..."
    echo "   Removing files..."
    for i in ${!server_sources[@]}; do
        echo "      Removing '${server_targets[$i]}'..."
        rm -f "${server_targets[$i]}"
    done

    echo "   Rebuilding dynamic linker cache..."
    ldconfig

    echo "   Done"

    # Then, install the server if compiled
    echo "Installing server..."
    echo "   Stopping service..."
    systemctl disable portlookup

    echo "   Removing files..."
    for i in ${!server_sources[@]}; do
        echo "      Removing '${server_targets[$i]}'..."
        rm -f "${server_targets[$i]}"
    done

    echo "   Removing configuration directory '$config_dir'..."
    rm -rf "$config_dir"

    echo "   Done"

else
    echo "Unknown mode '$mode'"
    exit -1
fi

# We're done!
exit 0
