#!/bin/sh
source sourceme-lin.sh
$OPENOCD_HOME/bin/openocd -f $OPENOCD_HOME/scripts/interface/ftdi/at1k-custom-ftdi.cfg -f $OPENOCD_HOME/scripts/target/i103.cfg