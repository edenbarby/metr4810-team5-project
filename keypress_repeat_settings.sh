#!/bin/bash

echo "Keypress repeat delay: "`gsettings get org.gnome.settings-daemon.peripherals.keyboard delay`
echo "Keypress repeat interval: "`gsettings get org.gnome.settings-daemon.peripherals.keyboard repeat-interval`