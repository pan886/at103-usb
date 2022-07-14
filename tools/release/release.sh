#!/bin/bash

RELEASE=yes

# we suppose that we are the 1 level upper the release folder. 

if [ "$1" ];then

    if [ -d "$1" ];then
        echo ""
        SDK_HOME_PATH="$(cd "$1" && pwd)";
        echo -e "\e[44;30m[note][user sdk path: $SDK_HOME_PATH]\e[0m";
        echo ""
    else
        echo -e "\e[31;1m[$BASH_SOURCE][line:$LINENO][fail][no path:\e[33;1m $1\e[31;1m]\e[0m";
        exit 1;
    fi
else
    SDK_HOME_PATH=$AT_SDK_HOME_SH;
    if [ -d "$SDK_HOME_PATH" ];then
        echo ""
        echo -e "\e[44;30m[note][default sdk path: $SDK_HOME_PATH]\e[0m";
        echo ""
    else
        echo -e "\e[31;1m[$BASH_SOURCE][line:$LINENO][fail][no path is entered, so please source sourceme-lin.sh to get default sdk path.]\e[0m";
        exit 1;
    fi
fi

RELEASE_SDK_NAME="$(basename $SDK_HOME_PATH)-release"
RELEASE_SDK_PATH="$SDK_HOME_PATH/../$RELEASE_SDK_NAME"
if [ -d "$RELEASE_SDK_PATH" ];then
    echo ""
    RELEASE_SDK_PATH="$(cd "$RELEASE_SDK_PATH" && pwd)";
    echo -e "\e[31;1m[$BASH_SOURCE][line:$LINENO][warning][this directory already exists and may be overwritten:\e[33;1m $RELEASE_SDK_PATH\e[31;1m]\e[0m";
    echo ""
else
    echo ""
    RELEASE_SDK_PATH="$(mkdir "$RELEASE_SDK_PATH" && cd "$RELEASE_SDK_PATH" && pwd)";
    echo -e "\e[46;30m[note][release sdk path: $RELEASE_SDK_PATH]\e[0m";
    echo ""
fi

cp -aft "$RELEASE_SDK_PATH" "$SDK_HOME_PATH/."

source $RELEASE_SDK_PATH/sourceme-lin.sh;

RELEASE_FUNCTION_PATH=$RELEASE_SDK_PATH/tools/release;
if [ -d "$RELEASE_FUNCTION_PATH" ];then
    cd $RELEASE_FUNCTION_PATH;
    source release-function.sh;
    cd - > /dev/null;
else
    echo -e "\e[31;1m[$BASH_SOURCE][line:$LINENO][fail][no path:\e[33;1m $RELEASE_FUNCTION_PATH\e[31;1m]\e[0m";
    exit 1;
fi


echo -e "\n\e[93m#STEP1: Delete some directories and files...\e[0m\n"

cd $RELEASE_SDK_PATH
rm $RELEASE_SDK_PATH/test -rf
rm $RELEASE_SDK_PATH/doc -rf
rm -rf .git
rm .gitignore
rm .clang-format

echo -e "\n\e[93m#STEP2: Rename files...\e[0m\n"

if [ -n "$2" ];then

FILE_LIST= FILE_LIST="$(find $RELEASE_SDK_PATH -type f 2>/dev/null| grep -iE "\.h$|\.c$|\.lds$|\.S$|\.mk$|\.gdb$|\/Makefile$|\.sh$"|sort)";
FILE_MATCH_TYPE='at103';
FILE_MATCH_TARGET="${2,,}";
match_string_replace "${FILE_LIST[*]}" "$FILE_MATCH_TYPE" "$FILE_MATCH_TARGET";

FILE_MATCH_TYPE='AT103';
FILE_MATCH_TARGET="${2^^}";
match_string_replace "${FILE_LIST[*]}" "$FILE_MATCH_TYPE" "$FILE_MATCH_TARGET";

FILE_LIST= FILE_LIST="$(find $RELEASE_SDK_PATH -type f 2>/dev/null| grep -iE "\.h$|\.c$|\.lds$|\.S$|\.mk$|\.gdb$|\.sh$"|sort)";
FILE_MATCH_TYPE='at103';
FILE_MATCH_TARGET="${2,,}";
match_filename_replace "${FILE_LIST[*]}" "$FILE_MATCH_TYPE" "$FILE_MATCH_TARGET";

FILE_LIST= FILE_LIST="$(find $RELEASE_SDK_PATH -type f 2>/dev/null| grep -iE "\.h$|\.c$|\.lds$|\.S$|\.mk$|\.gdb$|\.sh$"|sort)";
FILE_MATCH_TYPE='AT103';
FILE_MATCH_TARGET="${2^^}";
match_filename_replace "${FILE_LIST[*]}" "$FILE_MATCH_TYPE" "$FILE_MATCH_TARGET";

fi

rm -rf $RELEASE_SDK_PATH/tools/release
sed -i 's/RELEASE=no/RELEASE=yes/' sourceme-lin.sh

source $RELEASE_SDK_PATH/sourceme-lin.sh;
echo -e "\n\e[93m#STEP3: Build all projects...\e[0m\n"
build_all.sh ./

echo -e "\n\e[93m#STEP4: Clean up project folders...\e[0m\n"
build_all.sh ./ clean
