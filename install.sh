#! /bin/bash

# Install script for HRRT reconstruction binaries.
# Installs from development directory into ${HRRT_HOME}/bin etc.
# Note that HRRT development environment also requires installation
# (through Git mechanism ) of Perl scripts into ~/BIN/perl/bin and /lib.

if [ -z "$HRRT_HOME" ]; then
    echo "Set envt var HRRT_HOME to install directory root"
    exit 1
fi

rsync -tiv bin/* ${HRRT_HOME}/bin
chmod +x ${HRRT_HOME}/bin/*

rsync -tiv etc/* ${HRRT_HOME}/etc

# Where is hrrt_rebinner.lut installed from and to?
rsync -tiv lib/* ${HRRT_HOME}/lib
