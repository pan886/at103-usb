#!/bin/bash
export TOP_DIR=`pwd`/../../..
export TOOLS_DIR=$TOP_DIR/tools

## for JTAG interface
## for openocd
export OPENOCD_HOME=$TOOLS_DIR/lin/jtag/openocd-0.10.0-0-20201205-1300/
export PATH=$OPENOCD_HOME/bin:$PATH
export PATH=$OPENOCD_HOME/scripts:$PATH

## for at103 gnu tools
export PATH=$TOOLS_DIR/lin/gcc/7.1.1-20181125-20201113-1236/bin/:$PATH

## for at103 udl tools
export PATH=$TOOLS_DIR/lin/udl/:$PATH

export AT_SDK_HOME_SH=`pwd`/../..

export RELEASE=no

