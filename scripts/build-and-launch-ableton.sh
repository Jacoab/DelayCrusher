#!/bin/bash

# Ableton cannot load the updated plugin after it has been re-built and
# I was tired of manually opening and closing it every time I made a change, 
# so this script automates that process.

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo ""
    echo "Builds the project and restarts Ableton Live with the specified project."
    echo ""
    echo "Usage:"
    echo "  ./build-and-launch-ableton.sh path/to/project.als"
    exit 0
else
    echo "############################################################"
    echo "Closing Ableton Live 12 Suite..."
    echo "############################################################"
    osascript -e 'quit app "Ableton Live 12 Suite"'

    echo "############################################################"
    echo "Building project..."
    echo "############################################################"
    cmake --build build

    echo "############################################################"
    echo "Launching Ableton Live 12 Suite with project $1"
    echo "############################################################"
    open -a "Ableton Live 12 Suite" "$1"
fi