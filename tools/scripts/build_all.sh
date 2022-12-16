#!/bin/bash
#for all

#1. Script: build_all.sh
#
#1.2 How to build all projects?
#    (1) In sdk home directory,execute the following command:
#        source sourceme-xxx.sh
#    (2) In any directory,execute the following command:
#        source build_all.sh [path] [option]
#        [path] : 
#            <1> An absolute path
#            <2> A relative path based on current path
#            <3> A partial path name with "*"
#            <4> The current path with "."
#        [option] :
#            <1> The default is empty,meaning build
#            <2> clean
#1.3 Note
#    When represenr partial path name with "*",Please enclose them in double quotation marks " ".


SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SDK_HOME_PATH="$(cd "$SCRIPT_PATH/../../" && pwd)"
EXEC_PATH="$(pwd)"
cd $SDK_HOME_PATH
exception=0

source ./sourceme-lin.sh
if [ $? -ne 0 ]; then
    echo -e "\e[31;1m[fail][The home path error:\e[33;1m $SDK_HOME_PATH\e[31;1m]\e[0m";cd $EXEC_PATH
    ((exception++))
else
    echo -e "\e[32;1m[note][source successful]\e[0m";cd $EXEC_PATH
fi

if [ ! -n "$1" ]; then
    echo -e "\e[31;1m[fail][\e[33;1m$1\e[31;1m][Please enter a correct\e[33;1m path\e[31;1m][$EXEC_PATH/\e[33;1mpath\e[31;1m]\e[0m";
    ((exception++))
fi

if [ $# -gt 2 ]; then
    echo -e "\e[31;1m[fail][\e[33;1m$*\e[31;1m][Please enter the correct parameter:\e[33;1m[path option]\e[31;1m,otherwise try enclosing parameters with \" \"]\e[0m";
    ((exception++))
fi

OPTION="$(echo "$2" |& grep -w -E "clean")"
if [ -z "$OPTION" ] && [ -n "$2" ]; then
    echo -e "\e[31;1m[fail][Please enter one of the options:\e[33;1m clean\e[31;1m ,default: ]\e[0m";
    ((exception++))
fi

MAKEFILES_PATH="$(find $EXEC_PATH/$1 -name Makefile -exec dirname {} \; 2>/dev/null| grep "$1"|sort -r)"

if [ ! -n "$MAKEFILES_PATH" ]; then
    echo -e "\e[31;1m[fail][No Makefile in the \e[33;1mpath\e[31;1m][\e[33;1m$EXEC_PATH/$1\e[31;1m]\e[0m";
    ((exception++))
fi

error=0
ERROR_LISTS=
#>> $EXEC_PATH/log 1>&1
if [ $exception -eq 0 ]; then
    for path in $MAKEFILES_PATH;
    do
        ERROR_LIST=$(make $OPTION --no-print-directory -C $path 2>&1 >/dev/null)
        if [ -n "$ERROR_LIST" ]; then
            ERROR_LISTS="${ERROR_LISTS}\n${ERROR_LIST}\e[31;1m\n[fail][${OPTION:-"build"} failed][$path]\n\e[0m"
            echo -e "\e[31;1m[fail][${OPTION:-"build"} failed][$path]\e[0m";
            ((error++))
            # break
        else
            echo -e "\e[32;1m[note][${OPTION:-"build"} successful][$path]\e[0m";
        fi
    done
fi

if [ $? -eq 0 ] && [ $error -eq 0 ] && [ $exception -eq 0 ]; then
    echo -e "\e[32;1m[pass][${OPTION:-"build"} all successful]\e[0m";
elif [ $error -ne 0 ] && [ $exception -eq 0 ]; then
    echo -e "\e[47;31m[fail][${OPTION:-"build"}][***********************************error list***********************************]\e[0m";
    echo -e "$ERROR_LISTS";
    echo -e "\e[31;1m[fail][${OPTION:-"build"} failed project number:\e[33;1m$error\e[31;1m]\e[0m";
else
    echo -e "\e[31;1m[fail][${OPTION:-"build"} exception number:\e[33;1m$exception\e[31;1m]\e[0m";
fi

cd $EXEC_PATH