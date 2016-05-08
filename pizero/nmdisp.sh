#!/bin/bash

# Runs nmap -sn <network>.

network_ip=""
ignore_ip=""

if [ "$#" -lt 1 ]; then
    echo "Incorrect number of arguments."
fi

network_ip=$(echo "$1" | cut -d' ' -f1 | cut -d. -f-3)

for input in $@; do
    ignore_ip="$input $ignore_ip";
done
ignore_ip=$(echo "$ignore_ip" | xargs)

echo "Checking for devices on $network_ip.1-255..."
echo "Ignoring $ignore_ip..."

while true; do
    output=$(nmap -sn "$network_ip.0-255"| grep "($network_ip.*)" | cut -d" " -f6 | cut -d"(" -f2 | cut -d")" -f1)

    echo
    echo "Found devices on the following IPs:"
    for ip in $ignore_ip; do
        output=$(echo "$output" | sed "/\<$ip\>/d")
    done

    echo "$output"

    sleep 3s
done